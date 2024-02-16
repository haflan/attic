class Instrument:
    """
    An instrument class should probably contain information about pfield max
    value, min value, default value, data type, and optionally description or
    alternative name.
    """
    def __init__(self, pfmin, pfmax, pfdef, pfdesc, name):
        self.pfmin = pfmin
        self.pfmax = pfmax
        self.pfdef = pfdef  # defaults
        self.name = name

