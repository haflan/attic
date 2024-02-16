taact
=====
**T**erminal
**A**udio
**Act**ion  

**Everything under the TODO section should be considered, handled upon, and 
removed when all good**

TODO
----
This project turned into a couple of different projects, and requires a 
cleanup. Some points:
- There's no use for **getopt** (optargs) in an ncurses program, unless you're
  making an alternative backend for taact that supports terminal commands.
  If taact should be ncurses only, you can move the getopt stuff, eg. into the
  general programming repo
- The **lightwav** "sub-project" shouldn't really interfer with the taact
  project in the way it does now, as it is essentially a recreation of
  `libsndfile`.  Consider creating a new project for lightwav where you use the
  same format as below ("What?, why?, how?") to explain the motivation and such.
- `taact` should focus on implementing the following modules:
  + ncurses interface with keybindings for easily editing audio files
  + audio playback module that uses alsa, portaudio or jack behind the scenes.
  + sound file module that uses libsndfile or lightwav bts.

# Resources
Taken from another text dump file, but it fits here:

- GNU readline - Might be interesting for taact?
- alsa-project.org/alsa-doc/alsa-lib/pcm.html (ALSA API for aprog)

# From original idea paper - This should be moved somewhere
- `-r` should find `.wav` files recursively
- `-s` should calculate the SNR of the signal (using RMS)...
- posix threads for dealing with channels individually (mostly for learning
  posix threading)

**v  The following part is the original readme. Keep that  v**

What?
-----
A TUI program that lets the user perform audio editing in the terminal, with 
the ability to "scroll" through the audio file and play segments, like one 
would with programs like Audacity. 

_If I find that it's worth it, taact might also implement some audio effects,
mixing and other functionality found in Audacity. It could also be a good
opportunity to use some of the ideas found in mini\_projects.md
I suspect that sox, or at least Csound already has a lot of effects covered, 
though, so I will focus on creating a program for quickly editing audio 
samples in the terminal_

Why?
----
The program sox seems like a good program for certain audio editing tasks,
however a command based program can only go so far. Rerunning `play` and `trim`
commands until the timing is right seems tedious. `taact` will be a vim-like
audio editing program that lets you scroll through an audio clip with a chosen
level of precision (sec, ms, samples) and with a single button press play from 
the current position in the audio clip.

This way it should be easier to find the right place to slice the audio or set 
markers that can be used by other programs, for instance Csound or sox.

How?
----
As this will be a TUI, the easiest way to implement it might be with (n)curses,
although it is worth looking into using ANSI escape sequences directly to
determine keys pressed... 
