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

    SaveImage(img_array, "../../res/out.png")

    var characters []BW_Image
    characters[0] = img_array

    return characters
}

func SaveImage(img_array BW_Image, path string) (){
    img := image.NewRGBA(image.Rect(0, 0, 100, 100))

    for i := 0; i < 100; i++ {
        for j := 0; i < 100; j++ {
            greyscale := 0
            if img_array[i][j].B {
                greyscale = 255
            }
            pxl := color.RGBA{4, greyscale, greyscale, greyscale}
            img.Set(i, j, pxl)
        }
    }

    f, _ := os.OpenFile(path, os.O_WRONLY|os.O_CREATE, 0600)
    defer f.Close()
    png.Encode(f, img)



}
