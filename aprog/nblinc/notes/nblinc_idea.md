- [!] I think a lot of this might be implemented already. 
 *You need to go through all the code and ideas and organize everything*.

Moving on:
==========

- 'Universal' Events: Events could contain all the information necessary for dealing with both MIDI and csound syntax:
    char inst;
    int note;
    /* for nblinc interface */
    double timestamp;
    double duration;
    struct Event * next;    /* next event in the events list */
    struct Event * prev;    /* previous event in the ev list */

    /* JACK functionality */
    /* linked event - if this event is a note on, the linked event is
     * a note off and vice versa. */
    struct Event * linked;  
    int midi_message;


As a JACK MIDI sequencer
------------------------
To achieve this you have to make sure note off events are placed correctly in the linked list
at the same time as note on events are placed, or their duration 

## Idea:
Instead of having a note event with start_time and duration, make all events *linked*.
I think that's what's up in seq24. So:

- When changing timestamp for a note on event, you are also changing the timestamp for the linked note off event
- When changing duration for a note on event, you are also changing the start duration 
- When placing a note on event, the note off event simply traverses the list until it finds an event with a bigger time stamp. 


## Reconsideration:
If nblinc is only going to be used with Csound, there is probably no need for midi functionality through JACK.
With no JACK Midi there is no point in including all the midi 'note off' events and such in the nblinc events.
Suggestion: Use two different event types, 'Event' and 'MidiEvent', where Event is the original nblinc events.
Let JACK Midi functionality be given as an argument, and only if that is given:
- Create and maintain a list of MidiEvents in addition to the list of nblinc events.

nblinc as a bigger thing
------------------------
This is some stuff that would be *very* useful if Csound is to be used for music 
production alone. It would still be useful when using Ardour (and maybe
LinuxSampler), but of course, then you have to prioritize what you spend time on. 
It might be hard to perform entire mixes using Csound and not Ardour, but
you could lay the groundwork in Csound and then use Ardour for "perfection".

# If used as a Csound controller
- Include a mixer mode where 'faders' are moved up/down using keys, for instance 
  q/a, w/r, f/s, p/t, l/n, u/e, y/i   etc.

# As a MIDI "heart"
- Give nblinc midi input capabilities to make all MIDI go through it.
- That way it can be used to control the digital piano easily, for instance:
    - An option to set which midi channel the piano maps to, and then quickly
      being able to change the instrument that the piano controls, be it an
      internal (Csound) instrument or external output to another JACK instrument.


   
