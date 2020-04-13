/*
#EE569 Homework Assignment #4
#Date: 12th March 2020
#Name: Yogesh Sanat Gajjar
#USCID: 5476153636
#Email: ygajjar@usc.edu

Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

------------------------------------------------------------------------
    Problem 1a 1b. Texture Classification
------------------------------------------------------------------------
STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output" 
3.  Run and execute the code and then write "./output height width train_read
4.  This will run the training images and write the feature vectors into a file called "featurevector_class.txt" 
5.  Follow the steps mentioned in the terminal. It says to run the next mode "test_read". Write in terminal "./output 128 128 test_read"
6.  After that, follow the commands mentioned in the terminal. 
7.  While executing the program in MATLAB, please mention the working directory where this program is saved. 
*/ 

#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <algorithm> 
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
#include <fstream> 


using namespace std;
using namespace cv;

class Classification {
private:
    unsigned char **Image, **ImagewithBoundary, **fixedThresh;
    int height, width, window, padding, **lawFilters, filterCount, trainCount, testCount;
    vector<string> fileName;
    vector<string> fileNameTest;
    double **intermediateImage, **featureVector, minFilter, maxFilter, average, **newfeatureVector, **zeromeanDataMatrix, **imageMeanSubtracted;

public:
    Classification(int, int, int);
    void readImage(char *, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    int **generateMatrix2DInt(int,int);
    double **generateMatrix2DDouble(int,int);
    unsigned char **boundaryExtension(double **);
    void delete2D(unsigned char **, int);
    void delete2DDouble(double **, int);
    void delete2DInt(int **, int);
    void subtractImageMean();
    void applyLawFilters();
    void calculateEnergy(int, int);
    void generateFilename(int);
    void convert25Dto15D(int);
    void computePCA(int, string);
    void generateFilenameTest();
    void kMeans15D(int);
    void kMeans3D(int);
    void deleteAll();
};

Classification::Classification(int h, int w, int win) {
    height = h;
    width = w;
    window = win;
    padding = win/2;
    filterCount = 25;
    average = 0;
    trainCount = 36;
    testCount = 12;
    minFilter = INT_MAX;
    maxFilter = INT_MIN;
}

void Classification::readImage(char *fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName,"rb"))) {
		cout << "Cannot open file: " << fileName <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);	
}

void Classification::writeImage(string filename, unsigned char **final) {
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

unsigned char **Classification::generateMatrix2D(int h, int w) {
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

int **Classification::generateMatrix2DInt(int h, int w) {
    long int count{};
    int **image = new int *[h];
    for(int i=0; i < h; i++) {
        image[i] = new int [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }
    return image;
}

double **Classification::generateMatrix2DDouble(int h, int w) {
    long int count{};
    double **image = new double *[h];
    for(int i=0; i < h; i++) {
        image[i] = new double [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }
    return image;
}


void Classification::generateImage(unsigned char *main_image) {
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

unsigned char **Classification::boundaryExtension(double **image) {
    ImagewithBoundary = generateMatrix2D((2*padding)+height, (2*padding)+width);

    long int count{};
    //center
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary[i+padding][j+padding] = image[i][j];
            count++;
        }
    }
    for(int i=0; i < padding; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary[i][j+padding] = image[padding -i-1][j];
            ImagewithBoundary[height+(2*padding)-i-1][j+padding] = image[height-padding+i][j];
        }
    }

    for(int i=0; i < padding; i++) {
        for(int j=0; j < height+(2*padding); j++) {
            ImagewithBoundary[j][i] = ImagewithBoundary[j][(2*padding)-i];
            ImagewithBoundary[j][width+(2*padding)-i-1] = ImagewithBoundary[j][width+(2*padding)-1-(2*padding)+i];
        }
    }

    return ImagewithBoundary;
}


void Classification::delete2D(unsigned char **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void Classification::delete2DDouble(double **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void Classification::delete2DInt(int **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}



void Classification::subtractImageMean() {
    imageMeanSubtracted = generateMatrix2DDouble(height, width);
    int meanSum{};
    float meanVal{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            meanSum += Image[i][j];
        }
    }

    meanVal = meanSum / (height*width);
    for(int i= 0; i < height; i++) {
        for(int j=0; j < width; j++) {
            imageMeanSubtracted[i][j] = (Image[i][j] - meanVal);
        }
    }
}


void Classification::applyLawFilters() {
    int filterInitialization[5][5] = {{1,4,6,4,1}, 
                                      {-1,-2,0,2,1},
                                        {-1,0,2,0,-1},
                                        {-1,2,0,-2,1}, 
                                        {1,-4,6,-4,1}};
    int row = sizeof(filterInitialization)/sizeof(filterInitialization[0]);
    lawFilters = generateMatrix2DInt(25,25);

    for(int i=0; i < row; i++) {
        for(int j=0; j < row; j++) {
            for(int k=0; k < row; k++) {
                for(int l=0; l < row; l++) {
                    lawFilters[i*row+j][k*row+l] = filterInitialization[i][k] * filterInitialization[j][l];
                }
            }
        }
    }
}

void Classification::generateFilename(int flag) {
    
    if(flag == 1){
        string ext = ".raw";
        string filename;
        vector<string> strname = {"blanket", "brick", "grass", "rice"};
        for(int p=0; p < strname.size(); p++) {
            for(int i = 1; i < 10; i++) {
                stringstream ss;
                ss << i;
                filename = strname[p] + ss.str() + ext;
                fileName.push_back(filename);
            }
        }
    }
    else {
        string ext = ".raw";
        string filename;
        for(int i = 1; i < 13; i++) {
            stringstream ss;
            ss << i;
            filename = ss.str() + ext;
            fileName.push_back(filename);
        }
    }
    
}

void Classification::calculateEnergy(int flag, int count) {
    featureVector = generateMatrix2DDouble(count,filterCount);  //change the variables
    unsigned char **temp;
    temp = generateMatrix2D((2*padding)+height, (2*padding)+width);
    intermediateImage = generateMatrix2DDouble(height, width);
    unsigned char *Imagedata = new unsigned char[height*width];

    for(int x = 0; x < count; x++) {
        char file_name[20] = {};
        generateFilename(flag);
        sprintf(file_name, fileName[x].data(), x+1);
        readImage(file_name, Imagedata);
        generateImage(Imagedata);
        subtractImageMean();
        applyLawFilters();
        temp = boundaryExtension(imageMeanSubtracted);  
        for(int y=0; y < filterCount; y++) {
            average = 0;
            for(int i=0; i < height; i++) {
                for(int j=0; j < width; j++) {
                    double sum = 0;
                    for(int l=0; l < window; l++) {
                        for(int m=0; m < window; m++) {
                            sum += temp[i+l][j+m] * lawFilters[y][l*5+m]; 
                        }
                    }
                    intermediateImage[i][j] = sum;
                }
            }

            for(int i=0; i < height; i++) {
                for(int j=0; j < width; j++) {
                    average += (intermediateImage[i][j] * intermediateImage[i][j]);
                }
            }

            average /= (height*width);
            featureVector[x][y] = average;
        }
    }



    for(int i=0; i < count; i++) {
        for(int j=0; j < filterCount; j++) {
            maxFilter = max(maxFilter, featureVector[i][j]);
            minFilter = min(minFilter, featureVector[i][j]);
        }
    }

    for(int i=0; i < count; i++) {
        for(int j=0; j < filterCount; j++) {
            featureVector[i][j] = ((featureVector[i][j] - minFilter)/ (maxFilter - minFilter)); // check if the its featurevector - minfilter or featurvector + minflter
        }
    }

    FILE *feature;
    feature=fopen("featurevector_class25D.txt","w");
    
    if(feature!=nullptr) {
        for(int i=0;i<count;i++) {
            for(int j=0;j<25;j++) {
                fprintf(feature,"%lf\t",featureVector[i][j]);
            }
            fprintf(feature,"\n");
        }
    }
    fclose(feature);


 
    delete2D(temp, height+(2*padding));   
}

void Classification::convert25Dto15D(int count) {

    newfeatureVector = generateMatrix2DDouble(count,15);
    int p = 0;
    for(int i=0; i < count; i++) {
        for(int j =0; j < 25; j++) {
            if (j==0) newfeatureVector[i][p] = featureVector[i][j];
            if (j == 1) newfeatureVector[i][p+1] = (featureVector[i][1] + featureVector[i][5])/2;
            if (j == 7) newfeatureVector[i][p+2] = (featureVector[i][7] + featureVector[i][11])/2;
            if (j == 6) newfeatureVector[i][p+3] = (featureVector[i][6]);
            if (j == 2)  newfeatureVector[i][p+4] = (featureVector[i][2] + featureVector[i][10])/2;
            if (j == 8)  newfeatureVector[i][p+5] = (featureVector[i][8] + featureVector[i][16])/2;
            if (j == 12) newfeatureVector[i][p+6] = (featureVector[i][6]);
            if (j ==3)  newfeatureVector[i][p+7] = (featureVector[i][3] + featureVector[i][15])/2;
            if (j == 9)  newfeatureVector[i][p+8] = (featureVector[i][9] + featureVector[i][21])/2;
            if (j == 18) newfeatureVector[i][p+9] = (featureVector[i][18]);
            if (j == 4)  newfeatureVector[i][p+10] = (featureVector[i][4] + featureVector[i][20])/2;
            if (j == 13)  newfeatureVector[i][p+11] = (featureVector[i][13] + featureVector[i][17])/2;
            if (j == 24) newfeatureVector[i][p+12] = (featureVector[i][24]);
            if (j == 19)  newfeatureVector[i][p+13] = (featureVector[i][19] + featureVector[i][23])/2;
            if (j == 14)  newfeatureVector[i][p+14] = (featureVector[i][14] + featureVector[i][22])/2;
        }
    }
    delete2DDouble(featureVector, count);
}

void Classification::computePCA(int count, string choice) {
    double *meanPCA = new double[15]; 
    zeromeanDataMatrix = generateMatrix2DDouble(count,3);
    for(int j=0; j < 15; j++) {
        double sum = 0;
        for(int i=0; i < count; i++) {
            sum += newfeatureVector[i][j]; 
        }
        meanPCA[j] = sum / count;
    }

    if (choice == "train_read") {
        FILE *feature;
        FILE *mean;
        feature=fopen("featurevector_class.txt","w");
        mean = fopen("meanvector_class.txt", "w");
        
        if(feature!=nullptr) {
            for(int i=0;i<count;i++) {
                for(int j=0;j<15;j++) {
                    fprintf(feature,"%lf\t",newfeatureVector[i][j]);
                }
                fprintf(feature,"\n");
            }
        }
        if(mean!=nullptr) {
        for(int i=0;i<15;i++) fprintf(mean,"%lf\t",meanPCA[i]);
        }
        fclose(mean);
        fclose(feature);
    }
    
    if(choice == "test_read") {
        FILE *feature;
        FILE *mean;
        feature=fopen("featurevector_classT.txt","w");
        mean = fopen("meanvector_classT.txt", "w");
        
        if(feature!=nullptr) {
            for(int i=0;i<count;i++) {
                for(int j=0;j<15;j++) {
                    fprintf(feature,"%lf\t",newfeatureVector[i][j]);
                }
                fprintf(feature,"\n");
            }
        }
        if(mean!=nullptr) {
        for(int i=0;i<15;i++) fprintf(mean,"%lf\t",meanPCA[i]);
        }
        fclose(mean);
        fclose(feature);
    }
    if (choice == "test_PCA") {
        double x;
        int i{}, j{};
        ifstream matlab;
        matlab.open("PCA_test.txt");
        if(!matlab) {
            cout << "Unable to open file";
            exit(1);
        }
        while(matlab >> x) {
            zeromeanDataMatrix[i][j] = x;
            j++;
            if(j == 3) {
                j=0;
                i++;
            }
        } 
        matlab.close();
    }

   
}


void Classification::kMeans15D(int count) {
    cout << "------------------ Test Image with 15D features ----------------" << endl;
    vector<string> groundTruth = {"Grass", "Blanket", "Blanket", "Brick", "Rice", "Grass", "Brick", "Rice", "Rice", "Brick", "Blanket", "Grass"};
    int *imageLabel = new int[testCount];
    vector<string> imageName;
    double **centroid = NULL;
    double **newCentroid = NULL;
    centroid = generateMatrix2DDouble(4, count);
    newCentroid = generateMatrix2DDouble(4, count);
    double minValue = 0;
    int grass{}, blanket{}, rice{}, brick{};

       
    for(int i=0; i < count; i++) {
        centroid[0][i] = newfeatureVector[0][i];
        centroid[1][i] = newfeatureVector[1][i];
        centroid[2][i] = newfeatureVector[3][i];
        centroid[3][i] = newfeatureVector[4][i];
    }

    for(int temp = 0; temp < 2; temp++) {
        int label[4] = {0};
        for(int i=0; i < testCount; i++) {
            double distance[4] = {0};
            for(int p=0; p < 4; p++) {
                for(int j=0; j < count; j++) {
                    distance[p] += sqrt(pow((centroid[p][j] - newfeatureVector[i][j]),2));
                }
            }
            minValue = distance[0];
            for(int x=0; x < 4; x++) {
                if(distance[x] < minValue) {
                    minValue = distance[x];
                }
            }
            

            if(minValue == distance[0]) {
                imageName.push_back("Grass");
                imageLabel[i] = 1;
                for(int t=0; t < count; t++) newCentroid[0][t] += newfeatureVector[i][t];
                grass++;
                label[0]++;
            }
            if(minValue == distance[1]) {
                imageName.push_back("Blanket");
                imageLabel[i] = 2;
                for(int t=0; t < count; t++) newCentroid[1][t] += newfeatureVector[i][t];
                blanket++;
                label[1]++;
            }
            if(minValue == distance[2]) {
                imageName.push_back("Brick");
                imageLabel[i] = 3;
                for(int t=0; t < count; t++) newCentroid[2][t] += newfeatureVector[i][t];
                brick++;
                label[2]++;
            }
            if(minValue == distance[3]) {
                imageName.push_back("Rice");
                imageLabel[i] = 4;
                for(int t=0; t < count; t++) newCentroid[3][t] += newfeatureVector[i][t];
                rice++;
                label[3]++;
            }
        }
    }
    for(int a=0; a < 4; a++) {
        for(int z=0; z < count; z++) {
            centroid[0][z] = (newCentroid[0][z]/grass);
            centroid[1][z] = (newCentroid[1][z]/blanket);
            centroid[2][z] = (newCentroid[2][z]/brick);
            centroid[3][z] = (newCentroid[3][z]/rice);
            newCentroid[a][z] = 0;
        }
    }

    double tensor;
    for(int i=0; i < groundTruth.size(); i++) {
        if(groundTruth[i] != imageName[i]) tensor++;
    }


    cout<<endl;
    cout<<"Test Image"<<"\t\t"<<"Class Label"<<"\t\t"<<"Class Name"<<"\t\t" << "Ground Truth"<<endl;
    for(int i=0;i<12;i++) cout<<i+1<<"\t\t\t"<<imageLabel[i]<<"\t\t\t"<<imageName[i]<< "\t\t\t" << groundTruth[i] << endl;
    cout<<endl;

    cout<<"\nFinal Class Labels:"<<endl;
    int grasscount{}, blanketcount{}, brickcount{}, ricecount{};
    cout<<"Images belong Grass: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 1) {
            cout<<j+1<<",";
            grasscount++;
        }
    }
    cout << " and Count for each label is: " << grasscount << endl;
    cout<<"Images belong to Blanket: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 2) {
            cout<<j+1<<",";
            blanketcount++;
        }
    }
    cout << " and Count for each label is: " << blanketcount << endl;
    cout<<"Images belong to Brick: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 3) {
            cout<<j+1<<",";
            brickcount++;
        }
    }
    cout << " and Count for each label is: " << brickcount << endl;
    cout<<"Images belong to Rice: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 4) {
            cout<<j+1<<",";
            ricecount++;
        }
    }
    cout << " and Count for each label is: " << ricecount << endl;
    cout << "\n";
    cout << "Misclassification error rate : " << ((tensor / groundTruth.size()) * 100) << "%" << endl;
    cout << endl;
    cout << endl;

    delete[] imageLabel;
    delete2DDouble(centroid, 4);
    delete2DDouble(newCentroid, 4);
}

void Classification::kMeans3D(int count) {
    cout << "------------------ Test Image with PCA reduced 3D features ----------------" << endl;
    vector<string> groundTruth = {"Grass", "Blanket", "Blanket", "Brick", "Rice", "Grass", "Brick", "Rice", "Rice", "Brick", "Blanket", "Grass"};
    int *imageLabel = new int[testCount];
    vector<string> imageName;
    double **centroid = NULL;
    double **newCentroid = NULL;
    centroid = generateMatrix2DDouble(4, count);
    newCentroid = generateMatrix2DDouble(4, count);
    double minValue = 0;
    int grass{}, blanket{}, rice{}, brick{};
    
    for(int i=0; i < count; i++) {
        centroid[0][i] = zeromeanDataMatrix[0][i];
        centroid[1][i] = zeromeanDataMatrix[1][i];
        centroid[2][i] = zeromeanDataMatrix[3][i];
        centroid[3][i] = zeromeanDataMatrix[4][i];
    }

    for(int temp = 0; temp < 2; temp++) {
        int label[4] = {0};
        for(int i=0; i < testCount; i++) {
            double distance[4] = {0};
            for(int p=0; p < 4; p++) {
                for(int j=0; j < count; j++) {
                    distance[p] += sqrt(pow((centroid[p][j] - zeromeanDataMatrix[i][j]),2));
                }
            }
            minValue = distance[0];
            for(int x=0; x < 4; x++) {
                if(distance[x] < minValue) {
                    minValue = distance[x];
                }
            }
            
            if(minValue == distance[0]) {
                imageName.push_back("Grass");
                imageLabel[i] = 1;
                for(int t=0; t < count; t++) newCentroid[0][t] += zeromeanDataMatrix[i][t];
                grass++;
                label[0]++;
            }
            if(minValue == distance[1]) {
                imageName.push_back("Blanket");
                imageLabel[i] = 2;
                for(int t=0; t < count; t++) newCentroid[1][t] += zeromeanDataMatrix[i][t];
                blanket++;
                label[1]++;
            }
            if(minValue == distance[2]) {
                imageName.push_back("Brick");
                imageLabel[i] = 3;
                for(int t=0; t < count; t++) newCentroid[2][t] += zeromeanDataMatrix[i][t];
                brick++;
                label[2]++;
            }
            if(minValue == distance[3]) {
                imageName.push_back("Rice");
                imageLabel[i] = 4;
                for(int t=0; t < count; t++) newCentroid[3][t] += zeromeanDataMatrix[i][t];
                rice++;
                label[3]++;
            }
        }
       
    }
    for(int a=0; a < 4; a++) {
        for(int z=0; z < count; z++) {
            centroid[0][z] = (newCentroid[0][z]/grass);
            centroid[1][z] = (newCentroid[1][z]/blanket);
            centroid[2][z] = (newCentroid[2][z]/brick);
            centroid[3][z] = (newCentroid[3][z]/rice);
            newCentroid[a][z] = 0;
        }
    }

    double tensor{};
    for(int i=0; i < groundTruth.size(); i++) {
        if(groundTruth[i] != imageName[i]) tensor++;
    }
    
    cout<<endl;
    cout<<"Test Image"<<"\t\t"<<"Class Label"<<"\t\t"<<"Class Name"<<"\t\t" << "Ground Truth"<<endl;
    for(int i=0;i<12;i++) cout<<i+1<<"\t\t\t"<<imageLabel[i]<<"\t\t\t"<<imageName[i]<< "\t\t\t" << groundTruth[i] << endl;
    cout<<endl;

    cout<<"\nFinal Class Labels:"<<endl;
    int grasscount{}, blanketcount{}, brickcount{}, ricecount{};
    cout<<"Images belong Grass: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 1) {
            cout<<j+1<<",";
            grasscount++;
        }
    }
    cout << " and Count for each label is: " << grasscount << endl;
    cout<<"Images belong to Blanket: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 2) {
            cout<<j+1<<",";
            blanketcount++;
        }
    }
    cout << " and Count for each label is: " << blanketcount << endl;
    cout<<"Images belong to Brick: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 3) {
            cout<<j+1<<",";
            brickcount++;
        }
    }
    cout << " and Count for each label is: " << brickcount << endl;
    cout<<"Images belong to Rice: ";
    for(int j = 0 ; j < 12 ; j++) {
        if(imageLabel[j] == 4) {
            cout<<j+1<<",";
            ricecount++;
        }
    }
    cout << " and Count for each label is: " << ricecount << endl;
    cout << "\n";
    cout << "Misclassification error rate : " << ((tensor / groundTruth.size()) * 100) << "%" << endl;

    delete[] imageLabel;
    delete2DDouble(centroid, 4);
    delete2DDouble(newCentroid, 4);
}

void Classification::deleteAll() {
    delete2D(Image, height);
    delete2DDouble(intermediateImage, height);
    delete2DInt(lawFilters, 25);
}

int main(int argc, char *argv[]) {
    int height{atoi(argv[1])}, width{atoi(argv[2])}, trainCount{36}, testCount{12}; 
    string choice{argv[3]};
    Classification *feature = new Classification(height, width, 5);

    if (choice == "train_read") {
        cout << "Train Read Mode: " << endl;
        feature->calculateEnergy(1, trainCount);
        feature->convert25Dto15D(trainCount);
        feature->computePCA(trainCount, choice);
        cout << "Now run the next mode i.e. test_read" << endl;
    }
    
    if(choice == "test_read") {
        cout << "Test Read Mode: " << endl;
        feature->calculateEnergy(0, testCount);
        feature->convert25Dto15D(testCount);
        feature->computePCA(testCount, choice);    
        cout << "Step 1: Open MATLAB and run the file classification.m. That file will read the .txt generated by this program and will perform PCA for both train and test images." << endl;
        cout << "Step 2: After executing the classification.m file in MATLAB, also execute the classification_test.m file to perform PCA for KMeans unsupervised learning classification. " <<
                         "Also ensure the working directory is same while entering it in the MATLAB file. " << endl;
        cout << "Step 3: After executing the all MATLAB files, run the next mode i.e test_PCA." << endl;   
    }
    
    if(choice == "test_PCA") {
        cout << "Test PCA mode: " << endl;
        feature->calculateEnergy(0, testCount);
        feature->convert25Dto15D(testCount);
        feature->computePCA(testCount, choice);
        feature->kMeans15D(15);
        feature->kMeans3D(3);
    }

    feature->deleteAll();
    delete feature;
    return 0;
}
