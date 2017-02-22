import numpy as np
import cv2

img = cv2.imread('images/img4.png')
gray = img.copy()
gray = cv2.cvtColor(gray, cv2.COLOR_BGR2GRAY)
# cv2.Canny(image, threshold1, threshold2[, edges[, apertureSize[, L2gradient]]])
edges = cv2.Canny(image=gray,
				  threshold1=50,
				  threshold2=150,
				  apertureSize=3,
				  L2gradient=True)
# cv2.imwrite('edges-50-150.jpg',edges)
minLineLength = 100
# lines = cv2.HoughLinesP(image=edges,rho=1,theta=np.pi/180, threshold=100,lines=np.array([]), minLineLength=minLineLength,maxLineGap=80)
lines = cv2.HoughLinesP(image=edges,
						rho=1,
						theta=np.pi / 90,
						threshold=100,
						lines=np.array([]),
						minLineLength=minLineLength,
						maxLineGap=1)
print "lines", lines

a,b,c = lines.shape
# for i in range(a):
for line in lines:
    cv2.line(gray,
    		 (line[0][0],
    		  line[0][1]),
    		 (line[0][2],
    		  line[0][3]),
    		 (0, 0, 255),
    		 3)
    # cv2.imwrite('houghlines5.jpg',gray)
cv2.imshow('houghlines', gray)
cv2.imshow('original image', img)
cv2.imshow('edges', edges)
cv2.moveWindow('original image', 0, 0)
cv2.moveWindow('edges', 600, 0)
cv2.moveWindow('houghlines', 1200, 0)
# cv2.waitKey(0)


contours = []
hierarchy = []
# cv2.findContours(image, mode, method[, contours[, hierarchy[, offset]]]) contours, hierarchy
contours, hierarchy = cv2.findContours(image=gray,
									   mode=cv2.RETR_CCOMP,
									   method=cv2.CHAIN_APPROX_SIMPLE)
# get biggest contour area
rectangles = []
areas = []
biggestArea = 0
biggestContour = None
for c in contours:
	rect = cv2.boundingRect(c)
	area = cv2.contourArea(c)
	if area > biggestArea:
		biggestArea = area
		biggestContour = c
	rectangles.append(rect)
	areas.append(area)

print 

cv2.drawContours(image=img,
				 	 contours=biggestContour,
				     contourIdx=0,
				     color=(0,255,0),
				     thickness=1,
				     lineType=1)
"""
for i in range(0, len(contours)):
	# cv2.drawContours(image, contours, contourIdx, color[, thickness[, lineType[, hierarchy[, maxLevel[, offset]]]]])
	cv2.drawContours(image=img,
				 	 contours=contours,
				     contourIdx=i,
				     color=(0,255,0),
				     thickness=1,
				     lineType=1)
"""
"""
c = None
if contours is not None and len(contours) > 0:
    c = sorted(contours, key=cv2.contourArea, reverse=True)
    if len(c) > 0:
        c = sorted(contours, key=cv2.contourArea, reverse=True)[0]
"""
# cv2.drawContours(image, contours, contourIdx, color[, thickness[, lineType[, hierarchy[, maxLevel[, offset]]]]])
"""
cv2.drawContours(img,
				 contours,
				 0,
				 (0,255,0),
				 1,
				 1)
"""
cv2.imshow("contours", img)
cv2.moveWindow('contours', 0, 400)
cv2.waitKey(0)
