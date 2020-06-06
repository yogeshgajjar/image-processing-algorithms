import cv2
import numpy as np
import matplotlib.pyplot as plt
import math
import csv


def generateVocab():
    minHessian = 400
    dictsize = 8
    BOW = cv2.BOWKMeansTrainer(dictsize)

    detector = cv2.xfeatures2d.SIFT_create(minHessian)
    img1 = cv2.imread("Husky_1.jpg")
    img2 = cv2.imread("Husky_2.jpg")
    img3 = cv2.imread("Husky_3.jpg")
    img4 = cv2.imread("Puppy_1.jpg")
    keypoints1, descriptors1 = detector.detectAndCompute(img1, None)
    keypoints2, descriptors2 = detector.detectAndCompute(img2, None)
    keypoints3, descriptors3 = detector.detectAndCompute(img3, None)
    keypoints4, descriptors4 = detector.detectAndCompute(img4, None)

    
    
    
    BOW.add(descriptors2)
    BOW.add(descriptors1)
    BOW.add(descriptors4)

    codebook = BOW.cluster()

    return descriptors1, descriptors2, descriptors3, descriptors4, codebook


def calculateDistance(descriptors, codebook):
    # distance = []
    index = 0
    minimum = 0
    eucDistance = np.zeros(shape=(8))
    freq = eucDistance
    index_list = []
    for i in range(descriptors.shape[0]):
        for j in range(codebook.shape[0]):
            distance = 0
            for k in range(descriptors.shape[1]):
                distance += (descriptors[i][k] - codebook[j][k])*(descriptors[i][k] - codebook[j][k])

            eucDistance[j] = math.sqrt(distance)
            # print(eucDistance)
        minimum = eucDistance[0]
        index = 0
        for p in range(1, codebook.shape[0]):
            if(minimum < eucDistance[p]):
                minimum = eucDistance[p]
                index = p
        index_list.append(index)
        freq[index] += 1
    
    return freq, index_list

def codewordsCompare(freq, freq_1):
    error = 0
    for i in range(8):
        error += abs(freq[i] - freq_1[i])

    error /= 8
    return error


def main():
    k_clusters = 8
    des1, des2, des3, des4, vocab = generateVocab()

    distance_vocab1, ind1 = calculateDistance(des1, vocab)
    distance_vocab2, ind2 = calculateDistance(des2, vocab)
    distance_vocab3, ind3 = calculateDistance(des3, vocab)
    distance_vocab4, ind4 = calculateDistance(des4, vocab)

    plt.figure(1)
    plt.hist(ind1, np.arange(0,9)-0.5, facecolor="blue", edgecolor="black", alpha=0.5)
    plt.xlabel("Cluster's")
    plt.ylabel("Frequency of samples falling into each cluster")
    plt.title("Histogram for the image Husky_2")

    plt.figure(2)
    plt.hist(ind2, np.arange(0,9)-0.5, facecolor="red", edgecolor="black", alpha=0.5)
    plt.xlabel("Cluster's")
    plt.ylabel("Frequency of samples falling into each cluster")
    plt.title("Histogram for the image Husky_1")

    plt.figure(3)
    plt.hist(ind3, np.arange(0,9)-0.5, facecolor="green", edgecolor="black", alpha=0.5)
    plt.xlabel("Cluster's")
    plt.ylabel("Frequency of samples falling into each cluster")
    plt.title("Histogram for the image Husky_3")

    plt.figure(4)
    plt.hist(ind4, np.arange(0,9)-0.5, facecolor="yellow", edgecolor="black", alpha=0.5)
    plt.xlabel("Cluster's")
    plt.ylabel("Frequency of samples falling into each cluster")
    plt.title("Histogram for the image Puppy_1")

    # with open("hist1.csv", 'w', newline='') as myfile:
    #     wr = csv.writer(myfile, quoting=csv.QUOTE_ALL)
    #     for word in ind1:
    #         wr.writerow([word])

    # print(ind1)
    plt.figure(5)
    plt.plot(distance_vocab1, 'r', label="Husky_1")
    plt.plot(distance_vocab3, 'g', label="Husky_3")
    plt.xlabel("Cluster CodeBook")
    plt.ylabel("Features of the image")
    plt.title("Husky_1 vs Husky_3")

    plt.figure(6)
    plt.plot(distance_vocab2, 'b', label="Husky_2")
    plt.plot(distance_vocab3, 'g', label="Husky_3")
    plt.xlabel("Cluster CodeBook")
    plt.ylabel("Features of the image")
    plt.title("Husky_2 vs Husky_3")

    plt.figure(7)
    plt.plot(distance_vocab4, 'y', label="Puppy_1")
    plt.plot(distance_vocab3, 'g', label="Husky_3")
    plt.xlabel("Cluster CodeBook")
    plt.ylabel("Features of the image")
    plt.title("Puppy_1 vs Husky_3")

    plt.figure(8)
    plt.plot(distance_vocab1, 'r', label="Husky_1")
    plt.plot(distance_vocab2, 'b', label="Husky_2")
    plt.plot(distance_vocab3, 'g', label="Husky_3")
    plt.plot(distance_vocab4, 'y', label="Puppy_1")
    plt.xlabel("Cluster CodeBook")
    plt.ylabel("Features of the image")
    plt.title("Husky_1 vs Husky_2 vs Husky_3 vs Puppy_1")


    plt.legend()
    # plt.xlabel("Sample Values")
    # plt.ylabel("Frequency of samples falling into each bin")
    # plt.title("Histogram of 5000 Uniformly Distributed Samples")
    plt.show()


    error1 = codewordsCompare(distance_vocab1, distance_vocab3)
    error2 = codewordsCompare(distance_vocab2, distance_vocab3)
    error3 = codewordsCompare(distance_vocab4, distance_vocab3)

    print("Error with respect to :", error1)
    print("Error with respect to :", error2)
    print("Error with respect to : ", error3)


if __name__ == "__main__":
    main()


