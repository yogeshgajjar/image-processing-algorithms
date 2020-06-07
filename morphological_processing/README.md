## Morphological Processing 

Another different method of image processing is the way we modify the image based on the shape and size where objects can be utilized for the object recognition to detect and identify objects in order to modify them. ‘Morph’ essentially means shape and so is why shape of an object is critical while performing these operations. Morphological processing is performed on binary images as binary images can be easily handled and contains imperfections in form of
texture and noise. Morphological image processing is a collection of non-linear operations related to the shape or morphology of features in an image. According to Wikipedia, morphological operations heavily depend on the relative ordering of pixel values, and not on their numerical values, and therefore are especially suited to the processing of binary images. 

The three different use of morphological process is covered in this repository are: 

- Basic morphological process 
    - Skeletonization
    - Thinning
    - Shrinking 

- Counting objects 
- PCB analysis 
- Defect detection


For more details look into the individual folder. 

## Steps to build 

Follow the steps to build morphological processing source code. Please note, the steps here will build all the different type of algorithms mentioned in this directory.


```
cd image-processing-algorithms/morphological_processing
make 
sh execute.sh 
```

The output images will be in **.raw** format. Please use [this](http://rawpixels.net/) to visualize the output. 