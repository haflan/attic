# Components
Second attempt at a plan, at least for the component *structs*. A component
should be an interface with the function `draw()`. 

## Figure
- rectangle 
- anything containing other components inside it. Can contain other figures.
- variables:
  ```
  (x0, y0)
  (x1, y1)
  components[]
  ```
- `.draw()` for a figure simply calls `.draw()` for all compoments in the list.

## Box
- rectangle only containing text (label)
- variables:
  ```
  (x0, y0)
  (x1, y1)
  label
  ```

## Line
- a simple start position, stop position, and line type (dotted, thick? etc)
- variables:
  ```
  (x0, y0)
  (x1, y1)
  type  
  ```

## Arrow
- (x, y)
- direction 
  ```
  0     1       2       3
  UP    DOWN    LEFT    RIGHT
  ^     v       <       >
  ```
