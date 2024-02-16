# File names
EXEC = taact
_DEP = wavefile.h
_OBJ = taact.o wavefile.o

# Directory structure
SDIR=src
ODIR=obj
DEP = $(patsubst %,$(SDIR)/%,$(_DEP))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Compiler
CC=gcc # What compiler to use
CFLAGS=-I./src -g -Wall 

LIBS = `pkg-config --libs jack` -lsndfile -lm -lncurses -lcsound64

all: $(EXEC)	
	
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(LIBS) $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.c $(DEP)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@ 

clean: 
	rm $(ODIR)/*.o $(EXEC) 

ifm: internalfm all

internalfm: 
	$(eval CFLAGS += -DINTERNALFM)

.PHONY: clean ifm
