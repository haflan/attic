# Terminal Sampler
*Apparently I have planned taact before, right here. This describes a lot of
functionality similar to the taact idea, so there's no reason to keep these two
separate*

## Motivation
I want to be able to use the terminal for making complete compositions,
*including* audio recordings. To do this I have to be able to place the audio
recordings at exactly the right place without seeing a graphical representation
of the waveform.

## Description
Inspired by the nblinc interface, this project should implement a sampler with 
a terminal UI that looks something like this:
    sample_name = audio_sample_file[10234:18243]
This defines a new audio sample that consists of the values from index 10234 to 
18243 in the with file name `audio_sample_file.wav` or something (loaded with
`libsndfile`). The interface should then work as described in the milestones:

## Milestone 1:
| Key name              | Action                                            |
|:----------------------|:--------------------------------------------------|
| `PLAY_SAMPLE`         | Plays the sample from start index to stop index.  |
| `START_INC_10000`     | Increase start index with 10000                   |
| `START_INC_100`       | Increase start index with 100                     |
| `...etc`              | ...other increase values                          |
| `STOP_INC_1000` etc.. | ...Same thing for the stop index.                 |

The idea is that you can easily trigger playback of the selected portion of the 
sample, so that listening for the optimal start index is fast as can be in the
terminal.

## Milestone 2:

| Key name              |     
|:----------------------|:--------------------------------------- 
| `LOOP_SAMPLE`         | Toggles looping of the sample \*.
|                       | 

\* If looping is on, the sample will loop until note-off. If not, it will only
play once. 

## Csound?
Unless it is easier to implement your own audio functionality, you should
consider building an interface for the [Csound looper](
http://www.csounds.com/manual/html/loscil.html) instead. Then you can simply
save everything as a Csound file. Only drawback is that it *might* be slower to
play a Csound file every time you press the playback button.

If you decide to go with the Csound approach instead of reinventing the wheel,
you can use this as part of a complete Csound TUI DAW!

## Plan (from paper notes)
1. Simply make a module for playing arrays as audio (with JACK or PortAudio or
   whatever). Not necessary if you choose Csound.
2. Make optional function for playing *part of* array.
3. Make simple ncurses interface with VIM-like keybindings for "scrolling through" 
   audio sample to set loop points. Let one finger trigger the playback of the
   audio / loop for a certain amount of time.
