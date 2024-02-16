Alternative uses of nblinc
==========================

## nblinc as JACK MIDI sequencer
+   don't have to learn to write plugins for SC
+   can be used with all software supporting JACK
+   can / must use JACK (transpose) as clock for entire project
+   Seems easy to set a JACK client to be [master of clock](http://jackaudio.org/files/docs/html/group__TransportControl.html)
     This means that it might be easy to give nblinc the power over transpose, which is a huge bonus.
+   knows that it might work. knows nothing about supercollider.
+   can get it up and running with sound by the end of the week
+   The case that has the most uses, because it can be used with everything supporting JACK.
*   When 'rough' / 'simple' composition is considered finished, a python script can be used to make
    supercollider code to include in case you want to do something more "manually" with the instruments,
    for instance portamento etc.
*   Seems like supercollider midi is not written with JACK support, so you have to use JACK with
    the 'a2jmidid' package (easy to install in Arch) to bridge
-   limited to midi events. can not control frequency of instruments directly,
    which would be nice for making portamento and such. Also OSC is generally more flexible than midi
-   has to rewrite the code to include note off events in the linked lists.
     / Though I don't know the amount of rewriting necessary for making an SC implementation.

### Requirements
So this requires the following setup:
- JACK, for connecting everything
- qjackctl, JACK GUI control (to make connections)
  I think the alternative is to make the JACK clients take 
  connection arguments at startup, which can be harder.
- a2jmidid, to bridge JACK and ALSA MIDI
    (add 'a2jmidid -e &' as a JACK 'after startup' script.)
- running nblinc as MIDI sequencer

- supercollider running with MIDI functionality, basically
  just used as an instrument. Of course, with a JACK setup you don't have
  to use supercollider at all. You can use any MIDI controllable software synth.

### Use cases
- nblinc:           As MIDI sequencer and clock controller
- JACK:             As connection kit, but controlled by nblinc
- SuperCollider:    As a MIDI controlled instrument among other JACK instruments.
- [ardour]:         For recording

3rd Alternative: Use an OSC library to send OSC events.
+   Can use OSC for everything
-   needs an external clock (unless clock from SC is sent through)

## nblinc as a Csound host
+   Needs csound only (AFAIK) (a big plus! Entire projects can in theory be saved in a single file)
    +   JACK not needed
    +   MIDI not needed
+   Csound seems much easier for the task of non-live composition than supercollider
-   SC seems more fit for live experimentation.
    
+   Don't have to learn to write plugins for SC

## Best of both worlds?
-   nblinc as csound host with optional JACK MIDI functionality?

# New ideas and musts
# Ideas:
-   a [m]ute symbol next to muted events. 
    options for muting entire instruments

# Musts:
 -  If using midi, you need note *off* events too, which means some modifications are needed to the event list code.
    Suggestion for how: Create a new linked list for note off events that is maintained during song play.
