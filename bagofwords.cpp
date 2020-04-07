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
    Problem 1c. Bag of Words
------------------------------------------------------------------------
STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output file_name1.jpg file_name2.jpg file_name3.jpg file_name4.jpg 

*/ 


#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;


class BagOfWords {
private:    
    Mat image, descriptor_1, descriptor, featureList;
    vector<KeyPoint> keypoints_1, keypoints;
    vector<Mat> Descriptor;
    int minHessian, clusters;
    vector<int> list_index;
    Mat codeBook;
    double *nearestDistance, *frequency, **histogramFrequency;

public:
    BagOfWords();
    unsigned char **generateMatrix2D(int, int);
    int **generateMatrix2DInt(int,int);
    double **generateMatrix2DDouble(int,int);
    void delete2D(unsigned char **, int);
    void delete2DDouble(double **, int);
    void delete2DInt(int **, int);
    void readImages(vector<string>);
    void calculateDistance();
    void writeHistogram();
    void calculateClusters();
}; 

BagOfWords::BagOfWords() {
    minHessian = 400;
    clusters = 8;
}

unsigned char **BagOfWords::generateMatrix2D(int h, int w) {
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

int **BagOfWords::generateMatrix2DInt(int h, int w) {
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

double **BagOfWords::generateMatrix2DDouble(int h, int w) {
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

void BagOfWords::readImages(vector<string> file) {

    // to read the training images and clustering them to make a codebook
    Ptr<SIFT> detector = SIFT::create(minHessian);
    for(int i=0; i < file.size()-1; i++) {
        image = imread(file[i], IMREAD_GRAYSCALE);
        detector->detect(image, keypoints);
        detector->compute(image, keypoints, descriptor);
        featureList.push_back(descriptor);
        Descriptor.push_back(descriptor);
    }

    //separately read the test image and append its descriptor to the main vector. 
    Mat image_test = imread(file[3]);
    detector->detect(image_test, keypoints_1);
    detector->compute(image_test, keypoints_1, descriptor_1);
    Descriptor.push_back(descriptor_1);
}

void BagOfWords::calculateClusters() {
    BOWKMeansTrainer BOW(clusters);
    codeBook = BOW.cluster(featureList);
}

void BagOfWords::calculateDistance() {
    int count = 0;
    double minimum = INT_MAX;
    nearestDistance = new double[clusters];
    frequency = new double[clusters];

    histogramFrequency = generateMatrix2DDouble(4,8);

    for(int itr=0; itr < Descriptor.size(); itr++) {
        for(int i=0; i < descriptor.rows; i++) {
            for(int j=0; j < clusters; j++) {
                double dist = 0;
                for(int k=0; k < descriptor.cols; k++) {
                    dist += pow((Descriptor[itr].at<float>(i,k) - codeBook.at<float>(j,k)),2);
                }
                nearestDistance[j] = sqrt(dist);
                minimum = min(minimum, nearestDistance[j]);
                if(minimum == nearestDistance[j]) count = j;
            }
            frequency[count]++;
            list_index.push_back(count);
            for(int b=0; b < clusters; b++) {
                histogramFrequency[itr][b] = frequency[b];
            }
        }
    }
    delete [] nearestDistance;
    delete [] frequency;
}

void BagOfWords::writeHistogram() {
    ofstream hist_data;
    hist_data.open("Histogram.txt");
    if(hist_data.is_open()) {
        for(int i=0; i < 4; i++) {
            for(int j=0; j < clusters; j++) {
                hist_data << "\t" << histogramFrequency[i][j];
            }
            hist_data << "\n";
        }
        hist_data.close();
    }
    delete2DDouble(histogramFrequency, 4);
}


int main(int argc, char *argv[]) {
    string file_input1{argv[1]}, file_input2{argv[2]}, file_input3{argv[3]}, file_input4{argv[4]};

    vector<string> filename; 
    filename.push_back(file_input1);
    filename.push_back(file_input2);
    filename.push_back(file_input4);
    filename.push_back(file_input3);

    BagOfWords *bog = new BagOfWords();
    bog->readImages(filename);
    bog->calculateClusters();
    bog->calculateDistance(); // send descriptor_list and codebook 
    bog->writeHistogram();

    delete bog;
    return 0;
}


