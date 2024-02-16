Specification for 'v1.0'
========================

Valid symbols
-------------
- Joints and corners: `+`
- Lines:              `-`, `|`
- Arrows              `v`, `^`, `<`, `>`
- All symbols are allowed within a rectangle

Use
---
- Rectangles are defined by four corners connected by lines or joints
- Rectangles can have as many joints as fits, but these should be connected to
  lines
- Text is only supported inside rectangles
- Only text is supported inside rectangles 
  (everything within the rectangle boundaries is read as the rectangle label).
- Lines *must* start / end with a joint or an arrow
- Lines can have as many joints as it can fit 
- Arrows *must* point away from the line that they end

v2.0 (proposal)
===============

Valid symbols
-------------
- Dotted lines: `:` `.`
  These don't really mix well with the rest of the symbols, so they might not 
  be useful.

New features
------------
- Text should be allowed wherever in the figure. 
  It should simply be read from the first to the last letter.
- Rectangles can contain everything that is allowed in the figure itself. 
  (See issue about recursive Figures / rectangles or something ) \*

v3.0 (proposal)
---------------
- Diagonal lines `/`, `\`, for instance for diamond shapes...


Functionality
=============

- Symbols that can be found unprocessed: ` `, `+`, `<`, `^`
    + Nothing to do: ` `
    + Rectangle detection: `+`
    + Line detection: `+`, `<`, `^`
- Symbols that can [only] \* be found during line processing:
    + [`>`, `v`]
    + `+`, `|`, `-`

\* These are up for discussion. It might be desirable to be able to draw
  *through* / *over* boxes, in which case lines must be allowed to cross the  
  box borders. This will in turn make it hard to implement "recursive
  rectangles", as the rectangle contents are not necessarily isolated.


