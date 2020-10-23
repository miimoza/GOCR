package img

import
(
    "fmt"
    "image"
    "image/color"
    "image/png"
    "os"
)

func GetCharacters(img_array BW_Image) ([]BW_Image) {
    fmt.Println("Start Segmentation")

    SaveImage(img_array, "./initial.png")

    var characters []BW_Image
    characters = append(characters, img_array)



    row := 0
    for row < len(img_array) {
        firstline := isLineWhite(img_array[row])
        for row < len(img_array) && firstline && isLineWhite(img_array[row]) {
            row++
        }

        for i := 0; row > 1 && i < len(img_array[0]); i++ {
            img_array[row][i].B = false
        }
        row++
    }

    SaveImage(img_array, "./out.png")

    return characters
}

func isLineWhite(line[] BW_Pixel) (bool) {
    i:= 0
    for ; i < len(line) && line[i].B; i++ {
    }

    return i == len(line)
}

func SaveImage(img_array BW_Image, path string) (){
    img := image.NewRGBA(image.Rect(0, 0, len(img_array[0]), len(img_array)))

    for j, row := range img_array {
        for i, pixel := range row {
            var greyscale uint8 = 0
            if pixel.B {
                greyscale = 255
            }

            pxl := color.RGBA{greyscale, greyscale, greyscale, 255}
            img.Set(i, j, pxl)
        }
    }

    f, _ := os.OpenFile(path, os.O_WRONLY|os.O_CREATE, 0600)
    defer f.Close()
    png.Encode(f, img)



}
