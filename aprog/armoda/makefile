CC=gcc # What compiler to use
EXEC = armoda
CFLAGS=-I./include -g -Wall 

IDIR=./include
SDIR=src
ODIR=obj
SRC = armoda.c audio.c
_OBJ = armoda.o audio.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = audio.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
LIBS = `pkg-config --libs jack` -lsndfile -lm

all: $(EXEC)
	
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(LIBS) $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean: 
	rm -f $(ODIR)/*.o $(EXEC) 
