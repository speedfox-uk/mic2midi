CC=g++
CFILES=$(shell ls *.c)
CFLAGS=-D__UNIX_JACK__ -I.  -lfftw3 -lm -lrtmidi -ljack 
CFLAGS+=$(shell sdl2-config --cflags --libs)
OBJ=$(patsubst %.c,%.o,$(CFILES))

%.o: %.c notes.h
	$(CC) -c -o $@ $< $(CFLAGS)

mic2midi: $(OBJ) 
	$(CC) -o $@ $^  $(CFLAGS)

clean:
	rm *.o mic2midi 
