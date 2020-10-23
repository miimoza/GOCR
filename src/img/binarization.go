package img

const (
   R_LUMINANCE = 0.2126
   G_LUMINANCE = 0.7152
   B_LUMINANCE = 0.0722
   THRESHOLD = 0.5
)

type BW_Pixel struct {
    B bool
}

type BW_Image [][]BW_Pixel

func Binarize(rgb_image RGB_Image) (BW_Image) {

    var bw_image BW_Image
    for _, s := range rgb_image {
        var row []BW_Pixel
        for _, s2 := range s {
            var luminance float64 = float64(s2.R) * R_LUMINANCE + float64(s2.G) * G_LUMINANCE + float64(s2.B) * B_LUMINANCE
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
