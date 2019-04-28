#include "SDL.h"
#include <stdlib.h>
#include <fftw3.h>
#include <unistd.h>
#include <notes.h>
#include <rtmidi/RtMidi.h>

volatile int audio_count=0;
volatile int isSampling = 0;
volatile int firstRun =1;
volatile int currentNotes = 0;
int notes[2][NUM_NOTES];
double *in, *out;
fftw_plan p;
RtMidiOut *midiout = NULL;
bool verbose = false;
bool analyse = false;
int devNum = 0;
const char* inputDevice = NULL;
#define vebosetream \
    if (!verbose) {} \
    else std::cerr


void AudioCallback(void* userdata, uint8_t *stream, int len1){
    if(isSampling){
        return;
    }
    isSampling = 1;
    int audioVal =0;
    int maxSamp =0, minSamp =0;
    for(int i=0; i<len1; i++){
        int val = (int8_t)stream[i];
        in[i] = stream[i];
        maxSamp = (maxSamp < val) ? val :maxSamp;
        minSamp = (minSamp < val)? minSamp : val;
        int thisVal = stream[i] -128;
        if(thisVal < 0)
            thisVal = -1 * thisVal;
        
        audioVal += thisVal;
    }

    fftw_execute(p);
    double max = 0;
    int maxPos = -1;
    for(int i=14; i<(NUM_SAMPLES); i++){
        double thisVal = abs(out[i]);
        if(max <= thisVal){
            maxPos = i;
            max=thisVal;
        }
    }

    memset(notes[currentNotes], 0, sizeof(int)*NUM_NOTES);
    int numTriggers = sizeof(triggers) / sizeof(NoteTrigger);
    for(int i=0; i< numTriggers; i++){
        NoteTrigger* trigger = &(triggers[i]);
        if(abs(out[trigger->index]) > trigger->value){
            notes[currentNotes][trigger->note] = 1;            
        }
    }
    vebosetream << "Max pos:" << maxPos << " Max val:" << max << " audioval:" 
    << audioVal << "\n";
    
    int nextNotes = (0 == currentNotes) ? 1 : 0;
    if(!firstRun)
    {
        std::vector<unsigned char> message;
        for(int i=0; i<NUM_NOTES ;i++){
            if(notes[nextNotes][i] != notes[currentNotes][i]){
                message.clear();
                if(notes[currentNotes][i]){
                    std::cout << "Note " << i << " pressed\n";
                    message.push_back(144);
                    message.push_back(i + MIDI_OFFSET);
                    message.push_back(90);
                    midiout->sendMessage(&message);
                }
                else{
                    std::cout << "note " << i <<" released\n";
                    message.push_back(128);
                    message.push_back(i + MIDI_OFFSET);
                    message.push_back(90);
                    midiout->sendMessage(&message);
                }
            }
        }
    }
    else{
        firstRun =0;
    }
    memcpy(notes[nextNotes], notes[currentNotes], sizeof(int)*NUM_NOTES);
    audio_count++;
    isSampling =0;
}

void showHelp(){
    std::cout << "Usage: mic2midi [OPTION]...\n";
    std::cout << "\t -h\t\tshow this help message\n";
    std::cout << "\t -l\t\tList the available audio devices\n";
    std::cout << "\t -d [dev name]\tuse the specifed audio device\n";
    std::cout << "\t -v\t\tverbose logging\n";
}

int listDevs(){
    int count = SDL_GetNumAudioDevices(1);
    for (int i = 0; i < count; ++i) {
        std::cout << i << ":" << SDL_GetAudioDeviceName(i, 1) <<"\n";
    }
    return count;
}


int main(int argc, char* argv[]){
    //Initialise SDL before we do anything so we can list devs if we want to
    SDL_Init(SDL_INIT_AUDIO);
     // Check available ports.
    midiout  = new RtMidiOut();
    unsigned int nPorts = midiout->getPortCount();
    if ( nPorts == 0 ) {
        std::cout << "No ports available!\n";
        exit(1);
    }

    midiout->openVirtualPort();
 
    //Start up and process the command line options
    int option;
    while((option = getopt(argc, argv, "vlhd:"))!=-1){
        switch(option){
            case 'v':
                verbose = true;
                break;
            case 'd':
                inputDevice = optarg;
                break;
            case 'l':
                listDevs();
                exit(0);
            case 'h':
                showHelp();
                exit(0);
            default:
                std::cout <<"Invalid parameters\n";
                showHelp();
                exit(1);
        }
    }

    int count = SDL_GetNumAudioDevices(1);    
    if(0 > count){
        std::cout << "Failed to get audio devices with error " <<
        SDL_GetError() << "\n";
        exit(1);
    }
    else if(0 == count){
        std::cout << "No audio capture devices present\n";
        exit(0);
    }
    std::cout << "Found " << count << " audio devices \n";

    if(NULL == inputDevice){
        inputDevice = SDL_GetAudioDeviceName(0, 1);
    }

    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
    want.freq = WANTED_FREQ;
    want.format = AUDIO_U8;
    want.channels = 1;
    want.samples = NUM_SAMPLES;
    want.callback = AudioCallback;

    SDL_AudioDeviceID devId = SDL_OpenAudioDevice(inputDevice, 1, &want, &have,
     SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    
    if(0 == devId)
    {
         std::cout << "Failed to open " <<  inputDevice << "\n";
         exit(1);
    }

    std::cout << "Opend audio device " << inputDevice << "\n";

    //Set up for FFTs
    in = (double*) fftw_malloc(sizeof(double) * NUM_SAMPLES);
    out = (double*) fftw_malloc(sizeof(double) * NUM_SAMPLES);
    p = fftw_plan_r2r_1d(NUM_SAMPLES, in, out, (fftw_r2r_kind)FFTW_FORWARD, 
        FFTW_MEASURE);

    //Get everything going
    SDL_PauseAudioDevice(devId, 0);

    //TODO: put a singal handler in place that flips a boolean that breaks the loop
    while(1){
        SDL_Delay(1);
    }

    SDL_UnlockAudioDevice(devId);
    SDL_CloseAudioDevice(devId);

}
