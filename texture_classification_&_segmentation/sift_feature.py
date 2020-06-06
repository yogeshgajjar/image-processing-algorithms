import cv2
import numpy as np 
import matplotlib.pyplot as plt 

def partA(img1, img2): 
    img2 = cv2.imread("Husky_1.jpg")
    img1 = cv2.imread("Husky_3.jpg")

    minHessian = 400
    detector_1 = cv2.xfeatures2d.SIFT_create(minHessian)

    keypoints1, descriptors1 = detector_1.detectAndCompute(img1, None)
    keypoints2, descriptors2 = detector_1.detectAndCompute(img2, None)

    result_1 = cv2.drawKeypoints(img1, keypoints1, img1) #, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    result_2 = cv2.drawKeypoints(img2, keypoints2, img2) #, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    # cv2.imwrite('/home/yogesh/envirnments/opencv_env/husky_images/husky_3_keypoints_orientation.jpg', result_1)
    # cv2.imwrite('/home/yogesh/envirnments/opencv_env/husky_images/husky_1_keypoints_orientation.jpg', result_2)

    matcher = cv2.DescriptorMatcher_create(cv2.DescriptorMatcher_FLANNBASED)
    knn_matches = matcher.knnMatch(descriptors1, descriptors2, 2)


    ratio_thresh = 0.75
    good_matches = []
    for m,n in knn_matches:
        if m.distance < ratio_thresh * n.distance:
            good_matches.append(m)

    # good_matches = sorted(good_matches, key = lambda x:x.distance)
    matching_result = cv2.drawMatches(img1, keypoints1, img2, keypoints2, good_matches[:1], None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
    cv2.imshow('new', matching_result)
    cv2.imwrite('/home/yogesh/envirnments/opencv_env/husky_images/matching_husky31.jpg', matching_result)
    cv2.waitKey()

def partB(img1, img2):

    minHessian = 400
    detector_1 = cv2.xfeatures2d.SIFT_create(minHessian)

    keypoints1, descriptors1 = detector_1.detectAndCompute(img1, None)
    keypoints2, descriptors2 = detector_1.detectAndCompute(img2, None)

    result_1 = cv2.drawKeypoints(img1, keypoints1, img1) #, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    result_2 = cv2.drawKeypoints(img2, keypoints2, img2) #, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    # cv2.imwrite('husky_3_keypoints_orient.jpg', result_1)
    # cv2.imwrite('husky_1_keypoints_orient.jpg', result_2)

    matcher = cv2.DescriptorMatcher_create(cv2.DescriptorMatcher_FLANNBASED)
    knn_matches = matcher.knnMatch(descriptors1, descriptors2, 2)


    ratio_thresh = 0.75
    good_matches = []
    for m,n in knn_matches:
        if m.distance < ratio_thresh * n.distance:
            good_matches.append(m)

    good_matches = sorted(good_matches, key = lambda x:x.distance)
    matching_result = cv2.drawMatches(img1, keypoints1, img2, keypoints2, good_matches, None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
    cv2.imshow('new', matching_result)
    cv2.waitKey()
    cv2.imwrite('/home/yogesh/envirnments/opencv_env/husky_images/matching_husky1puppy1.jpg', matching_result)

def main():
    img2 = cv2.imread("Husky_1.jpg")
    img1 = cv2.imread("Husky_3.jpg")
    img3 = cv2.imread("Husky_2.jpg")
    img4 = cv2.imread("Puppy_1.jpg")

    # partA(img1, img2)
    partB(img2, img4)

if __name__ == "__main__":
    main()