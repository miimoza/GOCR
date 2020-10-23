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

    var characters []BW_Image
    characters = append(characters, img_array)


    lines := getLines(img_array)
    for _, line := range  lines {

        column := 0
        for column < len(line[0]) {
            firstline := isLineWhite(getColumn(line, column))

            var character BW_Image

            for column < len(line[0]) && !xor(firstline, isLineWhite(getColumn(line, column))) {
                if !firstline {
                    character = append(character, getColumn(line, column))
                }
                column++
            }

            if !firstline {
                characters = append(characters, redressChar(character))
            }
        }
    }

    return characters
}

func getColumn(line BW_Image, column int) ([] BW_Pixel) {
    var column_array []BW_Pixel

    for i := 0; i < len(line); i++ {
        column_array = append(column_array, line[i][column])
    }

    return column_array
}

func redressChar(character BW_Image) (BW_Image){
    new_character := make([][]BW_Pixel, len(character[0]))
    for i := range new_character {
        new_character[i] = make([]BW_Pixel, len(character))
    }

    for i, l := range character {
        for j, pxl := range l {
            new_character[j][i] = pxl
        }
    }

    return new_character
}

func xor(a bool, b bool) (bool){
    return (a || b) && !(a && b)
}

func getLines(img_array BW_Image) ([]BW_Image) {
    var lines []BW_Image

    row := 0
    for row < len(img_array) {
        firstline := isLineWhite(img_array[row])

        var line BW_Image

        for row < len(img_array) && !xor(firstline, isLineWhite(img_array[row])) {
            if !firstline {
                line = append(line, img_array[row])
            }
            row++
        }

        if !firstline {
            lines = append(lines, line)
        }
    }

    return lines
}

func isLineWhite(line[] BW_Pixel) (bool) {
    i := 0

    for i < len(line) && line[i].B {
        i++
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
