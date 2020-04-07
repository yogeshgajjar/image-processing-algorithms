
/*
#EE569 Homework Assignment #2
#Date: 4th February 2020
#Name: Yogesh Sanat Gajjar
#USCID: 5476153636
#Email: ygajjar@usc.edu

Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

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

*/ 

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class ErrorDiffusion {
private:
    unsigned char **Image, **imageTemp, **imageTempBinarized, **ImagewithBoundary;
    int height, width, window, padding;
    long double error;

public:
    ErrorDiffusion(int, int, long double);
    unsigned char **boundaryExtension(int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix(int, int);
    unsigned char **errorFS();
    unsigned char **errorJJN();
    unsigned char **errorS();

    
};

ErrorDiffusion::ErrorDiffusion(int h, int w, long double e) {
    height = h;
    width = w;
    error = e;
}

void ErrorDiffusion::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void ErrorDiffusion::writeImage(string filename, unsigned char **final) {
    FILE *file;

    unsigned char finalImg[height*width];
    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            finalImg[count] = final[i][j];
            count++;
        }
    }

    if (!(file=fopen(filename.data(),"wb"))) {
		cout << "Cannot open file: " << filename.data() << endl;
		exit(1);
	}
	fwrite(finalImg, sizeof(unsigned char), height*width, file);
	fclose(file);

}

unsigned char **ErrorDiffusion::generateMatrix(int h, int w) {
    long int count{};
    unsigned char **image = new unsigned char *[h];
    for(int i=0; i < h; i++) {
        image[i] = new unsigned char [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }

    return image;
}

void ErrorDiffusion::generateImage(unsigned char *main_image) {
    long int count{};
    Image = new unsigned char *[height];
    for(int i = 0; i < height; i++) {
        Image[i] = new unsigned char [width];
        for(int j = 0; j < width; j++) {
            Image[i][j] = main_image[count];
            count++;
        }
    }
}

unsigned char **ErrorDiffusion::boundaryExtension(int pad) {
    ImagewithBoundary = generateMatrix((2*pad)+height, (2*pad)+width);

    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary[i+pad][j+pad] = Image[i][j];
            count++;
        }
    }
    return ImagewithBoundary;
}

unsigned char **ErrorDiffusion::errorFS() {
    window = 3;
    padding = window/2;

    float FS[3][3] = {{0,0,0}, 
                      {0,0,7.0},
                      {3.0,5.0,1.0}};
    
    imageTemp = boundaryExtension(padding);
    imageTempBinarized = generateMatrix(height, width);
    
    for (int i = padding; i < height+padding; i++ ) {
        if((i-padding)%2 == 0) {
            for (int j = padding; j < width+padding; j++) {
                if (imageTemp[i][j] < 128) {
                    error = imageTemp[i][j] - 0;
                    imageTempBinarized[i-padding][j-padding] = 0;
                }
                else {
                    error = imageTemp[i][j] - 255;
                    imageTempBinarized[i-padding][j-padding] = 255;
                }
                for (int k = 0; k < window; k++) {
                    for (int l = 0; l < window; l++) {
                        imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (FS[k][l]/16.0));
                    }
                }
            }
        }

        if((i-padding)%2 !=0) {
            for (int j = width; j > 0; j--) {
                if (imageTemp[i][j] < 128) {
                    error = imageTemp[i][j] - 0;
                    imageTempBinarized[i-padding][j-padding] = 0;
                }
                else {
                    error = imageTemp[i][j] - 255;
                    imageTempBinarized[i-padding][j-padding] = 255;
                }
                for (int k = 0; k < window; k++) {
                    for (int l = window-1; l >=0; l--) {
                        imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (FS[k][l]/16.0));
                    }
                }
            }
        }
        
    }

    return imageTempBinarized;
    
}

unsigned char **ErrorDiffusion::errorJJN() {
    window = 5;
    padding = window/2;

    float JJN[5][5] = {{0,0,0,0,0},
                       {0,0,0,0,0},
                       {0,0,0,7.0,5.0},
                       {3.0,5.0,7.0,5.0,3.0},
                       {1.0,3.0,5.0,3.0,1.0}};

    imageTemp = boundaryExtension(padding);
    imageTempBinarized = generateMatrix(height, width);

    for(int i=padding; i < height+padding; i++) {
        for(int j=padding; j < width+padding; j++) {
            if(imageTemp[i][j] > 127) {
                error = imageTemp[i][j] - 255;
                imageTempBinarized[i-padding][j-padding] = 255;
            }
            else {
                error = imageTemp[i][j] - 0;
                imageTempBinarized[i-padding][j-padding] = 0;
            }
            for(int k=0; k < window; k++) {
                for(int l=0; l < window; l++) {
                    imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (JJN[k][l]/48.0));
                }
            }
        }
    }
    return imageTempBinarized;
}

unsigned char **ErrorDiffusion::errorS() {
    window = 5;
    padding = window/2;

    float S[5][5] = {{0,0,0,0,0},
                       {0,0,0,0,0},
                       {0,0,0,8.0,4.0},
                       {2.0,4.0,8.0,4.0,2.0},
                       {1.0,2.0,4.0,2.0,1.0}};

    imageTemp = boundaryExtension(padding);
    imageTempBinarized = generateMatrix(height, width);

    for(int i=padding; i < height+padding; i++) {
        for(int j=padding; j < width+padding; j++) {
            if(imageTemp[i][j] > 127) {
                error = imageTemp[i][j] - 255;
                imageTempBinarized[i-padding][j-padding] = 255;
            }
            else {
                error = imageTemp[i][j] - 0;
                imageTempBinarized[i-padding][j-padding] = 0;
            }
            for(int k=0; k < window; k++) {
                for(int l=0; l < window; l++) {
                    imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (S[k][l]/42.0));
                }
            }
        }
    }
    return imageTempBinarized;
}

int main(int argc, char *argv[]) {
    int height{atoi(argv[5])}, width{atoi(argv[6])};
    string file_input{argv[1]}, fileOutputFS{argv[2]}, fileOutputJJN{argv[3]}, fileOutputS{argv[4]};
    ErrorDiffusion *error = new ErrorDiffusion{height, width, 0};
    unsigned char *ImageData = new unsigned char[height*width];
    error->readImage(file_input, ImageData);
    error->generateImage(ImageData);
    error->writeImage(fileOutputFS, error->errorFS());
    error->writeImage(fileOutputJJN, error->errorJJN());
    error->writeImage(fileOutputS, error->errorS());
    delete ImageData;
    delete error;
    return 0;
}