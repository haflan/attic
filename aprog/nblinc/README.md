# Description
Description of current functionality and plans.

- ncurses interface (probably)

## First goal
- MIDI over JACK, using JACK transport as clock.
- Linked list over note events, sorted by event timestamps
- When 'p' is pressed, the time from JACK transport is registered,
  and notes in the event list are played in order when their timestamp
  is reached by the JACK transport.
- That way, the events are played in order by traversing the linked list.
  ```c
  if( event_to_play->timestamp <= jack_current_time - jack_start_time){
      /* play the given event, then jump to next in list: */
      event_to_play = event_to_play->next;
  }
  ```
- ncurses synced to the transport, so that it shows which note is currently
  being played. This can be done simply by putting a '<-' next to the current
  event\_to\_play.


# To do
## Functionality
- [ ] Make a Csound version of nblinc and eventually ...

### General
- [ ] Add a toggle-able "bar" view that shows "bar:beat:" instead of just beat.
- [ ] Implement save/load to text file. (consider implementing midi file support
  later)
- [ ] Start using [ncurses windows](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html)
      Until now everything has been drawn to the stdscr, which is not how ncurses is supposed to be used.
- [ ] Include scrolling of screen when max height is exceeded.
- [/] Set minimum and maximum values for some of the numbers. 'A'->'Z'. 
- [ ] Instead of hard coding the keybindings, make a header file like dwm for starters, and perhaps a runtime config file later
- [ ] Add velocity to the Events

- Using the ``focus_event()`` function:
    - [ ] Let a key ('g'?) set current\_event = event\_to\_play, so that there is a shortcut to edit played events.
    - [ ] Shortcut to go back to current\_event 
    - [ ] Shortcuts to go to first\_event and last\_event
- [ ] Yank and paste
- [ ] Visual (selection) mode
- [ ] Delete events
- [ ] Consider adding other events than note events / generalizing the events functionality
- [ ] Piano keyboard mode: Use keyboard as piano to find the right note. When
  found: Press a key a insert that note.
- [ ] Consider making dynamic array out of the linked list before playing it,
  for performance improvement..

### JACK MIDI
- [ ] *Better playback functionality*: 
    - [ ] *Multiple playback options*: Right now, 'p' toggles the jack transport, and the notes played depend on the transport. A better idea is probably to let the space bar trigger the transpose and play the sequence in 'song mode', while for instance pressing 'p' plays the current note and 'P' loops the previous 2 beats or so *independent* of the transport.
    - [ ] *starting transport at other time than 0.0*: If you start the transport from another time than 0.0 you need to make sure that the next\_timestamp and the event\_to\_play contains the right data. If that proves hard in some cases, simply traverse the list from the first\_event _without playing anything_ until you reach the event with the right timestamp.

## Functions
- [/] append\_events();   Seems to kind of work, just needs ncurses to catch up.
- [/] ch\_inst();         Needs cleanup and modifications.
- [/] ch\_note();         Needs cleanup and modifications.
- [/] ch\_dur();          Needs cleanup and modifications.



# Done
- [x] Started: *Organizing MIDI events*: At this moment note off events are handled realtime by a new linked list for *active* events. An alternative that is probably better is to keep a single list of (linked) note on and note off events, which depend on the position of one another. A list that is *not* created during playback.
- [x] *Fix premature note off problem*: Note on events seem to be triggered at the right times, but note off seems... off. Sounds like they are triggered almost instantly after their corresponding note on event. This will probably be fixed if you implement the fixes in the point above, though.
- [x] kill\_midseq();     v
- [x] make\_midiseq();    Seems to work. No more memory leaks reported from valgrind.
- [x] Typedef for note events (containing timestamp, duration, note etc)
- [x] Done with a linked list.
    ~~Implement a tone buffer that contains all of the existing notes plus the 
    one about to be placed, so that this can be used to decide the properties
    of the new note (the buffer will be played when 'p' is pushed).~~
- [x] The problem was probably caused by the process function containing a redraw\_all() function, which is way too slow.
~~Fix the [zombification](http://linux-audio-dev.linuxaudio.narkive.com/dl2MdZEz/jack-zombified) In other words, the process() function is probably too slow!
        - A possible solution to this is just setting a bunch of flags in process() that
          tells the rest of the program what to do with linked lists and such.
        - Another alternative might be to make a single list for active and non-active events 
          when pressing 'p', but this will make live composition a pain...~~
- [x] swap\_events():     Seems to be working as intended.
- [x] ch\_timestamp():    Working
- [x] focus\_event();     Working
- [x] insert\_event():    Working
- [x] Done, but still using ncurses completely wrong. ~~Clean up ncurses mess - consider just having one function for redrawing the entire screen at once, at least for starters. That way you avoid the messing of having to keep track of curX and curY all the time.~~
