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

    bw_array := img.Binarize(rgb_array)
    fmt.Println("Binarized")


    _ = img.GetCharacters(bw_array)



    //fmt.Print(rgb_array)

    //img.preprocess(pixel_array)

    //img.generateChar(pixel_array)
}
