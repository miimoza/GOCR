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

    SaveImage(img_array, "./out.png")

    var characters []BW_Image
    characters = append(characters, img_array)




    return characters
}

func SaveImage(img_array BW_Image, path string) (){
    img := image.NewRGBA(image.Rect(0, 0, len(img_array[0]), len(img_array)))

    for i, row := range img_array {
        for j, pixel := range row {
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
