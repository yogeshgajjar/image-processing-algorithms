## Edge Detectors 

Edge detection is a very important as well as widely used technique to find the edges or
boundaries of object within images. It is an algorithm which essentially finds the region in an
image where there is a sharp change in intensities or sharp change in color. A high value or
dark edge indicates steep change and a low change or light edge indicates a shallow change. 

### Sobel Edge Detector 

Sobel Edge detection is one of the very primitive work on the field of edge detections. The
Sobel operator, sometimes called the Sobel-Feldman operator or Sobel filter is a used in
Digital Image Processing and Computer vision applications where it creates an image
emphasizing edges. It works on the principle of first order derivative of the pixel also called as
gradient of an image. Sobel operator is a 3x3 kernel for each of the two directions of an image.
The sobel matrix also called as gradient matrix for x direction consists of minus numbers on
the left-hand side and positive numbers on the right-hand side while preserving the data in
the center pixel. Similarly, the gradient for y-direction has minus numbers on the bottom and
positive numbers on top and here we are preserving a little bit on the middle row pixels.

### Canny Edge Detector 

Canny edge detector is an update to the Sobel Edge detector in terms of processing the
image to improve the quality of the edges in the image. Canny edge detection uses a multistage algorithm to detect a wide range of edge in the image. It is a technique that essentially
extracts the useful structural information from different images and reduces the amount of
data to be processed. It uses the calculus of variations – a technique that finds the function
which optimizes a given functional. The optimal function in Canny’s detector is described
by the sum of four exponential terms, but it can be approximated by the first derivative of
a Gaussian.

## Build Steps 

### Dependencies 

This program depends on the following software/libraries 

- [OpenCV](https://opencv.org/) (computer vision library)
  - Can be installed from [source](https://cv-tricks.com/installation/opencv-4-1-ubuntu18-04/) 

### Build Steps 

To build the program, follow the steps. 

```
git clone git@github.com:yogeshgajjar/image-processing-algorithms.git 
cd edge\ detector/
make 
```

This will build the executable and to run the executable, follow the steps 
- For Sobel edge detection, ``` ./output_sobel test_images/<name of image.raw> gradientX.raw gradientY.raw gradient_map.raw normalized_final.raw <height_image> <width_image> <no_of_channel> <threshold_value> ```

