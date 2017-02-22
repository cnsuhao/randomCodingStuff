# camera.py

import cv2
import argparse

WINDOW_WIDTH = 1920
WINDOW_HEIGHT = 1080

# aloitus argumentit
ap = argparse.ArgumentParser()
# construct the argument parse and parse the arguments
ap.add_argument("-i", "--input", help="video input, 0-1")

args = vars(ap.parse_args())

global videoInput


def getStartingArgs(args):
    global videoInput

    if len(args) > 0:
        if args["input"]:
            videoInput = args["input"]
        else:
        	videoInput = 0
    return


getStartingArgs(args)

# get image capture
capture = cv2.VideoCapture(int(videoInput))
# settings for capture
capture.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, WINDOW_WIDTH)
capture.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, WINDOW_HEIGHT)
capture.set(cv2.cv.CV_CAP_PROP_FPS, 24)  # 24 better than 30, no waveing imagelights

cv2.namedWindow("video")
cv2.moveWindow("video", 0, 0)

if not capture.isOpened():
    print "no video input opened"
    capture.release()
else:
    while True:
        # get frame fro mcapture device
        _, frame = capture.read()

       	if frame is not None:
       		cv2.imshow("video", frame)

       	key = cv2.waitKey(1)
       	if key == 27:  # escape
       		break
       	elif key == 10:  # enter
       		print "taking a picture! saved: images/photo.png"
       		cv2.imwrite("images/photo.png", frame)
