CC=gcc # What compiler to use
EXEC = showcase
CFLAGS=-I.. -I. -ggdb -Wall

# If deps are symlinks, `make` won't know when they've changed, I think?
# So ideally you should replace the symlinks with the path to the actual files
DEPS = showcase.h sound.h 
OBJ = showcase.c sound.c seqs.c 

LIBS = -lsndfile -lm 

all: $(EXEC)	
	
$(EXEC): $(OBJ)
	$(CC) -DSHOWCASE -o $(EXEC) $(LIBS) $(OBJ)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@  

.PHONY: clean
clean: 
	rm *.wav $(EXEC) 
