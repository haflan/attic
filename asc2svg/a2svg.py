#!/bin/python

font_size = 12
x_scaling = 0.5


class Figure:
    def __init__(self, ascii_text):
        self._elements = []
        self._ascii_text = [line.strip('\n') for line in ascii_text]
        self._height = len(ascii_text)
        self._width = max([len(line) for line in ascii_text])

        # NEW KNOWLEDGE: Using [[' ']*width]*height just makes `height` number of *copies* (!) of the inner list
        # In other words, every change you make to one of the lists will be applied to *all* of them.. :(
        # Using append instead, to avoid this

        # Make a '2d list' to keep track of processed characters
        self._processed = [[False]*self._width]
        for y in range(self._height - 1):
            self._processed.append([False]*self._width)

        # Make a '2d list' of the asci figure
        self._figarray = [[' ']*self._width]
        for y in range(self._height - 1):
            self._figarray.append([' ']*self._width)

        for y, line in enumerate(self._ascii_text):
            for x, cell in enumerate(line):
                self._figarray[y][x] = cell

    def process_all(self):
        # Process rectangles first
        for y in range(self._height):
            for x in range(self._width):
                if not self._processed[y][x]:
                    rects = self.rectangle(y, x)
                    if rects:
                        self._elements += rects

        # then process line. (VERY TEMPORARY SOLUTION)
        for y in range(self._height):
            for x in range(self._width):
                if not self._processed[y][x]:
                    self.process_char(y, x)

    def get_elements(self):
        return self._elements

    def get_array(self):
        return self._figarray

    def process_char(self, y, x):
        new_elements = None
        symbol = self._figarray[y][x]
        if symbol == '+':
            rects = self.rectangle(y, x)
            if rects:
                # TODO: Refactor this text extraction
                for r in rects:
                    r.extract_text(self)
                new_elements = rects
            # If not a rectangle, it must be a line joint
            else:
                new_elements = self.polyline(y, x, "FIND")
        elif symbol == '^':
            new_elements = self.polyline(y, x, "DOWN")
            new_elements.append(Arrow('^', (y, x)))
        elif symbol == '<':
            new_elements = self.polyline(y, x, "RIGHT")
            new_elements.append(Arrow('<', (y, x)))

        if new_elements:
            self._elements += new_elements
        self._processed[y][x] = True

    def __str__(self):
        figure_string = ""
        for row in self._figarray:
            for cell in row:
                figure_string += cell
            figure_string += '\n'
        return figure_string

    def __has_char(self, y, x, char):
        if y < 0 or y >= self._height:
            return False
        if x < 0 or x >= self._width:
            return False
        # Processed characters should not be considered
        if self._processed[y][x]:
            return False
        if self._figarray[y][x] == char:
            return True
        return False

    def __joint_in(self, y, x):
        return self.__has_char(y, x, '+')

    def __arrow_in(self, y, x):
        if self.__has_char(y, x, '<'):
            return True
        if self.__has_char(y, x, '>'):
            return True
        if self.__has_char(y, x, '^'):
            return True
        if self.__has_char(y, x, 'v'):
            return True
        return False

    """
    Finds all lines in a polyline recursively
    MAYBE: The best option is to just use SVG 'Line'. Polylines do not handle
    crossing lines well, I think.  (So TODO : refactor name)
    """
    # TODO: Refactor this mess entirely
    def polyline(self, y, x, direction):
        # TODO : Deal with arrows. Only one arrow is allowed for each direction, so this shouldn't be too hard
        # Find lines
        line_list = []
        # TODO: No reason for this horrible recursive mess! Just make a function
        # for all the directions.
        if direction == "FIND":
            if self.__has_char(y+1, x, '|'):
                line_list += self.polyline(y, x, "DOWN")
            if self.__has_char(y-1, x, '|'):
                line_list += self.polyline(y, x, "UP")
            if self.__has_char(y, x-1, '-'):
                line_list += self.polyline(y, x, "LEFT")
            if self.__has_char(y, x+1, '-'):
                line_list += self.polyline(y, x, "RIGHT")
            return line_list

        elif direction == "LEFT":
            x_test = x - 1
            while self.__has_char(y, x_test, '-'):
                self._processed[y][x_test] = True
                x_test -= 1
            if self.__arrow_in(y, x_test):
                line_list.append(Line((y, x_test - 1), (y, x)))
                line_list.append(Arrow('<', (y, x_test)))
                self._processed[y][x_test]
            elif self.__joint_in(y, x_test):
                line_list.append(Line((y, x_test), (y, x)))
                self._processed[y][x_test]
            else:
                line_list.append(Line((y, x_test + 1), (y, x)))
            x = x_test
        elif direction == "RIGHT":
            x_test = x
            if self.__has_char(y, x_test, '<'):
                self._processed[y][x_test] = True
                x -= 1
                x_test += 1
            x_test += 1
            while self.__has_char(y, x_test, '-'):
                self._processed[y][x_test] = True
                x_test += 1
            if self.__arrow_in(y, x_test):
                line_list.append(Line((y, x), (y, x_test + 1)))
                line_list.append(Arrow('>', (y, x_test)))
            elif self.__joint_in(y, x_test):
                line_list.append(Line((y, x), (y, x_test)))
            else:
                line_list.append(Line((y, x), (y, x_test - 1)))
            x = x_test

        # WORKING MESS
        elif direction == "DOWN":
            y_test = y
            if self.__has_char(y_test, x, '^'):
                self._processed[y_test][x] = True
                y -= 1
                y_test = y + 1
            y_test += 1
            while self.__has_char(y_test, x, '|'):
                self._processed[y_test][x] = True
                y_test += 1
            if self.__arrow_in(y_test, x):
                line_list.append(Line((y, x), (y_test+1, x)))
                line_list.append(Arrow('v', (y_test, x)))
            elif self.__joint_in(y_test, x):
                line_list.append(Line((y, x), (y_test, x)))
            else:
                line_list.append(Line((y, x), (y_test-1, x)))
            y = y_test
        else:
            pass

        # If the line we're testing has a '+' char, find new lines recursively
        if self.__has_char(y, x, '+'):                     # This should deffo be moved to the bottom
            line_list += self.polyline(y, x, "FIND")

        return line_list



    """
    Checks if a joint belongs to a rectangle (and if there are subrectangles)
    If it does, the rectangles are added as an element, and all associated characters
    are registered as processed, *except* joints on the rectangles.
    """
    def rectangle(self, y_start, x_start):
        # Temporary list of processed chars. Must dismiss if this is not a rectangle
        processed = [[y_start, x_start]]
        process_later = []
        if x_start + 1 >= self._width:
            return None

        # TODO: Refactor this into helper method 'find_y_start'?
        # 1. Find the vertical stop of the rectangle, if any
        y = y_start + 1
        y_stop = None
        while not y_stop and y < self._height:
            cell = self._figarray[y][x_start]
            if cell == '|':
                processed.append([y, x_start])
                y += 1
            elif cell == '+':
                # If '-' to the right, this should be the y_stop
                if self._figarray[y][x_start+1] == '-':
                    y_stop = y
                    processed.append([y, x_start])
                    break
                # If not, the '+' might be a connection to a line on the left
                process_later.append([y, x_start])
                y += 1
            else:
                break

        if not y_stop:
            return None

        # TODO: Clean up
        # 2. Find the horizontal stop, if any
        x_stop = None
        x = x_start + 1
        while not x_stop and x < self._width:
            upper_cell = self._figarray[y_start][x]
            lower_cell = self._figarray[y_stop][x]
            if upper_cell == '+' and lower_cell == '+':
                x_stop = x
                # Check that there's a line from upper cell to lower
                for y_test in range(y_start+1, y_stop):
                    cell = self._figarray[y_test][x]
                    if cell == '+':
                        process_later.append([y_test, x])
                    elif cell != '|':
                        x_stop = None
                        break
                if x_stop:
                    break
                # If x_stop is *not* defined, we probably found an x where both 'y's are connected to lines,
                # so we need to continue and add them
            if upper_cell != '-' and upper_cell != '+':
                break
            if lower_cell != '-' and lower_cell != '+':
                break
            if upper_cell == '+':
                process_later.append([y_start, x])
            if lower_cell == '+':
                process_later.append([y_stop, x])
            x += 1

        if not x_stop:
            return None

        # By this point, we know we have a rectangle, so we can register the
        # entire section as processed (except the bottom line, in case of
        # sub-rectangles)
        # TODO : Make Pythonic. (That goes for a lot of this code, though)
        for y in range(y_start, y_stop):
            for x in range(x_start, x_stop+1):
                if [y, x] not in process_later:
                    self._processed[y][x] = True

        new_rect = Rectangle((x_start, y_start), (x_stop, y_stop))
        new_rect.extract_text(self)
        rect_list = [new_rect]
        # Check if we're dealing with a rectangle chain
        sub_rects = self.rectangle(y_stop, x_start)
        if sub_rects:
            rect_list += sub_rects
        # If not, we can register the bottom line as processed
        else:
            for x in range(x_start, x_stop+1):
                if [y_stop, x] not in process_later:
                    self._processed[y_stop][x] = True

        return rect_list

    """Return SVG text of the entire figure"""
    def get_svg(self):
        svg_text = ""
        for el in self._elements:
            svg_text += el.draw() + "\n"
        return svg_text


class Rectangle:
    def __init__(self, pos_start, pos_stop):
        (self._x_start, self._y_start) = pos_start
        (self._x_stop, self._y_stop) = pos_stop
        self._text_lines = [None]

    def extract_text(self, fig):
        self._text_lines = []
        figarray = fig.get_array()
        for y in range(self._y_start+1, self._y_stop):
            line = ''.join(figarray[y][self._x_start+1:self._x_stop])
            self._text_lines.append(line)

    def __str__(self):
        return_string = "Rect, ({}, {}) to ({}, {}):".format(
                     self._y_start,
                     self._x_start,
                     self._y_stop,
                     self._x_stop,
                     )
        for line in self._text_lines:
            return_string += line + "\n"

        return return_string

    def draw(self):
        svg_text = "\t<rect x=\"{}\" y=\"{}\" ".format(
                x_scaling * self._x_start * font_size,
                self._y_start * font_size)
        rect_width = (self._x_stop - self._x_start) * font_size
        rect_height = (self._y_stop - self._y_start) * font_size
        svg_text += "width=\"{}\" height=\"{}\" ".format(
                x_scaling * rect_width,
                rect_height)
        svg_text += "fill=\"white\" stroke=\"black\"></rect>\n"

        for ln, line in enumerate(self._text_lines):
            svg_text += "\t<text x=\"{}\" y=\"{}\" font-family=\"Arial\" font-size=\"{}\" fill=\"black\">{}</text>\n".format(
                        x_scaling * self._x_start*font_size+15,
                        (self._y_start+ln)*font_size+15,
                        font_size,
                        line
                        )

        return svg_text


class Text:
    pass


# Not sure if Joint is useful in this code
class Joint:
    def __init__(self, position):
        # (pos should be a tuple)
        (self._x, self._y) = position
        pass

    def get_position(self):
        return (self._x, self._y)


# ~~The best alternative here is to use a *polyline*~~ nah
#   https://www.w3schools.com/graphics/svg_polyline.asp
class Line:
    def __init__(self, pos_start, pos_stop):
        (self._y_start, self._x_start) = pos_start
        (self._y_stop, self._x_stop) = pos_stop

    def draw(self):
        svg_text = "\t<line x1=\"{}\" y1=\"{}\" x2=\"{}\" y2=\"{}\" ".format(
            x_scaling * self._x_start * font_size,
            self._y_start * font_size,
            x_scaling * self._x_stop * font_size,
            self._y_stop * font_size)
        svg_text += "style=\"stroke:rgb(0,0,0);stroke-width:1\" />"
        return svg_text

    def __str__(self):
        text = "Line, ({}, {}) to ({}, {})\n".format(
            self._y_start,
            self._x_start,
            self._y_stop,
            self._x_stop
        )
        return text


# TODO : Replace all of the string representations and comparison with 'enums'
class Arrow():
    def __init__(self, symbol, position):
        self._y, self._x = position
        self._direction = symbol
        self._dir_func_map = {
                'v': self.__gen_coords_down,
                '^': self.__gen_coords_up,
                '<': self.__gen_coords_left,
                '>': self.__gen_coords_right,
            }

    # TODO : These *can* be replaced by class methods that take x and y as
    # arguments instead? Not sure if one is better than the other...
    # More important TODO : Use some brain power to make this prettier (by
    # rotating arrows and setting an offset or something, instead of copy-pasting
    # functions and changing 'manually')
    def __gen_coords_down(self):
            return [
            x_scaling * self._x * font_size,
            (self._y+1) * font_size - 1,
            x_scaling * (self._x-1) * font_size,
            self._y * font_size,
            x_scaling * (self._x+1) * font_size,
            self._y * font_size ]

    def __gen_coords_up(self):
            return [
            x_scaling * self._x * font_size,
            (self._y-1) * font_size - 1,
            x_scaling * (self._x-1) * font_size,
            self._y * font_size,
            x_scaling * (self._x+1) * font_size,
            self._y * font_size ]

    def __gen_coords_left(self):
            return [
            x_scaling * self._x * font_size,
            (self._y+1) * font_size - 1,
            x_scaling * (self._x-1) * font_size,
            self._y * font_size,
            x_scaling * (self._x+1) * font_size,
            self._y * font_size ]

    def __gen_coords_right(self):
            return [
            x_scaling * self._x * font_size,
            (self._y+1) * font_size - 1,
            x_scaling * (self._x-1) * font_size,
            self._y * font_size,
            x_scaling * (self._x+1) * font_size,
            self._y * font_size ]

    def draw(self):
        try:
            svg_text = "\t<polygon points=\"{},{} {},{} {},{}\" ".format(
                    *self._dir_func_map[self._direction]())
            svg_text += "style=\"stroke:rgb(0,0,0);stroke-width:1\" />"
            return svg_text
        except Exception as e:
            print("Invalid arrow detected")
            print(e)
            return ""

    def __str__(self):
        text = "Arrow, {}, ({}, {})\n".format(
            self._direction,
            self._x,
            self._y,
        )
        return text

### TESTING ###

with open("ascii.txt") as f:
    ascii_text = f.readlines()
    f.close()

drawing = Figure(ascii_text)
print(drawing)
drawing.process_all()
for el in drawing.get_elements():
    print(el)

svg_text = "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox = \"0 0 500 500\">\n"
svg_text += drawing.get_svg()
svg_text += "</svg>"

with open("test.svg", "w+") as f:
    f.write(svg_text)
    f.close()
