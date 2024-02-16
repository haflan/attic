package main

import (
	"bufio"
	"fmt"
	"github.com/edwvee/exiffix"
	"golang.org/x/image/draw"
	"image"
	"image/jpeg"
	"os"
	"time"
)

// scale image. Thanks: https://github.com/nfnt/resize/issues/63#issuecomment-540704731
func scaleImg(src image.Image, rect image.Rectangle, scale draw.Scaler) image.Image {
	dst := image.NewRGBA(rect)
	scale.Scale(dst, rect, src, src.Bounds(), draw.Over, nil)
	return dst
}

func thumbnail(src image.Image, maxW, maxH int, scale draw.Scaler) image.Image {
	var (
		scalar float64 = 1
		w              = float64(src.Bounds().Max.X)
		h              = float64(src.Bounds().Max.Y)
	)
	if w > float64(maxW) {
		scalar = float64(maxW) / w
	}
	if scalar*h > float64(maxH) {
		scalar = float64(maxH) / h
	}
	return scaleImg(src, image.Rect(0, 0, int(scalar*w), int(scalar*h)), draw.ApproxBiLinear)
}

func main() {
	start := time.Now()
	if len(os.Args) < 2 {
		fmt.Println("No img file given")
		os.Exit(1)
	}
	imgFile, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println("Error when reading:", err)
		os.Exit(1)
	}
	defer imgFile.Close()
	fmt.Println("Opened", time.Since(start))
	start = time.Now()
	img, _, err := exiffix.Decode(imgFile)
	if err != nil {
		fmt.Println("Error when decoding:", err)
		os.Exit(1)
	}
	fmt.Println("Decoded", time.Since(start))
	start = time.Now()
	of, err := os.Create("output.jpg")
	if err != nil {
		fmt.Println("Error when opening output file:", err)
		os.Exit(1)
	}
	newImg := thumbnail(img, 200, 200, draw.ApproxBiLinear)
	fmt.Println("Processed", time.Since(start))
	start = time.Now()
	imageWriter := bufio.NewWriter(of)
	err = jpeg.Encode(imageWriter, newImg, nil)
	fmt.Println("Written", time.Since(start))
}
