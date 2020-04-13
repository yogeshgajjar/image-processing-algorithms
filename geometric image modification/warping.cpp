
/*
#EE569 Homework Assignment #3
#Date: 20th February 2020
#Name: Yogesh Sanat Gajjar
#USCID: 5476153636
#Email: ygajjar@usc.edu

Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

------------------------------------------------------------------------
    Problem 1a. Geometric Warping
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the input image, height, width, BytePerPixel using command line 
2.  First, start with the conversion of square to disc. The height & width is reranged to [-1,1] and used the forward mapping formula to convert it to disc. As the formula used for forward 
mapping assumes the disc and square to be in the range [-1,1]. Stored the pixel values in a variable and printed it.  
3.  Next, performed the inverse mapping from disc to square image. First converted the square to disc(similar to step 2) and again converted it back to square to get the final image similar to the input
image. Used the inverse mapping formula after getting the pixel value of the disc. Stored the new pixel value in a variable and printed it.  
4.  Write the output onto .raw file.  


STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image1.raw input_image2.raw input_image3.raw warped_output1.raw warped_output2.raw warped_output3.raw original_image_out1.raw original_image_out2.raw original_image_out3.rawheight width BytePerPixel
4.  Use the Image Viewing Software to see the results.

*/ 


#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>

using namespace std;

class Warping {
private:
    unsigned char ***Image, ***imageWarp, ***imageorig;
    int height, width, BytesPerPixel;
    double x,y,u,v, *arrayU, *arrayV;

public:
    Warping(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImage(unsigned char *);
    unsigned char ***generateMatrix3D(int, int, int);
    unsigned char ***diskWarping();
    unsigned char ***planeWarping();
    int signum(double);
    void delete3D(unsigned char ***, int, int);
};

Warping::Warping(int h, int w, int b) {
    height = h;
    width = w;
    BytesPerPixel = b;
    u = v = x = y = 0.0;
}


void Warping::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void Warping::writeImage(string filename, unsigned char ***final) {
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

void Warping::generateImage(unsigned char *main_image) {
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

unsigned char ***Warping::generateMatrix3D(int h, int w, int b) {
    long int count{};
    unsigned char ***image = new unsigned char **[h];
    for(int i=0; i < h; i++) {
        image[i] = new unsigned char *[w];
        for(int j=0; j < w; j++) {
            image[i][j] = new unsigned char [b];
            for(int k=0; k < b; k++) {
                image[i][j][k] = 0;
                count++;
            }
        }
    }

    return image;
}

int Warping::signum(double x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

unsigned char ***Warping::diskWarping() {
    unsigned char ***image;
    image = generateMatrix3D(height, width, BytesPerPixel);
    imageWarp = generateMatrix3D(height, width, BytesPerPixel);
    imageorig = generateMatrix3D(height, width, BytesPerPixel);
    image = Image;
    double oldmin{}, oldmax{height}, newmax{1}, newmin{-1};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                x = ((i - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin; 
                y = ((j - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin;
                u = (x * (sqrt(1 - (y * y)/2))+1)*(height/2);
                v = (y * (sqrt(1 - (x * x)/2))+1)*(height/2);
                imageWarp[(int)u][(int)v][k] = image[i][j][k];           
            }
        }
    }
    return imageWarp; 
}



unsigned char ***Warping::planeWarping() {
    unsigned char ***image;
    double new_x{}, new_y{};
    image = generateMatrix3D(height, width, BytesPerPixel);
    imageorig = generateMatrix3D(height, width, BytesPerPixel);
    image = Image;
    double oldmin{}, oldmax{height}, newmax{1}, newmin{-1};

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                x = ((i - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin; 
                y = ((j - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin;
                               
                u = (x * (sqrt(1 - (y * y)/2)));
                v = (y * (sqrt(1 - (x * x)/2)));
                
                if (i == 0 && j == 0) {
                    new_x = 0;
                    new_y = 0;
                    continue;
                }

                x = ((0.5 * (sqrt(2 + (u * u) - (v * v) + 2 * sqrt(2) * u))) - (0.5 * (sqrt(2 + (u * u) - (v * v) - 2 * sqrt (2) * u))));
                y = ((0.5 * (sqrt(2 - (u * u) + (v * v) + 2 * sqrt(2) * v))) - (0.5 * (sqrt(2 - (u * u) + (v * v) - 2 * sqrt (2) * v))));
                double new_x = (x + 1)*(height/2);
                double new_y = (y + 1)*(height/2);
                imageorig[i][j][k] = image[(int)new_x][(int)new_y][k];                
            }   
        }
    }
    
    return imageorig; 
}

void Warping::delete3D(unsigned char ***image, int h, int w) {
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
			delete[] image[i][j];
		delete[] image[i];
	}
	delete[] image;
}


int main(int argc, char *argv[]) {
    int height{atoi(argv[10])}, width{atoi(argv[11])}, BytesPerPixel{atoi(argv[12])};
    string file_ih{argv[1]}, file_ir{argv[2]}, file_ib{argv[3]}, file_oh_1{argv[4]}, file_or_1{argv[5]}, file_ob_1{argv[6]}, file_oh_2{argv[7]}, file_or_2{argv[8]}, file_ob_2{argv[9]}; 
    Warping *warp = new Warping{height, width, BytesPerPixel};
    unsigned char *ImageData = new unsigned char[height*width*BytesPerPixel];
    warp->readImage(file_ih, ImageData);
    warp->generateImage(ImageData);
    warp->writeImage(file_oh_1, warp->diskWarping());
    warp->writeImage(file_oh_2, warp->planeWarping());
    warp->delete3D(warp->diskWarping(), height, width);
    warp->delete3D(warp->planeWarping(), height, width);

    warp->readImage(file_ir, ImageData);
    warp->generateImage(ImageData);
    warp->writeImage(file_or_1, warp->diskWarping());
    warp->writeImage(file_or_2, warp->planeWarping());
    warp->delete3D(warp->diskWarping(), height, width);
    warp->delete3D(warp->planeWarping(), height, width);

    warp->readImage(file_ib, ImageData);
    warp->generateImage(ImageData);
    warp->writeImage(file_ob_1, warp->diskWarping());
    warp->writeImage(file_ob_2, warp->planeWarping());
    warp->delete3D(warp->diskWarping(), height, width);
    warp->delete3D(warp->planeWarping(), height, width);

    delete ImageData;
    delete warp;
    return 0;
}