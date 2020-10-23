package img

import
(
    "fmt"
    "image"
)

func getCharacters(img BW_Image) (BW_Image[]) {
    fmt.Println("Start Segmentation")





}

func saveImage(img BW_Image, path string) (){
    img := image.NewRGBA(image.Rect(0, 0, 100, 100))

    for i := 0; i < 100; i++ {
		img.Set(i, i, color.RGBA{i, i, i, i})
	}

    f, _ := os.OpenFile("../../res/out.png", os.O_WRONLY|os.O_CREATE, 0600)
    defer f.Close()
    img.Encode(f, img)
}
