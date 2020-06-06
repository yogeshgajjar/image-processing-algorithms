## Digital Half-Toning 

Half-toning is a technique that simulates continuous-toned image into a two-toned image. A gray-scale image's pixel intensity varies from 0 to 255, whereas a half-tone image's pixel intensity varies between two tones i.e. white & black. This method is essentially converting the image into two tones making the gray-scale image into either white or black. Half-toning was introduced to utilize the fact that human eye fails to recognize or differentiate two dots very close to each other. Thus, the high-density dots in the local cells can be rendered as a gray scale pixel. 

The importance of using half-toning lies in the fact that every gray-scale image contains 8 bits which makes it a 256-tone image and RGB on the other end has 24 bits which makes it a 16.7 million colors. But most printing devices are not able to reproduce different shadows of gray or so many colors. So, the original image is transformed into an image which contains only white(no-ink) and black(one dot of ink).

The three half-toning techniques covered in this repository are : 

- Dithering 
    - Fixed Thresholding 
    - Random Thresholding 
    - Dithering Matrix 

- Error Diffusion

- Color halftoning with error diffusion
    - Separable Error Diffusion
    - MBVQ-based Error Diffusion

For more details look into the individual folder. 

## Steps to build 

Follow the steps to build digital half-toning source code. Please note, the steps here will build all the different type of algorithms mentioned in this directory.


```
cd image-processing-algorithms/digital_half-toning
make 
sh execute.sh 
```

The output images will be in **.raw** format. Please use [this](http://rawpixels.net/) to visualize the output. 