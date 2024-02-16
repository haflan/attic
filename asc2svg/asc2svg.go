package main

import (
        "fmt"
        "strings"
        "io/ioutil"
)

const (
        symJoint = "+"
        symLineV = "|"
        symLineH = "-"
)


/* Drawable interface and shape structs that implement it */
type drawable interface {
        draw()
}
type canvas struct {
        x0, y0, x1, y1 int
        array [][]rune
        processed [][]bool
        shapes []drawable
}
type block struct {
        x0, y0, x1, y1 uint
}
type line struct {
        x0, y0, x1, y1 uint
}

/* Draw functions for each drawable element */
func (b block) draw() {
        fmt.Printf("Block covering (%v, %v) to (%v, %v)", b.x0, b.y0, b.x1, b.y1)
}
func (l line) draw() {
        fmt.Printf("Line covering (%v, %v) to (%v, %v)", l.x0, l.y0, l.x1, l.y1)
}

/* Initialization ("constructor") */
func NewCanvas(raw_ascii string) *canvas {
        lines := strings.Split(raw_ascii, "\n")
        // 1. Find width and height, and create 2D slice / "array" (rename later)
        height := len(lines)
        width := 0
        // TODO: Optimize later. Or just don't. 
        for i := 0; i < height; i++ {
                if len(lines[i]) > width {
                        width = len(lines[i])
                }
        }
        c := canvas{0, 0, width , height, nil, nil, nil}
        // 2. Make slices to represent the ascii canvas
        c.array = make([][]rune, height)
        c.processed = make([][]bool, height)
        for y := 0; y < height; y++ {
                c.array[y] = make([]rune, width)
                c.processed[y] = make([]bool, width)
                for x, char := range lines[y] {
                        c.array[y][x] = rune(char)
                }
        }
        return &c
}
/* Analysis / detection functions */
func (c *canvas) analyze() {
        for y := 0; y < len(c.array); y++ {
                for x, char := range c.array[y] {
                        if c.processed[y][x] {
                                continue
                        }
                        switch string(char) {
                                case symJoint:
                                        c.detect_block(x, y)
                                        //c.detect_line(x, y)
                                case symLineV:
                                default:
                        }
                }
        }
}
func (c *canvas) detect_block(x, y int) {
        fmt.Printf("block detection at (%v, %v)\n", x, y)
        /* Nothing to see here! Just delete :]
        c.shapes = append(c.shapes, block{1, 2, 3, 4})
        c.shapes = append(c.shapes, block{5, 6, 7, 8})
        */
}
/* ^ NOTE: Using a canvas *pointer* instead of passing by value makes so that 
 * the contents are persisted (just like in C)
 */


/* File management */
func check(e error) {
        if e != nil {
                panic(e)
        }
}

func main() {
        figfile, err := ioutil.ReadFile("./fig.note")
        check(err)
        figascii := string(figfile)
        main_canvas := NewCanvas(figascii)
        main_canvas.analyze()
        for i := 0; i < len(main_canvas.shapes); i++ {
                main_canvas.shapes[i].draw()
                fmt.Println()
        }
}
