
#include "color_halftoning.h"
#include <iostream>
#include <string>

using namespace std;

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