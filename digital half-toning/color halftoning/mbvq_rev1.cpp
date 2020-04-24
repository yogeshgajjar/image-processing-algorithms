
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

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string> 

using namespace std; 

class MBVQ {
private: 
    unsigned char ***Image, ***Final, **ImagewithBoundary2D, ***imageTemp, ***imageTempBinarized, ***ImagewithBoundary3D;
    int  **R, **G, **B;
    int height, width, BytesPerPixel, window, padding, red, blue, green;
    long double error;
    string mbvq, vertex;
public:
    MBVQ(int, int, int, int, long double);
    string getNearestVortex(vector<unsigned char>);
    unsigned char **boundaryExtension(int, unsigned char **);
    unsigned char ***boundaryExtension3D(unsigned char ***);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    void findMBVQ(vector<unsigned char>);
    unsigned char ***errorFS();
    unsigned char ***generateMatrix3D(int, int, int);
    vector<unsigned char> colorRepresentation(string);
};

MBVQ::MBVQ(int h, int w, int b, int win, long double e) {
    height = h;
    width = w;
    BytesPerPixel = b;
    window = win;
    padding = window/2;
    error = e;
    red = 0;
    blue= 0;
    green = 0;
}

void MBVQ::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void MBVQ::writeImage(string filename, unsigned char ***final) {
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
unsigned char **MBVQ::generateMatrix2D(int h, int w) {
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


unsigned char ***MBVQ::generateMatrix3D(int h, int w, int b) {
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


void MBVQ::generateImage(unsigned char *main_image) {
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

unsigned char **MBVQ::boundaryExtension(int pad, unsigned char **image) {

    ImagewithBoundary2D = generateMatrix2D((2*pad)+height, (2*pad)+width);

    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary2D[i+pad][j+pad] = image[i][j];
            count++;
        }
    }
    return ImagewithBoundary2D;
}

unsigned char ***MBVQ::boundaryExtension3D(unsigned char ***image) {
    ImagewithBoundary3D = generateMatrix3D((2*padding)+height, (2*padding)+width, BytesPerPixel);
    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k = 0; k < BytesPerPixel; k++) {
                ImagewithBoundary3D[i+padding][j+padding][k] = image[i][j][k];
                count++;
            }
        }
    }
    return ImagewithBoundary3D;


}
void MBVQ::findMBVQ(vector<unsigned char> image) {
    red = image[0];
    green = image[1];
    blue = image[2];

    // We will receive the RGB(i,j) from the original_pixel. 

    if (red + green > 255) {
        if (green + blue > 255) {
            if (red + green + blue > 510) mbvq = "CMYW";
            else mbvq = "MYGC";
        }
        else mbvq = "RGMY";
    }
    else {
        if (green + blue <= 255) {
            if (red + green + blue <= 255) mbvq = "KRGB";
            else mbvq = "RGBM";
        }
        else mbvq = "CMGB";
    }
}

string MBVQ::getNearestVortex(vector<unsigned char> image) {
    findMBVQ(image);
    if (mbvq == "CMYW") {
		vertex = "white";
		if (blue < 128) {
			if (blue <= red) {
				if (blue <= green) vertex = "yellow";
			}
		}
		if (green < 128) {
			if (green <= blue) {
				if (green <= red) vertex = "magenta";
			}
		}
		if (red < 128) {
			if (red <= blue) {
				if (red <= green) vertex = "cyan";
			}
		}
	}

    // No.2 for MYGC
	if (mbvq == "MYGC") {
		vertex = "magenta";
		if (green >= blue) {
			if (red >= blue) {
				if (red >= 128) vertex = "yellow";
				else vertex = "green";
			}
		}
		if (green >= red) {
			if (blue >= red) {
				if (blue >= 128) vertex = "cyan";
				else vertex = "green";
			}
		}
	}

	// No.3 for RGMY
	if (mbvq == "RGMY") {
		if (blue > 128) {
			if (red > 128) {
				if (blue >= green) vertex = "magenta";
				else vertex = "yellow";
			}
			else {
				if (green > blue + red) vertex = "green";
				else vertex = "magenta";
			}
		}
		else {
			if (red >= 128) {
				if (green >= 128) vertex = "yellow";
				else vertex = "red";
			}
			else {
				if (red >= green) vertex = "red";
				else vertex = "green";
			}
		}
	}

	// No.4 for KRGB
	if (mbvq == "KRGB") {
		vertex = "black";
		if (blue > 128) {
			if (blue >= red) {
				if (blue >= green) vertex = "blue";
			}
		}
		if (green > 128) {
			if (green >= blue) {
				if (green >= red) vertex = "green";
			}
		}
		if (red > 128) {
			if (red >= blue) {
				if (red >= green) vertex = "red";
			}
		}
	}

	// No.5 for RGBM
	if (mbvq == "RGBM") {
		vertex = "green";
		if (red > green) {
			if (red >= blue) {
				if (blue < 128) vertex = "red";
				else vertex = "magenta";
			}
		}
		if (blue > green) {
			if (blue >= red) {
				if (red < 128) vertex = "blue";
				else vertex = "magenta";
			}
		}
	}

	// No.6 for CMGB
	if (mbvq == "CMGB") {
		if (blue > 128) {
			if (red > 128) {
				if (green >= red) vertex = "cyan";
				else vertex = "magenta";
			}
			else {
				if (green > 128) vertex = "cyan";
				else vertex = "blue";
			}
		}
		else {
			if (red > 128) {
				if (red - green + blue >= 128) vertex = "magenta";
				else vertex = "green";
			}
			else {
				if (green >= blue) vertex = "green";
				else vertex = "blue";
			}
		}
	}
    return vertex;
}

vector<unsigned char> MBVQ::colorRepresentation(string) {
    unsigned char var1{}, var2{}, var3{};
    vector <unsigned char> temp;
    if(vertex == "white") {
        var1 = 0;
        var2 = 0;
        var3 = 0;
    }
    else if(vertex == "yellow") {
        var1 = 0;
        var2 = 0;
        var3 = 255;
    }
    else if(vertex == "cyan") {
        var1 = 0;
        var2 = 255;
        var3 = 0;
    }
    else if(vertex == "magenta") {
        var1 = 255;
        var2 = 0;
        var3 = 0;
    }
    else if(vertex == "green") {
        var1 = 0;
        var2 = 255;
        var3 = 255;
    }
    else if(vertex == "red") {
        var1 = 255;
        var2 = 0;
        var3 = 255;
    }
    else if(vertex == "blue") {
        var1 = 255;
        var2 = 255;
        var3 = 0;
    }
    else if(vertex == "black") {
        var1 = 255;
        var2 = 255;
        var3 = 255;
    }

    // if(vertex == "black") {
    //     var1 = 0;
    //     var2 = 0;
    //     var3 = 0;
    // }   
    // else if(vertex == "blue") {
    //     var1 = 0;
    //     var2 = 0;
    //     var3 = 255;
    // }
    // else if(vertex == "green") {
    //     var1 = 0;
    //     var2 = 255;
    //     var3 = 0;
    // }
    // else if(vertex == "red") {
    //     var1 = 255;
    //     var2 = 0;
    //     var3 = 0;
    // }
    // else if(vertex == "cyan") {
    //     var1 = 0;
    //     var2 = 255;
    //     var3 = 255;
    // }
    // else if(vertex == "magenta") {
    //     var1 = 255;
    //     var2 = 0;
    //     var3 = 255;
    // }
    // else if(vertex == "yellow") {
    //     var1 = 255;
    //     var2 = 255;
    //     var3 = 0;
    // }
    // else if(vertex == "white") {
    //     var1 = 255;
    //     var2 = 255;
    //     var3 = 255;
    // }

    temp.push_back(var1);
    temp.push_back(var2);
    temp.push_back(var3);

    return temp;
}

unsigned char ***MBVQ::errorFS() {
    float FS[3][3] = {{0,0,0}, 
                      {0,0,7.0},
                      {3.0,5.0,1.0}};

    cout << Image[0][0][0] << endl;
    
    imageTemp = boundaryExtension3D(Image);
    imageTempBinarized = generateMatrix3D(height, width, BytesPerPixel);
    // double error;
    unsigned char ***temp, ***temp1;
    temp = generateMatrix3D(height, width, BytesPerPixel);
    temp1 = generateMatrix3D(height, width, BytesPerPixel);
    temp = boundaryExtension3D(Image);
    temp1 = boundaryExtension3D(temp1);

    string label{};
    vector<unsigned char> channel;

    for (int i = padding; i < height+padding; i++ ) {
        if((i-padding)%2 == 0) {
            for (int j = padding; j < width+padding; j++) {
                channel.clear();
                for(int k=0; k < BytesPerPixel; k++) {
                    if (k == 0) channel.push_back(temp[i][j][1]);
                    if (k == 1) channel.push_back(temp[i][j][0]);
                    if (k == 2) channel.push_back(temp[i][j][k]);
                }
                label = getNearestVortex(channel);
                vector<unsigned char> color = colorRepresentation(label);
                for(int k=0; k < BytesPerPixel; k++) {
                    temp1[i][j][k] = color[k];
                    error = imageTemp[i][j][k] - temp1[i][j][k];
                    imageTempBinarized[i-padding][j-padding][k] = 255 - temp1[i][j][k];
                    for (int x = 0; x < window; x++) {
                        for (int y = 0; y < window; y++) {
                            imageTemp[i-padding+x][j-padding+y][k] += (unsigned char)(error * (FS[x][y]/16.0));
                        }
                    }
                }                
            }
        }

        if((i-padding)%2 !=0) {
            for (int j = width; j > 0; j--) {
                channel.clear();
                for(int k=0; k < BytesPerPixel; k++) {
                    if (k == 0) channel.push_back(temp[i][j][1]);
                    if (k == 1) channel.push_back(temp[i][j][0]);
                    if (k == 2) channel.push_back(temp[i][j][k]);
                }
                label = getNearestVortex(channel);
                vector<unsigned char> color = colorRepresentation(label);
                for(int k=0; k < BytesPerPixel; k++) {
                    temp1[i][j][k] = color[k];
                    error = imageTemp[i][j][k] - temp1[i][j][k];
                    imageTempBinarized[i-padding][j-padding][k] = 255 - temp1[i][j][k];
                    for (int x = 0; x < window; x++) {
                        for (int y = window-1; y >= 0; y--) {
                            imageTemp[i-padding+x][j-padding+y][k] += (unsigned char)(error * (FS[x][y]/16.0));
                        }
                    }
                }
            }
        }
        
    }

    return imageTempBinarized;
}

int main(int argc, char *argv[]) {
    int height{atoi(argv[3])}, width{atoi(argv[4])}, BytesPerPixel{atoi(argv[5])};
    string file_input{argv[1]}, fileOutput{argv[2]};
    MBVQ *mbvq = new MBVQ{height, width, BytesPerPixel, 3, 0};
    unsigned char *Imagedata = new unsigned char[height*width*BytesPerPixel];
    mbvq->readImage(file_input, Imagedata);
    mbvq->generateImage(Imagedata);
    mbvq->writeImage(fileOutput, mbvq->errorFS());
    delete Imagedata;
    delete mbvq;
    return 0;

}   
