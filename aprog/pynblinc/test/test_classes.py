import unittest
from core import Events
# This requires that the test is started in the top-level directory
# (/pynblinc) using the following command:
#   python -m unittest discover test
# I have still not figured out relative imports

class TestEvent(unittest.TestCase):

    def setUp(self):
        self.ev = Events.Event([1, 0.0, 0.5, 60])

    def test_constructor(self):
        try:
            self.assertTrue(str(self.ev) == "i1 0.0000 0.5000 60 ")
        except AssertionError as ae:
            error_msg = "str(event) printed \"{}\".".format(str(ev))
            raise type(ae)(error_msg)

    def test_pfield_getter(self):
        self.assertEqual(self.ev.pfields[2], 0.5)
    def test_pfield_setter(self):
        self.ev.set_pfield(3, 3, 40, 80)
        self.assertEqual(self.ev.pfields[3], 40)
    def test_pfield_setter_toolow(self):
        self.ev.set_pfield(0, -4, 0, 5)
        self.assertEqual(self.ev.pfields[0], 0)
    def test_pfield_setter_toohigh(self):
        self.ev.set_pfield(0, 8, 0, 5)
        self.assertEqual(self.ev.pfields[0], 5)
    def test_pfield_setter_invalid_index(self):
        try:
            self.ev.set_pfield(9, 8, 0, 5)
        except IndexError:
            errmsg = "wrong index not handled correctly in pfield setter test"
            raise AssertionError(errmsg)

    #def test_pfield_limited_setter(self, new_val,

    #def test_increase(self):
    #    self.ev.pfield[0] = 3
    #    try:
    #        self.assertTrue(str(self.ev) == "i3 0.0000 0.5000 60 ")
    #    except AssertionError as ae:
    #        error_msg = "str(event) printed \"{}\".".format(str(ev))
    #        raise type(ae)(error_msg)



if __name__ == '__main__':
    unittest.main()
