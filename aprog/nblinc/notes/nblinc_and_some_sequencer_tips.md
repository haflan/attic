Some sequencer tips
===================
Here are some tips for further development of nblinc and more generally for programming 
(midi) sequencers in C/C++.

## nblinc
### Event organization 
-   Events are stored in a doubly linked list. I don't want to use any external library for it,
    so the linked events list is implemented "manually". 
-   *nblinc sequence*: nblinc itself stores the sequence using a doubly linked list where every event contains the following:
    - timestamp (a double containing the number of beats from beat 0)
    - duration  (another double type timestamp)
    - note      (a char containing a midi note number)
    - instr     (a char that contains instrument number)
    - velocity  (char with note velocity)
    This should be easy to translate for instance to a Csound score, if nblinc is going to be used as a Csound host.
-   *MIDI sequence*: MIDI sequences consist of note on and note off events, instead of 'note of certain duration on',
    which complicates the organization of the event sequence a bit. Since this functionality will not always be used,
    nblinc will only make MIDI sequences (and start as a JACK client) if it is given an optional  argument at startup.

#### JACK Midi functionality
-   In addition to pointing to the next and the previous event in the sequence, each 'note on'
    event points to a corresponding 'note off' event, while each 'note off' points to the 'note on' (or just NULL).
-   *When an event is inserted*, the following happens with the MidiEvent list:
    1. A _note on_ event is inserted with timestamp equal to one of the alternatives listed:
        - timestamp of previous 'note on' (if any)
        - timestamp of next 'note off' (if any)
        - timestamp of the 'note off' linked to the previous 'note on',
          in other words, the inserted note starts where the last one ended.
       In addition to the timestamp, the other event properties must be set, like note, duration, velocity:
        - use the properties of the previous 'on' event
        - use the properties of the previous 'off' event
        - use some default properties
    2. A _note off_ event is added with timestamp equal to that of the 'note on' event + the
       duration of the note on event. If this timestamp is bigger than the timestamp of the
       next event in the events list, the list is traversed until a bigger timestamp is found.
-   *When an event is appended*, the sequencer jumps to the last element and inserts there. 
-   *When the note of an event is changed*, both the note on and the linked note off event will change notes.
-   *When the timestamp of an event is changed*, the timestamp of both note events are changed by the
    same amount. If any of the events then happens to be moved past another event, that event must be
    swapped with 
-   *When the duration of an event is changed*: Only the note off event has its timestamp changed,
    and if the new timestamp happens to be before the previous event or after the next event,
    the note off event must be swapped with the event it "moves past".

-   Because I won't be using external libraries for anything but sound processing, 
    I also have to deal with sorting myself, but this doesn't have to be too complex.
    Considering most notes will probably end before the new one starts, traversing 
    the list and comparing new timestamps with existing timestamps (the "naïve" way) 
    won't be too demanding.


### Why organize like this?
-   A linked list makes sense for a sequencer, since it is *sequential* after all.
    Linked lists have an insertion time of O(1), but a random access time of O(n).
    For a sequence we often want to insert events anywhere in the sequence, while
    playback is usually from first to last element, which corresponds to traversing
    a list from first to last element. 

- DECEMBER 2018 NOTES:
    + A linked list is a good idea for maintaining the sequence, but not for
      playback. LLs doesn't have data localities like arrays do, which less
      caching and slower access times. 
    + The best of both worlds: When pressing a play button, make an array from 
      the LL before starting the playback, and try to use as little logic within
      the `process` function as possible.


## seq24, code analysis (for inspiration)
### Event organization
-   Events in a sequence are stored in a C++ list
-   The list is resorted when new events are added

### More detailed (mostly about C++ lists)
-   (Note) Events are stored in objects of the *event* class (see event.h and event.cpp), 
    which are similar to the event datatype in nblinc.
-   *sequence* objects contain a C++ list of *events*: `list < event > m_list_event`.
-   C++ lists are doubly linked lists like in nblinc, but with some smart functionalities.
    One of the functionalities used by seq24 is the `list_name.sort()`, which sorts the
    linked list effectively (not sure which sorting algorithms are used, but I know that 
    it compares the elements in the list simply by using the standard <>= operators.)
-   *event* objects can be compared in `m_list_event.sort()` because the _operators are
    overloaded_, so that `event1 < event2` means roughly 
    `event1->timestamp < event2->timestamp`. That way the sorting algorithm can be used 
    as normal, and a list of events will be sorted by the event timestamps.
-   As I understand it, each time a note is placed in the sequence roll, an event is added
    to the `m_list_events` list before the list is resorted using .sort(). That way the
    events will always be sorted by their timestamp.

-   I have not yet found out how the events are played, but that shouldn't be too hard when
    the elements are already sorted in a list. It would be interesting to see, though, as
    my implementation doesn't seem ideal.
