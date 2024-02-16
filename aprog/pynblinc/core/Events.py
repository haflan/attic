class Event():
    """
    General events class
    Contains a list of pfields and corresponding getters and setters
    """
    def __init__(self, pfields):
    # pfields is a list
        self.__pfields = pfields

    # General pfield getter
    @property
    def pfields(self):
        return self.__pfields

    ### SETTING AND CHANGING FIELDS ###
    # Shouldn't have a setter, as we need to be sure that set values are legal
    # General pfield setter, returns true if success
    def set_pfield(self, index, value, lower_lim, upper_lim):
        if index < 0 or index >= len(self.__pfields):
            return False    # return False if index error
        # If value out of range, set new value to the limit
        if lower_lim is not None and value < lower_lim:
            self.__pfields[index] = lower_lim
            # FIXME: This 'false' means that no redraw will happen when you
            # first hit the limit :
            return False
        if upper_lim is not None and value > upper_lim:
            self.__pfields[index] = upper_lim
            return False
        self.__pfields[index] = value
        return True

    def increase_pfield(self, index, increase, limit=None):
        """
        Increases or decreases a pfield with a given value.
        limit is upper or lower depinding on increase > 0
        """
        if index < 0 or index >= len(self.__pfields):
            return False    # return False if index error
        new_value = self.__pfields[index] + increase
        if increase < 0:
            return self.set_pfield(index, new_value, limit, None)
        else:
            return self.set_pfield(index, new_value, None, limit)

    def __str__(self):
        string = "i"
        for fld in self.__pfields:
            if type(fld) is float:
                string += "{:.4f} ".format(fld)
            else:
                string += "{} ".format(fld)
        return string

    def increase_field(self, field, increase, upper_limit):
        # Don't do anything if upper limit is reached
        if upper_limit and self.__pfields[field] == upper_limit:
            return
        self.__pfields[field] += increase

    def decrease_field(self, field, decrease, lower_limit):
        if lower_limit and self.__pfields[field] == lower_limit:
            return
        self.__pfields[field] -= decrease

    ### PLAY ###
    # [info about scoreEvent](https://github.com/fggp/ctcsound/blob/master/ctcsound.py)
    def play(self, pt):
        actual_timestamp = self.pfields[1] # Keep the original timestamp
        self.pfields[1]  = 0               # In order to play the event *right now*
        pt.scoreEvent(False, 'i', self.pfields)
        self.pfields[1]  = actual_timestamp
