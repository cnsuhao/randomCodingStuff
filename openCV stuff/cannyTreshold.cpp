#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


// build:
// g++ `pkg-config --cflags opencv` cannyTreshold.cpp -std=c++11 `pkg-config --libs opencv` -o cannyTreshold
// run:
// ./cannyTreshold

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

int MinContourArea = 100;
int MaxContourArea = 10000;

/// Function header
void thresh_callback(int, void* );

double getArea(Rect rect) {
  int width = rect.br().x - rect.tl().x;
  int height = rect.br().y - rect.tl().y;

  return width * height;
}

/** @function main */
int main( int argc, char** argv )
{
  if (argv[1] == NULL) {
    cout << "no arguments given!" << endl;
    return 0;
  }
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );

  if (src.empty()) {
    cout << "no valid image given!" << endl;
    return 0;
  }
  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  resize(src_gray, src_gray, Size(), 0.5, 0.5, INTER_LINEAR);
  resize(src, src, Size(), 0.5, 0.5, INTER_LINEAR);
  
  /// Create Window
  namedWindow( "Source", CV_WINDOW_AUTOSIZE );
  imshow( "Source", src );
  moveWindow("Source", 0, 0);

  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  moveWindow("Contours", 0, 600);

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  createTrackbar( " Min Contour Area:", "Source", &MinContourArea, MaxContourArea, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output,
                contours, 
                hierarchy, 
                CV_RETR_TREE,
                CV_CHAIN_APPROX_SIMPLE,
                Point(0, 0)
                );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         //double area = contourArea(contours[i]);
         Rect rect = boundingRect(contours[i]);
         double area = getArea(rect);
         if (area >= MinContourArea) {
              drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
         }
         // void putText(Mat& img, const string& text, Point org, int fontFace, double fontScale, Scalar color, int thickness=1, int lineType=8, bool bottomLeftOrigin=false )
         //putText(drawing, "", rect, int fontFace, double fontScale, Scalar color, int thickness=1, int lineType=8, bool bottomLeftOrigin=false )
     }

  /// Show in a window
  imshow( "Contours", drawing );
}