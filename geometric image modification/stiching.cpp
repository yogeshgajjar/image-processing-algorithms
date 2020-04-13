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
    Problem 1b. Homographic Transformation and Image Stitching
------------------------------------------------------------------------
STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output left_image.raw middle_image.raw right_image.raw final_output.raw height width ByteperPixels
4.  Use the Image Viewing Software to see the results.

*/ 

#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>

using namespace std;


class Stiching {
private:
    unsigned char ***Image, ***ImageL, ***ImageM, ***ImageR, ***imageOut;
    int height, width, BytesPerPixel, new_height{}, new_width{};

public:
    Stiching(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImageL(unsigned char *);
    void generateImageM(unsigned char *);
    void generateImageR(unsigned char *);
    unsigned char ***generateMatrix3D(int, int, int);
    void delete3D(unsigned char ***, int, int);
    unsigned char ***homographicTransformation();    

};

Stiching::Stiching(int h, int w, int b) {
    height = h;
    width = w;
    BytesPerPixel = b;
    new_height = 1200;
    new_width = 2000;
}

void Stiching::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void Stiching::writeImage(string filename, unsigned char ***final) {
    FILE *file;

    unsigned char finalImg[new_height*new_width*BytesPerPixel];
    long int count{};
    for(int i=0; i < new_height; i++) {
        for(int j=0; j < new_width; j++) {
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
	fwrite(finalImg, sizeof(unsigned char), new_height*new_width*BytesPerPixel, file);
	fclose(file);

}

void Stiching::generateImageL(unsigned char *main_image) {
    long int count{};
    ImageL = new unsigned char **[height];
    for(int i = 0; i < height; i++) {
        ImageL[i] = new unsigned char *[width];
        for(int j = 0; j < width; j++) {
            ImageL[i][j] = new unsigned char [BytesPerPixel];
            for(int k=0; k < BytesPerPixel; k++) {
                ImageL[i][j][k] = main_image[count];
                count++;
            }
        }
    }    
}

void Stiching::generateImageM(unsigned char *main_image) {
    long int count{};
    ImageM = new unsigned char **[height];
    for(int i = 0; i < height; i++) {
        ImageM[i] = new unsigned char *[width];
        for(int j = 0; j < width; j++) {
            ImageM[i][j] = new unsigned char [BytesPerPixel];
            for(int k=0; k < BytesPerPixel; k++) {
                ImageM[i][j][k] = main_image[count];
                count++;
            }
        }
    }    
}

void Stiching::generateImageR(unsigned char *main_image) {
    long int count{};
    ImageR = new unsigned char **[height];
    for(int i = 0; i < height; i++) {
        ImageR[i] = new unsigned char *[width];
        for(int j = 0; j < width; j++) {
            ImageR[i][j] = new unsigned char [BytesPerPixel];
            for(int k=0; k < BytesPerPixel; k++) {
                ImageR[i][j][k] = main_image[count];
                count++;
            }
        }
    }    
}

unsigned char ***Stiching::generateMatrix3D(int h, int w, int b) {
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

unsigned char ***Stiching::homographicTransformation() {
    // float H1[3][3] = {{1.8352, 0.0460, -338.6923},
    //                  {0.6605, 1.4968, -187.0543},
    //                  {0.0019, -0.000122, 1}};

    // float H1[3][3] = {{2.8471, -0.0004, 131.224},
    //                  {0.7020, 1.5361, -198.66},
    //                  {0.0020, -0.00010, 1}};

    // Pretty good one. Dont delete this
    // float H1[3][3] = {{2.8974, 0.1684, -474.3520},
    //                  {0.8655, 1.6410, -127.5682},
    //                  {0.0014, -0.000144, 1}};

    // float H1[3][3] = {{2.3761, 0.0680, 165.6609},
    //                  {0.4443, 1.5122, -160.1883},
    //                  {0.0014, 0.00003, 1}};

    double H1[3][3] = {{0.5764, -0.0207, 183.0248},
                     {-0.2883, 0.8444, 51.8647},
                     {-0.0008, 0, 1}};

    // double H1[3][3] = {{0.5142, 0.0138, 183.1661},
    //                    {-0.4332, 0.9558, 39.9584}, 
    //                    {-0.0012, 0.0002, 1.0000}};


    float H2[3][3] = {{1.84834428579621, 0.0704595686501150, -350.265597547593},
                     {0.579582659345601, 1.56603059117170, -184.983622011381},
                     {0.00172737114676377, 0.00003, 1}};


    imageOut = generateMatrix3D(new_height, new_width, BytesPerPixel);
   
    int p{}, q{};

    for(int i=-(new_width/5); i < 4*(new_width/5); i++) {
        for(int j=-(new_height/5); j < 4*(new_height/5); j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                double w2 = (H1[2][0]*i + H1[2][1]*j + H1[2][2]*1);
                double x2 = (H1[0][0]*i + H1[0][1]*j + H1[0][2]*1);
                double y2 = (H1[1][0]*i + H1[1][1]*j + H1[1][2]*1);

                x2 = x2 / w2;
                y2 = y2 / w2;

                if (x2 > 0 && x2 < width-1 && y2 > 0 && y2 < height-1) {
                    p = floor(x2);
                    q = floor(y2);
                    float delta_x2 = x2 - floor(x2);
                    float delta_y2 = y2 - floor(y2);


                    imageOut[j+(new_height/5)+2][i+(new_width/5)][k] = (1 - delta_x2)*(1 - delta_y2)*ImageL[q][p][k] + (delta_x2)*(1 - delta_y2)*ImageL[q][p+1][k] + 
                                                            (1 - delta_x2)*(delta_y2)*ImageL[q+1][p][k] + (delta_x2)*(delta_y2)*ImageL[q+1][p+1][k];


                }
             }
        }
    }

    
    for(int i=-(new_width/5); i < 4*(new_width/5); i++) {
        for(int j=-(new_height/5); j < 4*(new_height/5); j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                double w2 = (H2[2][0]*i + H2[2][1]*j + H2[2][2]*1);
                double x2 = (H2[0][0]*i + H2[0][1]*j + H2[0][2]*1);
                double y2 = (H2[1][0]*i + H2[1][1]*j + H2[1][2]*1);

                x2 = x2 / w2;
                y2 = y2 / w2;

               
                if (x2 > 0 && x2 < width-1 && y2 > 0 && y2 < height-1) {
                    p = floor(x2);
                    q = floor(y2);
                    float delta_x2 = x2 - floor(x2);
                    float delta_y2 = y2 - floor(y2);


                    imageOut[j+(new_height/5)][i+(new_width/5)][k] = (1 - delta_x2)*(1 - delta_y2)*ImageR[q][p][k] + (delta_x2)*(1 - delta_y2)*ImageR[q][p+1][k] + 
                                                            (1 - delta_x2)*(delta_y2)*ImageR[q+1][p][k] + (delta_x2)*(delta_y2)*ImageR[q+1][p+1][k];


                }
            }
        }
    }

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                imageOut[i+240][j+403][k] = ImageM[i][j][k];
            }
        }
    }    
    return imageOut;
}

void Stiching::delete3D(unsigned char ***image, int h, int w) {
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
			delete[] image[i][j];
		delete[] image[i];
	}
	delete[] image;
}



int main(int argc, char *argv[]) {

    int height{atoi(argv[5])}, width{atoi(argv[6])}, BytesPerPixel{atoi(argv[7])};
    string file_il{argv[1]}, file_im{argv[2]}, file_ir{argv[3]}, file_oh_1{argv[4]};  
    Stiching *stich = new Stiching{height, width, BytesPerPixel};
   
    unsigned char *ImageDataL = new unsigned char[height*width*BytesPerPixel];
    unsigned char *ImageDataM = new unsigned char[height*width*BytesPerPixel];
    unsigned char *ImageDataR = new unsigned char[height*width*BytesPerPixel];
    stich->readImage(file_il, ImageDataL);
    stich->readImage(file_ir, ImageDataR);
    stich->readImage(file_im, ImageDataM);
    stich->generateImageL(ImageDataL);
    stich->generateImageM(ImageDataM);
    stich->generateImageR(ImageDataR);
    stich->writeImage(file_oh_1, stich->homographicTransformation());
    stich->delete3D(stich->homographicTransformation(), 1200, 2000);

    delete ImageDataL;
    delete stich;
    return 0;
}