package img

import (
    "fmt"
)

type BW_Pixel struct {
    hue bool
}

type BW_Image [][]BW_Pixel

func Binarize(rgb_image RGB_Image) (BW_Image) {

    var bw_image BW_Image;

    for _, s := range rgb_image {
        for _, s2 := range s {
            fmt.Println(s2)
        }
    }

    return bw_image
}
