package img

import (
    "fmt"
)

type BW_Pixel struct {
    hue bool
}

type BW_Image [][]BW_Pixel

func binarize(rgb_image RGB_Image) (BW_Image) {

    var bw_image : BW_Image;

    for i, s := range rgb_image {
        for j, s2 := range s {
            fmt.Println(j)
        }
    }

    return bw_image
}
