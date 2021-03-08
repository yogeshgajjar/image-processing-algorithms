## Geometric Image Modification 

We see that changing the pixel values in the image changes the overall representation of the image. The concepts like Demosaicing, Denoising, Dithering etc. utilizes the concept of changing the color value to get a different image representation. But, with geometric image modification, we modify the position of the pixel value in an image to give the final image a different perceptive keeping the pixel value as it is. Geometric transformations are handy in correcting geometric distortions introduced in the image acquisition process, adding visual effects and working with multiple images. The geometric transformation of images includes :

- Translation - moves an image up, down, left, or right
- Scaling - enlarges or shrinks an image
- Rotation - rotates an image about a given point
- Affine - includes translation, scaling, and rotation in one operation

All transformation operations are performed by taking pixel values from their original spatial coordinates to new coordinates in the final image. Every pixel in the source image is passed through this transformation, creating a geometrically transformed output pixel location. Each pixel of the source image is transformed, pixel by pixel, to its new location in the destination
image. 

The two techniques covered in this repository are : 

- Image Warping 
- Image Stitching

For more details look into the individual folder. 

## Steps to build 

Follow the steps to build geometric image modification source code. Please note, the steps here will build all the different type of algorithms mentioned in this directory.


```
cd image-processing-algorithms/geometric_image_modification
make 
sh execute.sh 
```

The output images will be in **.raw** format. Please use [this](http://rawpixels.net/) to visualize the output. 