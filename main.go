package main

import (
    "fmt"
    "img"
)

const (
   VERSION = "alpha (α)"
   IMAGE_PATH = "res/test.jpg"
)

func main() {
	fmt.Printf("Welcome to GOCR %s!\n", VERSION)

    rgb_array := img.GetPixels(IMAGE_PATH)
    fmt.Println("[IMG] loaded ;)")

    _ = img.Binarize(rgb_array)
    fmt.Println("Binarized")

    //fmt.Print(rgb_array)

    //img.preprocess(pixel_array)

    //img.generateChar(pixel_array)
}
