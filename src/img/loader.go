package img

import (
    "fmt"
    "image"
    "image/jpeg"
    "os"
)

type RGB_Pixel struct {
    R int
    G int
    B int
}

type RGB_Image [][]RGB_Pixel

func GetPixels(path string) (RGB_Image) {
    image.RegisterFormat("jpeg", "jpeg", jpeg.Decode, jpeg.DecodeConfig)

    file, err := os.Open(path)
    if err != nil {
        fmt.Println("Error: File could not be opened")
        os.Exit(1)
    }
    defer file.Close()

    img, _, err := image.Decode(file)
    if err != nil {
        fmt.Println("Error: Image could not be decoded")
        os.Exit(1)
    }

    bounds := img.Bounds()
    width, height := bounds.Max.X, bounds.Max.Y

    var pixels RGB_Image
    for y := 0; y < height; y++ {
        var row []RGB_Pixel
        for x := 0; x < width; x++ {
            row = append(row, rgbaToRgb(img.At(x, y).RGBA()))
        }
        pixels = append(pixels, row)
    }

    return pixels
}

func rgbaToRgb(r uint32, g uint32, b uint32, a uint32) RGB_Pixel {
    return RGB_Pixel {
        int(r / 257),
        int(g / 257),
        int(b / 257),
    }
}
