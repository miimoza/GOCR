package img

import (
    "fmt"
)

const (
   R_LUMINANCE = 0.2126
   G_LUMINANCE = 0.7152
   B_LUMINANCE = 0.0722
)

type BW_Pixel struct {
    B bool
}

type GS_Pixel struct {
    V int
}

type BW_Image [][]BW_Pixel

type GS_Image [][]GS_Pixel

func Binarize(rgb_image RGB_Image) (BW_Image) {

    var histogram [256]int;

    var gs_image GS_Image
    for _, s := range rgb_image {
        var row []GS_Pixel
        for _, s2 := range s {
            var luminance int = int(float64(s2.R) * R_LUMINANCE + float64(s2.G) * G_LUMINANCE + float64(s2.B) * B_LUMINANCE)
            row = append(row, GS_Pixel {luminance})
            histogram[luminance]++
        }
        gs_image = append(gs_image, row)
    }

    i_m := 127
    fmt.Println("Binarize histogram etc.")

    i_s := 0
    i_e := 255
    i_m = int((i_s + i_e) / 2.0) // center of the weighing scale I_m
    w_l := get_weight(i_s, i_m + 1, histogram) // weight on the left W_l
    w_r := get_weight(i_m + 1, i_e + 1, histogram) // weight on the right W_r
    for (i_s <= i_e) {
        if (w_r > w_l) { // right side is heavier
            w_r -= histogram[i_e]
            i_e--
            if (((i_s + i_e) / 2) < i_m) {
                w_r += histogram[i_m]
                w_l -= histogram[i_m]
                i_m--
            }
        } else if (w_l >= w_r) { // left side is heavier
            w_l -= histogram[i_s]
            i_s++
            if (((i_s + i_e) / 2) >= i_m) {
                w_l += histogram[i_m + 1]
                w_r -= histogram[i_m + 1]
                i_m++
            }
        }
        fmt.Println(i_s, i_e, ",", i_m)
    }

    i_m = (i_m + 127 * 7) / 8

    var bw_image BW_Image
    for i, s := range rgb_image {
        var row []BW_Pixel
        for j, _ := range s {
            var luminance int = gs_image[i][j].V
            B := false
            if luminance > i_m {
                B = true
            }
            row = append(row, BW_Pixel {B})
        }
        bw_image = append(bw_image, row)
    }

    return bw_image
}

func get_weight(start int, end int, histogram [256]int) (int) {
    var weight int = 0
    for i := start; i < end; i++ {
        weight += histogram[i]
    }
    return weight
}
