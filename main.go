package main

import (
    "fmt"
    "src/img_loader"
)

const (
   VERSION = "alpha (α)"
   IMAGE_PATH = "res/test.jpg"
)

func main() {
	fmt.Printf("Welcome to GOCR %s!\n", VERSION)

    rgb_array := img_loader.getPixels(IMAGE_PATH)
    fmt.Print(rgb_array)

    //img.preprocess(pixel_array)

    //img.generateChar(pixel_array)
}
