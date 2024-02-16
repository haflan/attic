CC=gcc # What compiler to use
EXEC = nblinc
CFLAGS=-I./inc -g -Wall

IDIR=inc
SDIR=src
ODIR=obj

_DEPS = nblinc.h jackfunc.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = nblinc.o actions.o jackfunc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = `pkg-config --libs jack` -lsndfile -lm -lncurses -lcsound64

all: $(EXEC)	
	
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(LIBS) $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@  

.PHONY: clean

clean: 
	rm $(ODIR)/*.o $(EXEC) 
