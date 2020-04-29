import cv2
import numpy as np

img1 = cv2.imread("right.png")
img2 = cv2.imread("middle.png")


orb = cv2.ORB_create()

minHessian = 400
detector = cv2.xfeatures2d_SURF.create(hessianThreshold=minHessian)
keypoints1, descriptors1 = detector.detectAndCompute(img1, None)
keypoints2, descriptors2 = detector.detectAndCompute(img2, None)



matcher = cv2.DescriptorMatcher_create(cv2.DescriptorMatcher_FLANNBASED)
knn_matches = matcher.knnMatch(descriptors1, descriptors2, 2)

# print(knn_matches)

ratio_thresh = 0.7
good_matches = []
for m,n in knn_matches:
    if m.distance < ratio_thresh * n.distance:
        good_matches.append(m)


matching_result = cv2.drawMatches(img1, keypoints1, img2, keypoints2, good_matches[:30], None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

list_kp1 = [keypoints1[mat.queryIdx].pt for mat in good_matches[:30]]
list_kp2 = [keypoints2[mat.trainIdx].pt for mat in good_matches[:30]]

print(list_kp1)
print(" ")
print(list_kp2)
# print(good_matches[:4])
cv2.imshow('Good Matches', matching_result)
# cv2.imshow("middle", img2)
cv2.waitKey()

