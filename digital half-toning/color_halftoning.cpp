
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
    Problem 2c. Color Halftoninig using Seperable Error Diffusion
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the inout image, height, width, BytePerPixel using command line 
2.  Call the function seperateolor() which seperates the individual RGB channel and converts into CMY space by subtracting 255. 
3.  Perform image extension and perform diffusion of error using the FS serpentine method on each individual channels. 
4.  Merge the individual CMY channels into one CMY image and then convert it into RGB space. 
5.  Write the output onto .raw file.  



STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width ByteperPixels
4.  Use the Image Viewing Software to see the results.

*/ 

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class ErrorDiffusion {
private:
    unsigned char ***Image, **imageTemp, **imageTempBinarized, **ImagewithBoundary, **imageCyan, **imageMagenta, **imageYellow, **R, **G, **B;
    int height, width, window, padding, BytesPerPixel;
    long double error;

public:
    ErrorDiffusion(int, int, long double, int, int);
    unsigned char **boundaryExtension(int, unsigned char **);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix(int, int);
    unsigned char **errorFS(unsigned char **);
    void separateColor();
    void mergeRGB();
    unsigned char ***mergeColors();

    
};

ErrorDiffusion::ErrorDiffusion(int h, int w, long double e, int b, int win) {
    height = h;
    width = w;
    error = e;
    BytesPerPixel = b;
    window = win;
    padding = win/2;
}

void ErrorDiffusion::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void ErrorDiffusion::writeImage(string filename, unsigned char ***final) {
    FILE *file;

    unsigned char finalImg[height*width*BytesPerPixel];
    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                finalImg[count] = final[i][j][k];
                count++;
            }
        }
    }

    if (!(file=fopen(filename.data(),"wb"))) {
		cout << "Cannot open file: " << filename.data() << endl;
		exit(1);
	}
	fwrite(finalImg, sizeof(unsigned char), height*width*BytesPerPixel, file);
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
    Image = new unsigned char **[height];
    for(int i = 0; i < height; i++) {
        Image[i] = new unsigned char *[width];
        for(int j = 0; j < width; j++) {
            Image[i][j] = new unsigned char [BytesPerPixel];
            for(int k=0; k < BytesPerPixel; k++) {
                Image[i][j][k] = main_image[count];
                count++;
            }
        }
    }
}

void ErrorDiffusion::separateColor() {
    imageCyan = generateMatrix(height, width);
    imageMagenta = generateMatrix(height, width);
    imageYellow = generateMatrix(height, width);

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                imageCyan[i][j] = 255 - Image[i][j][0];
                imageMagenta[i][j] = 255 - Image[i][j][1];
                imageYellow[i][j] = 255 - Image[i][j][2];
            }
        }
    }

}

unsigned char **ErrorDiffusion::boundaryExtension(int pad, unsigned char **image) {
    ImagewithBoundary = generateMatrix((2*pad)+height, (2*pad)+width);

    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary[i+pad][j+pad] = image[i][j];
            count++;
        }
    }
    return ImagewithBoundary;
}

unsigned char **ErrorDiffusion::errorFS(unsigned char **image) {
    float FS[3][3] = {{0,0,0}, 
                      {0,0,7.0},
                      {3.0,5.0,1.0}};
    
    imageTemp = boundaryExtension(padding, image);
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

void ErrorDiffusion::mergeRGB() {
    R = generateMatrix(height, width); 
    G = generateMatrix(height, width);
    B = generateMatrix(height, width);

    imageCyan = errorFS(imageCyan);
    imageMagenta = errorFS(imageMagenta);
    imageYellow = errorFS(imageYellow);

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            R[i][j] = 255 - imageCyan[i][j]; 
            G[i][j] = 255 - imageMagenta[i][j];
            B[i][j] = 255 - imageYellow[i][j];
        }
    }
}

unsigned char ***ErrorDiffusion::mergeColors() {
    unsigned char ***final = new unsigned char **[height];
    for(int i=0; i < height; i++) {
        final[i] = new unsigned char *[width];
        for(int j=0; j < width; j++) {
            final[i][j] = new unsigned char [BytesPerPixel];
            for(int k=0; k < BytesPerPixel; k++) {
                final[i][j][k] = 0;
            }
        }
    }
    
    mergeRGB();
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                if(k == 0) final[i][j][k] = R[i][j];
                else if(k == 1) final[i][j][k] = G[i][j];
                else final[i][j][k] = B[i][j];
            }
        }
    }
    return final;
}

int main(int argc, char *argv[]) {
    int height{atoi(argv[3])}, width{atoi(argv[4])}, BytesPerPixel{atoi(argv[5])};
    string file_input{argv[1]}, fileOutputFS{argv[2]};
    ErrorDiffusion *error = new ErrorDiffusion{height, width, 0, BytesPerPixel, 3};
    unsigned char *ImageData = new unsigned char[height*width*BytesPerPixel];
    error->readImage(file_input, ImageData);
    error->generateImage(ImageData);
    error->separateColor();
    error->writeImage(fileOutputFS, error->mergeColors());
    delete ImageData;
    delete error;
    return 0;
}