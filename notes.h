/******************************************************************************
This is the file that defines all parts of the note detection fo rthe keybaord.
A lot of the stuff in here could be moved into a configuration file but I only 
have one keyboard that I want to use this whith, so I'm not going to bother.

This setup works for a Yamaha PortaSound PSS-11 with fully charged NiMH 
batteries at 1/2 volume (i.e. 7 increments up from the lowest volume setting)
******************************************************************************/
#define NUM_SAMPLES 4096
#define NUM_NOTES 32
#define WANTED_FREQ 96000

typedef struct {
    double value;
    int index;
    int note;
} NoteTrigger;

//The offset from the value in Notes to the corresponding Midi note value
const int MIDI_OFFSET = 41;

//Need an entry in here for each key on the keybaord
enum Notes
{
 FN1 = 0,
 FS1,
 GN1,
 GS1,
 AN2,
 AS2,
 BN2,
 CN2,
 CS2,
 DN2,
 DS2,
 EN2,
 FN2,
 FS2,
 GN2,
 GS2,
 AN3,
 AS3,
 BN3,
 CN3,
 CS3,
 DN3,
 DS3,
 EN3,
 FN3,
 FS3,
 GN3,
 GS3,
 AN4,
 AS4,
 BN4,
 CN4
};

//Values 1/2 volume (7 presses of the fol up from min vol)
NoteTrigger triggers[] = {
    {value: 9600.0, index:15, note:FN1},
    {value: 9000.0, index:4081, note:FN1},

    {value: 10000.0, index:16, note:FS1},
    {value: 10000.0, index:4080, note:FS1},

    {value: 9000.0, index:17, note:GN1},
    {value: 9000.0, index:4079, note:GN1},

    {value: 9000.0, index:18, note:GS1},
    {value: 9000.0, index:4078, note:GS1},

    {value: 8000.0, index:19, note:AN2},
    {value: 8000.0, index:4077, note:AN2},

    {value: 9000.0, index:20, note:AS2},
    {value: 9000.0, index:4076, note:AS2},

    {value: 9000.0, index:21, note:BN2},
    {value: 9000.0, index:4075, note:BN2},

    {value: 7500.0, index:22, note:CN2},
    {value: 7500.0, index:23, note:CN2},
    {value: 7500.0, index:4073, note:CN2},
    {value: 7500.0, index:4074, note:CN2},

    {value: 7000.0, index:24, note:CS2},
    {value: 7000.0, index:4072, note:CS2},

    {value: 7000.0, index:25, note:DN2},
    {value: 7000.0, index:4071, note:DN2},

    {value: 6000.0, index:26, note:DS2},
    {value: 7000.0, index:27, note:DS2},
    {value: 7000.0, index:4069, note:DS2},
    {value: 6000.0, index:4070, note:DS2},

    {value: 7800.0, index:28, note:EN2},
    {value: 8000.0, index:4068, note:EN2},

    {value: 9000.0, index:30, note:FN2},
    {value: 9000.0, index:4066, note:FN2},

    {value: 8000.0, index:31, note:FS2},
    {value: 7000.0, index:32, note:FS2},
    {value: 7000.0, index:4064, note:FS2},

    //Needs work
    {value: 7000.0, index:33, note:GN2},
    {value: 7000.0, index:34, note:GN2},
    {value: 5500.0, index:4062, note:GN2},
    {value: 7000.0, index:4063, note:GN2},    

    //Needs work
    {value: 7000.0, index:35, note:GS2},
    {value: 6000.0, index:36, note:GS2},
    {value: 7000.0, index: 4060, note:GS2},
    {value: 7500.0, index: 4061, note: GS2},

    {value: 8000.0, index:38, note:AN3},
    {value: 8000.0, index:4058, note:AN3},

    {value: 7500.0, index:40, note:AS3},
    {value: 7500.0, index:4056, note:AS3},

    {value: 9000.0, index:42, note:BN3},
    {value: 9000.0, index:43, note:BN3},
    {value: 75000.0, index:4054, note:BN3},
    {value: 7500.0, index:4053, note:BN3},

    {value: 7000.0, index:45, note:CN3},
    {value: 9000.0, index:4051, note:CN3},

    {value: 8000.0, index:47, note:CS3},
    {value: 8000.0, index:48, note:CS3},
    {value: 8000.0, index:4048, note:CS3},
    {value: 8000.0, index:4049, note:CS3},

    {value: 7500.0, index:50, note:DN3},
    {value: 7500.0, index:51, note:DN3},
    {value: 8000.0, index:4045, note:DN3},
    {value: 7500.0, index:4046, note:DN3},

    {value: 7500.0, index:53, note:DS3},
    {value: 7500.0, index:54, note:DS3},
    {value: 7500.0, index:4042, note:DS3},
    {value: 7500.0, index:4043, note:DS3},

    {value: 7500.0, index:56, note:EN3},
    {value: 7500.0, index:57, note:EN3},
    {value: 7500.0, index:4039, note:EN3},
    {value: 7500.0, index:4040, note:EN3},

    {value: 8000.0, index:60, note:FN3},
    {value: 8000.0, index:4036, note:FN3},

    {value: 9000.0, index:63, note:FS3},
    {value: 9000.0, index:4033, note:FS3},

    {value: 10000.0, index:67, note:GN3},
    {value: 10000.0, index:4029, note:GN3},

    {value: 10000.0, index:71, note:GS3},
    {value: 10000.0, index:4025, note:GS3},

    {value: 8000.0, index:75, note:AN4},
    {value: 8000.0, index:4021, note:AN4},

    {value: 10000.0, index:80, note:AS4},
    {value: 10000.0, index: 4016, note:AS4},

    {value: 8000.0, index:85, note:BN4},
    {value: 8000.0, index:4011, note:BN4},

    {value: 8000.0, index:90, note:CN4},
    {value: 8000.0, index:4006, note:CN4}

};
