#EE569 Homework Assignment #2
#Date: 16th February 2020
#Name: Yogesh Sanat Gajjar
#USCID: 5476153636
#Email: ygajjar@usc.edu

Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

------------------------------------------------------------------------
                Problem 1a. Sobel Edge Detector
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Converted the RGB image to Grayscale using the formula provided in the assignment. 
2.  Boundary extension on all the sides. Used mirror reflection method. 
3.  Initialized the sobel operators and performed convolution on the boundary extended image to find x-gradient map, y-gradient map and gradient magnitude map. 
4.  Performed normalization by calculating the minimum and maximum values of x-gradient map, y-gradient map and total image map and used a image normalization formula to calculate the 
normalized outputs.  
5.  Calculated CDF and to find the threshold value. Selected a pixel value corresponding to 85%, 90%, 95% and 99% of the CDF value. Compared it with normalized gradient map. If found 
greater than threshold, map it to 0, else mapped it to 1.  
6. 	Write the image onto a .raw file 

STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o program" 
3.  Run and execute the code and then write "./program input_image.raw gradient_x.raw gradient_y.raw gradient_map.raw normalized_image.raw BytePerPixel threshold" 
4.  Use the Image Viewing Software to see the results. 



------------------------------------------------------------------------
                Problem 1b. Canny Edge Detector
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the input image using imread and initialize variables as Mat datatype
2.  Convert the image into grayscale image using the function CvtColor. 
3.  Call the canny function to generate edges. Note: Canny function has Guassian blur function inbuilt. Also, provide lower and upper threshold values. 
4.  Write the image into a file. 

STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output `pkg-config --cflags --libs opencv`"
3.  Modify the directory in at line no 72 to the current directory. 
3.  Run and execute the code and then write "./program low_threshold high_threshold" 
4.  Use the Image Viewing Software to see the results. 


------------------------------------------------------------------------
                Problem 2a. Dithering 
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the inout image, height, width, BytePerPixel , index_value using command line 
2.  For fixed thresholding, consider a fixed threshold value eg. 127 and compare it with the input pixels. If found less than 127, map it to 0, else map it to 255. 
3.  For random thresholding, generate a random number using rand()%256. Compare the value of the random number in the range of 0 to 255 with the input image. If found greater than random value, 
map it to 255. Else map it to 0. 
4.  For dithering, generate Bayer matrix using the formalae provided in the lecture. Interate through the loop until count > 0. Recursively add elements in the Bayer filter matrix based on index provided. 
This will generate three matrix I2, I8, I32. 
5.  Compute the threshold matrix using the recursively generated bayer matrix and perform thresholding to generate an output file. 
5.  Write the individual output value onto a .raw file. 


STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw fixed_thresholding.raw random_thresholding.raw dithering_output.raw height width index(0 for I2, 1 for I8, 2 for I32)
4.  Use the Image Viewing Software to see the results.


------------------------------------------------------------------------
                Problem 2b. Error Diffusion
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the input image, height, width, BytePerPixel using command line. 
2.  Perform boundary extension using zero padding on the input image. 
3.  Calculate the error while performing FS error diffusion. This is done in nested loop. Convolve the FS matrix over the zero padded image and diffuse the error itself in nested loop to its neighbouring 
pixels. 
4.  Calculate the error while performing JJN error diffusion. This is done in nested loop. Convolve the JJN matrix over the zero padded image and diffuse the error itself in nested loop to its neighbouring 
pixels.
5.  Calculate the error while performing S error diffusion. This is done in nested loop. Convolve the S matrix over the zero padded image and diffused the error itself in nested loop to its neighbouring 
pixels.
6.  Write the output images onto .raw file 


STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw FS.raw JJN.raw S.raw height width
4.  Use the Image Viewing Software to see the results.


------------------------------------------------------------------------
    Problem 2c. Color Halftoning using Seperable Error Diffusion
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the inout image, height, width, BytePerPixel using command line 
2.  Call the function seperateolor() which seperates the individual RGB channel and converts into CMY space by subtracting 255. 
3.  Diffusion of error using the FS serpentine method on each individual channels. 
4.  Merge the individual CMY channels into one CMY image and then convert it into RGB space. 
5.  Write the output onto .raw file.  



STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width ByteperPixels
4.  Use the Image Viewing Software to see the results.

------------------------------------------------------------------------
    Problem 2c. Color Halftoning using MBVQ Method
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the inout image, height, width, BytePerPixel using command line 
2.  Calculate the MBVQ from a function, which returns the quadrant of the cube based on the pixel value of RGB 
3.  Based on the MBVQ, calculate the closest vertex based on the function provided. The function value returns a color which is closest to the original pixel value 
4.  After getting the color of the pixel value, determine the pixel value based on another function colorRepresentation(). 
5.  Use the FS diffusion method to diffuse the error onto the neighbouring pixels after calculating the error based on the difference between old pixel and new pixel.
6.  Write the output file onto .raw format 



STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width ByteperPixels
4.  Use the Image Viewing Software to see the results.
