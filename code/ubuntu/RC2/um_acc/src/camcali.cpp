#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"
#include <iostream>
 
#include "ros/ros.h"
#include "um_acc/CarState_L.h"
#include <sstream>
#include <vector>
#define UPPER 16.5
#define LOWER 8.5
#define SIDE 13.0
 
using namespace cv;
using namespace cv::cuda;
using namespace std;

// Image size (640, 480)
 
int main(int argc, char **argv)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
 
    ros::init(argc, argv, "publisher");
    ros::NodeHandle n;
    ros::Publisher lane_pub = n.advertise<um_acc::CarState_L>("lane_data", 1000);
    ros::Rate loop_rate(3);
    um_acc::CarState_L msg;
         
    double phi = 0, y = 0;
    double lane_width_thresh = 50, screen_mid = 240;
    double y_center = -0.16, car_length = 16 + 17;  //car_length in cm
    double t_curr = 0, t_prev, delta_t = 0.01;
    double y_prev = 0, phi_prev = 0;

    int cnt = 0;
        Mat src;
        cap >> src; // get a new frame from camera
        t_curr = ros::Time::now().toSec(); // get time of camera measurement from ros
     
        cuda::GpuMat d_src(src);
        cuda::GpuMat d_lines, d_dst;
        //cuda::cvtColor(d_src, d_dst, CV_BGR2GRAY, 0, cuda::Stream::Null());
        //gpu::bilateralFilter(d_dst, d_dst, -1, 50, 7);
        
        Mat cdst(d_src);
        imshow("SRC", cdst);

        Point2f inputQuad[4]; 
        Point2f outputQuad[4];
        Mat lambda(2, 4, CV_32FC1);
        
        lambda = Mat::zeros(cdst.rows, cdst.cols, cdst.type());
        // The 4 points that select quadilateral on the input , from top-left in clockwise order

        int distortPixDown = 3000;
        int backgroudHeight = 150;
        int distortPixUp = 0;
        int newHeight = 360;
        int newWidth = 320;

        inputQuad[0] = Point2f(distortPixUp, backgroudHeight);
        inputQuad[1] = Point2f(cdst.cols - 1 - distortPixUp, backgroudHeight);
        inputQuad[2] = Point2f(cdst.cols - 1 + distortPixDown, cdst.rows - 1);
        inputQuad[3] = Point2f(- distortPixDown, cdst.rows - 1);  
        // The 4 points where the mapping is to be done , from top-left in clockwise order
        outputQuad[0] = Point2f(0, 0);
        outputQuad[1] = Point2f(newWidth-1, 0);
        outputQuad[2] = Point2f(newWidth-1, newHeight - 1);
        outputQuad[3] = Point2f(0, newHeight - 1);
     
        // Get the Perspective Transform Matrix i.e. lambda 
        lambda = getPerspectiveTransform(inputQuad, outputQuad);
        // Apply the Perspective Transform just found to the src image
        warpPerspective(cdst, cdst, lambda, Size(newWidth, newHeight), INTER_LINEAR, BORDER_CONSTANT, (255, 255, 255));
 

        imshow("SRC", cdst);
        imwrite("/media/ubuntu/SD/src.jpg", src);
        imwrite("/media/ubuntu/SD/transformed.jpg", cdst);

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
