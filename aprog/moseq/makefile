CC=gcc # What compiler to use
EXEC = moseq
CFLAGS=-I. -g -Wall `pkg-config --cflags gtk+-3.0`
DEPS = audio.h gui.h
OBJ = moseq.o audio.o gui.o
LIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs jack` -lsndfile -lm

all: moseq	
	
moseq: $(OBJ)
	$(CC) -o $(EXEC) $(LIBS) $(OBJ)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@  

.PHONY: clean

clean: 
	rm *.o $(EXEC) 
