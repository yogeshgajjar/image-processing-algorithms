## Texture Classification & Segmentation 

Image textures are an essential part if the image which carries useful information about the image and help us in differentiating different objects in the image. Texture analysis in Digital Image Processing is often referred to as the segmentation of the regions of the image by their texture content. Texture
analysis attempts to describe intuitive qualities of a textured image described by its roughness, smoothness, silkiness, or bumpiness as a function of the spatial variation in pixel intensities. In this sense, the roughness or bumpiness refers to variations in the gray levels. Texture analysis has numerous applications in the field of biomedical and remote sensing. It is used in various applications, such as remote sensing, aerial photography, automated inspection, and medical image processing. The other application of texture analysis can be used to find the
texture boundaries, called texture segmentation. 

Texture analysis can be advantageous in certain applications where objects in an image are characterized by their texture than by intensity, and traditional segmentation techniques cannot be used effectively. Textures are a quasi-periodic pattern where the texture is not smooth or contains random noise. Classification problem deals with finding the texture class given the texture whereas
segmentation deals with given the texture and finding the boundary.

The three applications of Texture Analysis are :
1. Texture Classification – Classifies the images into different classes
2. Texture Segmentation – Segments an image into different sections
3. Texture Synthesis – Generate new samples from the given existing class


We perform classification and segmentation of the textured images and then we
classify the images into different clusters using the algorithm called K-Means.

## Steps to build 

Follow the steps to build the source code. Please note, the steps here will build all the different type of algorithms mentioned in this directory.


```
cd image-processing-algorithms/texture_classification_&_segmentation
make 
sh execute.sh 
```

The output images will be in **.raw** format. Please use [this](http://rawpixels.net/) to visualize the output. 