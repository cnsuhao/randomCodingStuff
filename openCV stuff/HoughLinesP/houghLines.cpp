/* This is a standalone program. Pass an image name as the first parameter
of the program.  Switch between standard and probabilistic Hough transform
by changing "#if 1" to "#if 0" and back */
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <vector>

// build:
// g++ `pkg-config --cflags opencv` houghLines.cpp -std=c++11 `pkg-config --libs opencv`
// run:
// ./a.out img1.jpg

using namespace std;
using namespace cv;


const double MIN_LINE_LENGTH = 0;

struct ImageData {
    Mat dst;
    Mat color_dst;
    vector<double> angles;

    ImageData(Mat dst1, Mat color_dst1, vector<double> angles1) {
        dst = dst1;
        color_dst = color_dst1;
        angles = angles1;
    }
};

// return the orientation which line closest to
float getLineDegrees(Vec4i line) {
    int x1 = min(line[0], line[2]);
    int x2 = max(line[0], line[2]);
    int y1 = min(line[1], line[3]);
    int y2 = max(line[1], line[3]);

    // calculate angle/degrees
    float angle = atan2(y1 - y2, x1 - x2);
    float degrees = (angle * 180) / CV_PI;

    // convert negative value to positive
    if (degrees < 0) {
        degrees = -degrees;
    }

    //cout << "1angle: " << angle << endl;
    //cout << "1degrees: " << degrees << endl;

    return degrees;
}

// Calcuates distance between two points == line length
double getLineLength(Vec4i line) {

    //cout << line << endl;
    
    int x1 = min(line[0], line[2]);
    int x2 = max(line[0], line[2]);
    int y1 = min(line[1], line[3]);
    int y2 = max(line[1], line[3]);

    int distancex = (x2 - x1)^2;
    int distancey = (y2 - y1)^2;

    if (distancex < 0) {
        distancex = -distancex;
    }
    if (distancey < 0) {
        distancey = -distancey;
    }
    int bigdVal = max(distancex, distancey);
    int smallVal = min(distancex, distancey);
    double distance = sqrt(bigdVal - smallVal);
    return distance;
}

double getMedianAngle(vector<double> angles) {
    double medAngle = 0;
    double sum = 0;

    for (int i = 0; i < angles.size(); i++) {
        sum += angles[i];
    }

    medAngle = sum / angles.size();

    return medAngle;
}

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

ImageData getHoughLines(Mat src, Mat dst, Mat color_dst) {

    double angle = 0;
    vector<double> angles;
    vector<Vec4i> lines;

    Canny( src, dst, 50, 200, 3 ); // Canny edges
    cvtColor( dst, color_dst, CV_GRAY2BGR ); // gray image
    HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10); // houghlines

    for( size_t i = 0; i < lines.size(); i++ )
    {
        //cout << "line length: " << getLineLength(lines[i]) << endl;
        // if line is long enough draw it
        if (getLineLength(lines[i]) >= 0) {
            // calculate angle in degrees
            
            float degrees = getLineDegrees(lines[i]);
            //cout << endl;
            //cout << "line " << i << endl;
            //cout << "degrees: " << degrees << endl;
            //cout << "length: " << getLineLength(lines[i]) << endl;
            //cout << lines[i] << endl;
            Point textPosition = Point(lines[i][0], lines[i][1]);
            
            //putText(color_dst, to_string(i), textPosition, FONT_HERSHEY_SIMPLEX, 1, Scalar(255,100,100), 2, 8, false);
            
            //horizontal
            if(degrees < 45) {
                // draw that line
                line( color_dst, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 1, 0); 
            } else if (degrees > 135 && degrees < 225) {
                line( color_dst, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 1, 0);
                angle = degrees - 180;
                angles.push_back(angle);
            } else if (degrees > 315) {
                line( color_dst, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 1, 0); 
            }
            // Else its vertical
            else {
                line( color_dst, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(255,0,0), 1, 8 ); // red
            }
            
        }
    }

    ImageData imagedata{dst, color_dst, angles};

    return imagedata;
}

Mat OpenWarpPerspective(const Mat& _image
  , const Point2f& _tl
  , const Point2f& _tr
  , const Point2f& _br
  , const Point2f& _bl
  , const Point2f& _tl_result
  , const Point2f& _tr_result
  , const Point2f& _br_result
  , const Point2f& _bl_result)
{
    // todo do some checks on input.
    Mat transform_matrix;
    Point2f source_points[4];
    Point2f dest_points[4];


    source_points[0] = _tl;
    source_points[1] = _tr;
    source_points[2] = _br;
    source_points[3] = _bl;

    dest_points[0] = _tl_result;
    dest_points[1] = _tr_result;
    dest_points[2] = _br_result;
    dest_points[3] = _bl_result;

    Mat dst;
    transform_matrix = getPerspectiveTransform(source_points, dest_points);
    warpPerspective(_image, dst, transform_matrix, dst.size());
    return dst;  
}

int main(int argc, char** argv)
{
    Mat src, dst, color_dst;
    if( argc != 2 || !(src=imread(argv[1], 0)).data) {
        cout << "no imagefile given!" << endl;
        return -1;   
    }

    ImageData imagedata = getHoughLines(src, dst, color_dst);
    

    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "Detected Lines", 1 );
    imshow( "Detected Lines", imagedata.color_dst );

    Mat rotated = src;
    double medianAngle = getMedianAngle(imagedata.angles);
    cout << endl;
    cout << "medianAngle: " << medianAngle << endl;
    rotated = rotate(rotated, medianAngle);

    namedWindow( "Rotated image", 1 );
    imshow( "Rotated image", rotated );

    waitKey(0);
    
    imagedata = getHoughLines(rotated, dst, color_dst);

    Size size(imagedata.color_dst.cols * 2, imagedata.color_dst.rows * 2);
    Mat dst2 = imagedata.color_dst;
    resize(imagedata.color_dst, dst2, size);

    namedWindow("Detected Lines", 1);
    imshow("Detected Lines", dst2);
    /*
    rotated = src;
    medianAngle = getMedianAngle(imagedata.angles);
    cout << endl;
    cout << "medianAngle: " << medianAngle << endl;
    rotated = rotate(rotated, medianAngle);

    namedWindow( "Rotated image", 1 );
    imshow( "Rotated image", rotated );
    */
    waitKey(0);
    //tl,tr,br,bl,tl_r,tr_r,br_r,bl_r
    Point2f tl(0, 0);
    Point2f tr(0, 300);
    Point2f bl(0, 300);
    Point2f br(300, 300);

    Point2f tl_r(10, 10);
    Point2f tr_r(10, 290);
    Point2f bl_r(10, 290);
    Point2f br_r(290, 290);

    Mat perspectiveImage = OpenWarpPerspective(src,tl,tr,br,bl,tl_r,tr_r,br_r,bl_r);
    imshow("perspective transformation", perspectiveImage);

    waitKey(0);

    return 0;
}