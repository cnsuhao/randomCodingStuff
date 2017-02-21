# ObjectDetection.py

import cv2
import math
import argparse

# get statrign args
ap = argparse.ArgumentParser()
ap.add_argument("-img", "--image", help="image path")
args = vars(ap.parse_args())

imagefile = ""

# get input image
if args.get("image", None) is not None:
	imagefile = args["image"]
else:
	imagefile = "img1.jpg"

im = cv2.cv.LoadImage(imagefile, cv2.cv.CV_LOAD_IMAGE_GRAYSCALE)
im2 = cv2.cv.CloneImage(im)

# Goodfeature to track algorithm
eigImage = cv2.cv.CreateMat(im.height, im.width, cv2.cv.IPL_DEPTH_32F)
tempImage = cv2.cv.CloneMat(eigImage)
cornerCount = 500
quality = 0.01
minDistance = 10

corners = cv2.cv.GoodFeaturesToTrack(im, eigImage, tempImage, cornerCount, quality, minDistance, 0, 0, True)

radius = 3
thickness = 2

for (x,y) in corners:
    cv2.cv.Circle(im, (int(x),int(y)), radius, (255,255,255), thickness)

cv2.cv.ShowImage("GoodfeaturesToTrack", im)

# SURF algorithm
hessthresh = 1500  # 400 500
dsize = 0  # 1
layers = 1  # 3 10

keypoints, descriptors = cv2.cv.ExtractSURF(im2, None, cv2.cv.CreateMemStorage(), (dsize, hessthresh, 3, layers))
for ((x, y), laplacian, size, dir, hessian) in keypoints:
    cv2.cv.Circle(im2, (int(x),int(y)), cv2.cv.Round(size / 2), (255,255,255), 1)
    x2 = x + ((size / 2) * math.cos(dir))
    y2 = y + ((size / 2) * math.sin(dir))
    cv2.cv.Line(im2, (int(x),int(y)), (int(x2),int(y2)), (255,255,255), 1)

cv2.cv.ShowImage("SURF ", im2)

cv2.cv.WaitKey(0)
