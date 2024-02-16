pynblinc
========
`pynblinc` is the Python implementation of `nblinc` (*n*early *bli*nd
*c*omposition), which is kind of a middle ground between a tracker / step
sequencer and a piano roll. The goal of (py)nblinc is to achieve the following:

- Keyboard based pattern / melody composition without the need to write text.
- VIM-like keyboard bindings
- TUI instead of GUI based, i.e. using [curses](https://docs.python.org/3/howto/curses.html).

Although it can be extended to support MIDI or OSC output, pynblinc will first
and foremost be developed with *Csound* in mind. The basic idea is that pynblinc
will be used for composing a score, while nvim is used to define the orchestra
(instruments)

Suggested use
-------------
- Paper plan (high level composition). You can't avoid composition on paper.
  Don't try to!
- Jamming (recording and melody composition)
- Putting melodies and samples together.
- Use Csound with separate score (`.sco`) and orchestra (`.orc`) files.
- Use (py)nblinc to compose score files and (n)vim to define the orchestra.



Programming resources
---------------------
Documentation for some of the libraries used in pynblinc

### Mido
For midi support:

- [MIDO General](http://mido.readthedocs.io/en/latest/)
- [MIDI Files](https://github.com/olemb/mido/blob/master/mido/midifiles/midifiles.py)
- [MIDI Tracks](https://github.com/olemb/mido/blob/master/mido/midifiles/tracks.py)

### TUI
- [curses](https://docs.python.org/3/howto/curses.html)
- [more curses](https://docs.python.org/3/library/curses.html)


Notes on programming
--------------------
- As the score will be edited by pynblinc and the orchestra by vim it makes
  sense to use separate files.
- For this to work with the current program, the score must be limited to
  *i* statements only. This should be fine for now, because:
    1. *f* statements can be moved to the orchestra (using *ftgen* opcode, which
       is really the preferred way to load tables anyway)
    2. *t* statements aren't supported by ctcsound anyway, it seems.

  To deal with tempos, allow a single t statement as the first line of the score
  and parse this manually in pynblinc. 
