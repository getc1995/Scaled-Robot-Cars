#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"
#include <iostream>
#include <cmath>
 
#include "ros/ros.h"
#include <um_acc/CarState_L.h>
#include <um_acc/Motor_data.h>
#include <um_acc/IMU.h>
#include <sstream>
#include <vector>
#define SIDE 70.0
#define BLACKTHRE 95
 
using namespace cv;
using namespace cv::cuda;
using namespace std;

// Src image size (640, 480)
float u = 0;
float gz = 0;

void motorCallback(const um_acc::Motor_data::ConstPtr& msg)
{
  u = float(msg->countPerSecond) * 0.004641;
}

void IMUCallback(const um_acc::IMU::ConstPtr& msg)
{
  gz = float(msg->gz);
}
 
int main(int argc, char **argv)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
 
    ros::init(argc, argv, "publisher");
    ros::NodeHandle n;
    ros::Publisher lane_pub = n.advertise<um_acc::CarState_L>("lane_data1", 1000);
    ros::Subscriber motor_sub = n.subscribe("Motor_data1", 2, motorCallback);
    ros::Subscriber IMU_sub = n.subscribe("IMU_data1", 2, IMUCallback);

    ros::Rate loop_rate(10);
    um_acc::CarState_L msg;
         
    double phi = 0, y = 0;
    double sum_phi = 0, theta_n = 0;//Not used
    double lane_edge_1 = -1, lane_edge_2 = -1, x_mid = 0;//Not used
    double lane_edge_1_count = 0, lane_edge_2_count = 0;//Not used
    double lane_width_thresh = 50, screen_mid = 240;
    double y_center = 0.16, car_length = 18 + 17;
    //car_length in cm (distance from lower bound of camera screen to the front wheel, + distance of front wheel to center of mass)
 
    double t_curr = 0, t_prev, delta_t = 0.01;
    double y_prev = 0, phi_prev = 0;
    double beginTime = ros::Time::now().toSec();

 
    while(ros::ok())
    {
        Mat src;
        cap >> src; // get a new frame from camera
	    t_curr = ros::Time::now().toSec(); // get time of camera measurement from ros
     
        cuda::GpuMat d_src(src);
        cuda::GpuMat d_lines, d_dst;
        cuda::cvtColor(d_src, d_dst, CV_BGR2GRAY, 0, cuda::Stream::Null());
        //gpu::bilateralFilter(d_dst, d_dst, -1, 50, 7);
 		
        Mat cdst(d_dst);
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
 
        vector<int> Mid_X, Mid_Y;
        for(int i = 0 ; i < cdst.rows ; i ++)
        {
            vector<int> black;
            for(int j = 0 ; j < cdst.cols ; j ++)
            {
                char pixel = cdst.at<char>(i,j);
                if ( pixel < BLACKTHRE )
                {
                	cdst.at<char>(i,j) = 0;
                    black.push_back(j);
                }
            }
            if( black.empty() == 0) // nonempty
            {
                int avg = accumulate( black.begin(), black.end(), 0.0) / black.size();
                Mid_X.push_back(avg);
                Mid_Y.push_back(i);
            }
             
            black.clear();
        }
         
        for(int i = 0 ; i < Mid_X.size() ; i ++)
        {
             cdst.at<char>(Mid_Y[i], Mid_X[i]) = 255;
        }
 
 
        // x = a0 + a1*y + a2 * y^2
        Mat Ymat(Mid_X.size(), 3, CV_32FC1);
        Mat YmatT(3, Mid_X.size(), CV_32FC1);
        Mat Xmat(Mid_Y.size(), 1, CV_32FC1);
        Mat Tmp33(3, 3, CV_32FC1);
        Mat a(3, 1, CV_32FC1);
 
        for(int i = 0 ; i < Mid_X.size() ; i ++)
        {
            YmatT.at<float>(0, i) = 1;
            YmatT.at<float>(1, i) = Mid_Y[i];
            YmatT.at<float>(2, i) = Mid_Y[i]*Mid_Y[i];
 
            Ymat.at<float>(i, 0) = 1;
            Ymat.at<float>(i, 1) = Mid_Y[i];
            Ymat.at<float>(i, 2) = Mid_Y[i]*Mid_Y[i];
 
            Xmat.at<float>(i, 0) = Mid_X[i];
        }
        Tmp33 = YmatT*Ymat;
        Tmp33 = Tmp33.inv();
 
        a = Tmp33*YmatT*Xmat;
 
		cout << a << endl;
 
        for(int i = 0 ; i < cdst.rows ; i ++)
        {
            int Tmpx =  a.at<float>(0, 0) + a.at<float>(0, 1)*i + a.at<float>(0, 2)*i*i;
            if( ( Tmpx >= 0)  && ( Tmpx < newWidth) )
            {
                cdst.at<char>(i, Tmpx) = 255;
            }
        }
 
        imshow("GRAY", cdst);
 
        Mid_X.clear();
        Mid_Y.clear();
        Ymat.release();
        YmatT.release();
        Xmat.release();
 
        float ratio = SIDE/newWidth;  //real to pixel
        y = - ratio * float(a.at<float>(0, 0) + a.at<float>(0, 1)*newHeight + a.at<float>(0, 2)*newHeight*newHeight - newWidth/2.0); 
        cout << "y = " << y << endl;
        
        //k is the slope of the curve at the bottom of the screen
        float k = (a.at<float>(0, 1) + 2*a.at<float>(0, 2)*newHeight);
        cout << "phi = " << atan(k) * 180/3.1415926 << endl;
        phi = atan(k);
 		
 		float rho = sqrt(1+k*k)*(1+k*k)/(2*a.at<float>(0, 2))*ratio/100.0;
 		cout << "rho = " << rho << endl;
        
 
        //transformation to get lane displacement
        //phi is positive in the clockwise direction
        msg.phi = phi;
        msg.y = ((y - y_center) - (car_length)*tan(msg.phi))/100.0; //msg.y is in meters.
		cout << msg.y << " " << msg.phi << endl;

		msg.time = t_curr - beginTime;
        delta_t = t_curr - t_prev;

        msg.rd = u/rho;
        msg.r = gz;
        msg.u = u;
        msg.v = (msg.y - y_prev)/delta_t;

        t_prev = t_curr;
        y_prev = msg.y;
        phi_prev = msg.phi;
        
        lane_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
 
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
