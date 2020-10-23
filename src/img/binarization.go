package img

const (
   R_LUMINANCE = 0.2126
   G_LUMINANCE = 0.7152
   B_LUMINANCE = 0.0722
   THRESHOLD = 127
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



    var bw_image BW_Image
    for i, s := range rgb_image {
        var row []BW_Pixel
        for j, _ := range s {
            var luminance int = gs_image[i][j].V
            B := false
            if luminance > THRESHOLD {
                B = true
            }
            row = append(row, BW_Pixel {B})
        }
        bw_image = append(bw_image, row)
    }

    return bw_image
}
