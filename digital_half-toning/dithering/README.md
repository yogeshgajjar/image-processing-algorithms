## Dithering

This method focuses on using either fixed/constant thresholding , random thresholding and dithering matrix for transforming the grayscale image into binary image consisting of either black or white pixels. The fixed/constant thresholding methods to binarize the image checks for a threshold value. If the pixel value of the original input image is less than the fixed/constant threshold, the output image is a black dot. In case of random thresholding,
the original image pixel when found less than a random value of threshold generated using rand() function, gives a black dot. In case of dithering matrix, a Bayer matrix consisting of Index matrix is convolved over the original image and the output value generated using the
process. The Bayer matrix are generated recursively and vary in size. Common examples are 4x4, 8x8, 16x6, 32x32 etc.

## Fixed Thresholding

This thresholding utilizes a 127-pixel intensity as the threshold to compare and transform
the original image pixel intensities into binary pixel intensities. Each pixel value is
compared with the value 127 and the output image (binarized) is assigned a black dot if
the pixel value is less than the threshold and a white dot if the pixel value is greater than
the threshold. The equation for this thresholding can be given as :

## Random Thresholding

This thresholding incorporates randomness in the pixel intensities by selecting the
threshold randomly. It tries to break the monotones in the result from the fixed
thresholding. The threshold is selected from a distribution and is used to compare with
the original image intensity values. If greater that we map it to 255 otherwise we map it
to zero.

## Dithering Matrix

To avoid exposed or noisy image outputs obtained from the fixed and random
thresholding, the dithering matrix is a subtle way to binarize the image. This is also
called as ordered dithering which works by moving a Bayer matrix of given size over to
the image. The original pixel intensities are compared with the pixel intensities in the
threshold matrix which is essentially generated from the Bayer Filter matrix.
The index matrix is a 2x2 size which calculates the likelihood of a pixel to be turned on.
Depending upon the values, certain pixels are turned on and off when this filter is
convolved over the original input image.

For reference [ordered dithering](https://en.wikipedia.org/wiki/Ordered_dithering)

## Output 

#### Original Image:

![Original](images/test_image.png) 

#### Fixed Thresholding Output: 

![Sobel](images/fixed.png) 

#### Random Thresholding Output: 

![Canny](images/random.png) 

#### I2 Dithering matrix output 

![I2](images/i2.png)

#### I8 Dithering matrix output 

![I8](images/i8.png)
