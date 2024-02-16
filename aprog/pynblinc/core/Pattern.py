from Events import Event
import ctcsound

class Pattern:
    next_pattern_num = 0
    def __init__(self, pt):
        self.__perfthread = pt
        self.__active_event = 0
        self.__events_list = []
        self.__tempo = [0, 120]     # default tempo is set to 120 at beat 0
        self.__name = "pattern {}".format(Pattern.next_pattern_num)
        Pattern.next_pattern_num += 1   # (Static?) class attribute

    # A class method is kind of an alternative to overloading constructors
    # This class method copies an existing pattern by first calling the Pattern
    # constructor, then setting the events list of the newly created pattern.

    @classmethod
    def from_existing(cls, pt, existing):
        new_pattern = cls(pt)
        new_pattern.__events_list = existing.events_list
        return new_pattern

    @classmethod
    def from_sco(cls, pt, score_list):
        """
        Makes a new pattern by parsing a score file. Does not support any other
        statements than i statements and a single t statement.
        I think this should be fine for now, at least for my use. f statements
        should be replaced by ftgen opcode in the orchestra anyway, as recommended by:
        http://csoundjournal.com/2006winter/encapsulatedInstruments.html
        pt -- Csound performance thread
        pt -- List of score lines read from a .sco file
        """

        new_pattern = cls(pt)
        new_pattern.events_list = []

        ### TODO: When you have included an instrument class you can use this
        # to handle empty fields, so that you fill in the last event with that
        # instrument or something. (Not that important if you use pynblinc for
        # all the composition, though...)

        ### TODO: I think some additional parsing is required to get the datatypes
        ### right.. And definitely if you're going to use filename strings in
        ### there eventually. Oh well...

        # parse score, event by event
        # not using regex, as the syntax is pretty limited
        for ev in score_list:
            # List comprehension for getting rid of blanks left by the split
            ev = [event for event in ev.split(' ') if event]

            # Handle cases where the statement letter and the first number are
            # not separated by space. Should probably rewrite this some day.
            if len(ev[0]) > 1:
                statement = ev[0][0]
                ev[0] = ev[0][1:]
            # if the letter is separated from the number, simply pop it from the list
            else:
                statement = ev.pop(0)
            if statement == 't':
                new_pattern.__tempo = ev
            elif statement == 'i':
                # FIXME: for God's sake, don't hard code like this
                pfields = [int(ev[0]), float(ev[1]), float(ev[2]), float(ev[3])]
                new_event = Event(pfields)
                new_pattern.events_list.append(new_event)

        return new_pattern

    def to_sco(self, filename):
        # TODO: Get pwd and save file there
        sco_test = ""
        for ev in self.events_list:
            sco_test += "{}\n".format(ev)

        with open(filename, 'w+') as f:
            f.write(sco_test)


    @property
    def tempo(self):
        return self.__tempo

    @property
    def name(self):
        return self.__name
    @name.setter
    def name(self, new_name):
        self.__name = new_name

    @property
    def active_event(self):
        return  self.__active_event
    @active_event.setter
    def active_event(self, event_to_set):
        self.__active_event = event_to_set

 # Don't really need num_events when we have 'len()', right?
  # @property
  # def num_events(self):
  #     return self.__num_events
  # @num_events.setter
  # def num_events(self, num_to_set):
  #     self.__num_events = num_to_set

    @property
    def events_list(self):
        return self.__events_list
    @events_list.setter
    def events_list(self, event_list_to_set):
        self.__events_list = event_list_to_set

    def insert_event(self, new_event):
        self.__events_list.insert(self.__active_event, new_event)

    def add_event(self, new_event):
        self.__events_list.append(new_event)

    #def sort_events(self):
    #    self.__events_list.sort(key=operator.attrgetter('start'))

    def num_events(self):
        return len(self.__events_list)

    def play_event(self):
        self.__events_list[self.__active_event].play(self.__perfthread)


    def toggleplay(self):
        """
        Creates a score out of all the events in the pattern,
        then plays that score with the given Performance Thread (pt).
        (( Currently hardcoded for testing))
        """
        score = []
        pt = self.__perfthread
        # Stop if already running?
        pt.flushMessageQueue()   # stop performance thread before processing all notes
        for event in self.events_list:
            pt.scoreEvent(False, 'i', event.pfields)
        pt.play()   # restart pt
    # TODO: Implement play, pause, stop etc.
    #       For this you need a *playing* attribute

    def play_from_current(self, pt):
        """
        Makes a score out of all events from the active_event and
        plays it in the given performance thread.
        """
        return

    def print(self):
        for ev in self.events_list:
            print(ev)

    def __str__(self):
        string = "<nblinc pattern with {} events>".format(len(self.__events_list))
        return string

    def fix_order(self, ts_change):
        """
        ts_change - negative if decreased
        """
        # Ugly, but seems to work
        el = self.__events_list
        done = False
        while not done:
            if ts_change < 0 and self.__active_event != 0:
                if el[self.__active_event].pfields[1] < el[self.__active_event-1].pfields[1]:
                    swapel   = el[self.__active_event]
                    el[self.__active_event]   = el[self.__active_event-1]
                    el[self.__active_event-1] = swapel
                    self.__active_event -= 1
                else:
                    done = True
            elif el[self.__active_event] != el[-1]:
                if el[self.__active_event].pfields[1] > el[self.__active_event+1].pfields[1]:
                    swapel   = el[self.__active_event]
                    el[self.__active_event]   = el[self.__active_event+1]
                    el[self.__active_event+1] = swapel
                    self.__active_event += 1
                else:
                    done = True
            else:
                done = True
        return

        ### OLD
        # TODO: Implement your own sorting function.
        # I think there's no other way of "following" the active event when it
        # suddenly skips multiple other events. Also, sorting the entire list
        # every time you change the timestamp of an event is not optimal anyway.
        #self.__events_list.sort(key = lambda x: x.pfields[1])
        return True

