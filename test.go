package main

import (
    "fmt"
    "os"
    "image/draw"
    "image"
    "image/jpeg"
    "code.google.com/p/graphics-go/graphics"
)

func main() {
	img_path = "res/test.jpg"


    fImg1, _ := os.Open(img_path)
    defer fImg1.Close()
    img1, _, _ := image.Decode(fImg1)

    m := image.NewRGBA(image.Rect(0, 0, 800, 600))
    draw.Draw(m, m.Bounds(), img1, image.Point{0,0}, draw.Src)
    //draw.Draw(m, m.Bounds(), img2, image.Point{-200,-200}, draw.Src)
    graphics.Rotate(m, img2, &graphics.RotateOptions{3.5})

    toimg, _ := os.Create("new.jpg")
    defer toimg.Close()

    jpeg.Encode(toimg, m, &jpeg.Options{jpeg.DefaultQuality})
}
