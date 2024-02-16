(not proper markdown, sorry)

# Two ideas for text based sequencer / tracker
## 1. 'Tracker' with constant line resolution:
-----------------------------------------------------------------
HEADER
-----------------------------------------------------------------
// resolution = 8 (1/8 beats per line)
// tempo        = 120 bpm
                = 16 lines per sec
( calculation)  = (120 bpm * 8 (lines / beat) / 60 (sec / min))
-----------------------------------------------------------------
COMPOSITION
-----------------------------------------------------------------
A(69, 0.5), A(73, 0.25, set: )

-----------------------------------------------------------------


## 2. Sequencer where each line starts with a time stamp (in beats):
- This has the advantage that the files will be more compact
- A disadvantage is that it might be harder to 
-----------------------------------------------------------------
HEADER
-----------------------------------------------------------------
tempo = 120 bpm
A     = "piano1"
B     = "violin"
-----------------------------------------------------------------
0:      A(69, .5), B(72, .25), filter1(res: 0.5)
0.5:    A((69-72, 0.25, 0.5, <curve>), 0.5)       
        ^ // Note lasts 0.5 beats, but after playing 0.25 beats,
          // it starts bending towards note 72 until the end of note
43.23:   

## 3. A hybrid:



# Is this a better alternative to a piano roll?
- Probably not for initial composing, but:
- When a composition is done, you have the advantage of having the
entire composition contained in one file and place.
- This makes it easy to build the instruments 'into' one another.
- An (ncurses?) interface can be made to make composition more 
effective with shortcuts.


# Idea for for effictive interface: 
- To make composition more effective, an ncurses interface can be 
made. This interface can simplify the process with vim-like shortcuts.
- In essence, this interface will be a piano roll for the blind.
- F
- In all modes:
    - m: button for toggling metronome (which is the basis for composing blindly)
    - 

- For each of th
- Insert mode
    - S:        move note to the closest whole beat on the left
    - T:        mmove note to the closest whole beat on the right
    - s:        move note left
    - t:        move note right
    - h:        shorten note
    - i:        lengthen note by a step
    - [#]n:     lower note by [#] steps
    - e:        higher note
    ^   Each of these buttons should trigger the new note, so that it is easier to
        hear the update
       

    - [#]w:     move note [#] beats forward (or other predifined amount of steps)
    - b:        move note [#] beats backward
    - space:    insert new note OR play entire composition
    - enter:    insert new note OR play entire composition
    - [#]p:     play the last [#] beats
    - [#]P:     toggle looping of the last [#] beats
    
- Normal mode: 
    - This should be implemented eventually, to make it easier to edit already
    existing compositions, BUT:
    - First get the insert interface working. 
    - The existing composition can be edited by editing the resulting text file.


- Configurations (use some defaults at first):
    - shortcut keys
    - What default value to use when inserting new notes:
        - Use the previously used instrument?
        - Start new note where the last stopped?
        - Use duration of last placed note?
    - 




