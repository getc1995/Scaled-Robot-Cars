#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"

#include <ros/ros.h>
#include <um_acc/Motor_data.h>
#include <um_acc/IMU.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt16.h>
#include "Car_parameters.h"


#include <iostream>
#include <sstream>
using namespace cv;
using namespace std;

#define steer_step 10

ros::Publisher servo_pub;
ros::Publisher motor_pub;

float v = 0;
float i = 0;
float r = 0;
int steer = 0;

string s_v, s_i, s_r, s_s;

// Mat3b img(300, 300, Vec3b(0, 255, 0));
Mat3b canvas;
string winName = "Manual Control";

Rect button_left;
Rect button_right;
Rect button_up;
Rect button_down;
Rect info;
string button_left_text("<-");
string button_right_text("->");
string button_up_text("^");
string button_down_text("v");

int button_contour = 1;
int dark = 150;

void refreshButtons(int r, int g, int b);
void callBackFunc(int event, int x, int y, int flags, void* userdata);
void addButtonText(Rect &button, string &buttonText);
void IMUCallback(const um_acc::IMU::ConstPtr& msg);
void motorCallback(const um_acc::Motor_data::ConstPtr& msg);
void servoCallback(const std_msgs::UInt16::ConstPtr& msg);


void IMUCallback(const um_acc::IMU::ConstPtr& msg)
{
    r = msg->gz;
    s_r = "r: " + to_string(r);
}

void motorCallback(const um_acc::Motor_data::ConstPtr& msg)
{
    i = msg->current;
    v = msg->countPerSecond * CPS2V;
    s_v = "v: " + to_string(v);
    s_i = "i: " + to_string(i);
}

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        if (button_left.contains(Point(x, y)))
        {
            cout << "left Clicked!" << endl;
            canvas(button_left) = Vec3b(dark,dark,dark);
            addButtonText(button_left, button_left_text);
            steer = steer - steer_step;
        }
        else if (button_right.contains(Point(x, y)))
        {
            cout << "right Clicked!" << endl;
            canvas(button_right) = Vec3b(dark,dark,dark);
            addButtonText(button_right, button_right_text);
            steer = steer + steer_step;
        }
        else if (button_up.contains(Point(x, y)))
        {
            cout << "up Clicked!" << endl;
            canvas(button_up) = Vec3b(dark,dark,dark);
            addButtonText(button_up, button_up_text);
        }
        else if (button_down.contains(Point(x, y)))
        {
            cout << "down Clicked!" << endl;
            canvas(button_down) = Vec3b(dark,dark,dark);
            addButtonText(button_down, button_down_text);
        }
        else{

        }
    }
    if (event == EVENT_LBUTTONUP)
    {
        refreshButtons(200,200,200);
    }

    imshow(winName, canvas);
    waitKey(1);
}

void addButtonText(Rect &button, string &buttonText){
    putText(canvas(button), buttonText, Point(button.width*0.35, button.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0,0,0));
}

void refreshButtons(int r, int g, int b){
    canvas(button_left) = Vec3b(r,g,b);
    canvas(button_right) = Vec3b(r,g,b);
    canvas(button_up) = Vec3b(r,g,b);
    canvas(button_down) = Vec3b(r,g,b);
    addButtonText(button_left, button_left_text);
    addButtonText(button_right, button_right_text);
    addButtonText(button_up, button_up_text);
    addButtonText(button_down, button_down_text);
}

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "remote_gui1");
    ros::NodeHandle nh;
    servo_pub = nh.advertise<std_msgs::UInt16>("servo1", 1);
    motor_pub = nh.advertise<std_msgs::Float32>("Motor_command1", 1);

    ros::Subscriber IMU_sub = nh.subscribe("IMU_data1", 2, IMUCallback);
    ros::Subscriber motor_sub = nh.subscribe("Motor_data1", 2, motorCallback);
    std_msgs::UInt16 servoMsg;

    ros::Rate loop_rate(100);
    // An image
    int canvas_h = 180*2; // 750;
    int canvas_w = 320*2; // 1334

    // The canvas
    canvas = Mat3b(canvas_h, canvas_w, Vec3b(0,0,0));

    // Your button
    button_left = Rect(0, canvas_h/2, canvas_w/3, canvas_h/2);
    button_right = Rect(canvas_w/3, canvas_h/2, canvas_w/3, canvas_h/2);
    button_up = Rect(2*canvas_w/3, 0, canvas_w/3, canvas_h/2);
    button_down = Rect(2*canvas_w/3, canvas_h/2, canvas_w/3, canvas_h/2);
    info = Rect(0, 0, 2*canvas_w/3, canvas_h/2);

    refreshButtons(200, 200, 200);



    // // Draw the button
    // canvas(button_left) = Vec3b(200,200,200);
    // canvas(button_right) = Vec3b(200,200,200);
    // canvas(button_up) = Vec3b(200,200,200);
    // canvas(button_down) = Vec3b(200,200,200);
    //     // putText(canvas(button), buttonText, Point(button.width*0.35, button.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));
    // addButtonText(button_left, button_left_text);
    // addButtonText(button_right, button_right_text);
    // addButtonText(button_up, button_up_text);
    // addButtonText(button_down, button_down_text);
    // Draw the image
    // img.copyTo(canvas(Rect(0, button.height, img.cols, img.rows)));

    // Setup callback function
    namedWindow(winName);
    setMouseCallback(winName, callBackFunc);

    imshow(winName, canvas);
    cout << "press ctrl-c or q or Esc to quit the program." << endl;
    while(ros::ok()){
        int k = waitKey(100);
        // cout << k << endl;
        if (k == 262243 || k == 113 || k == 27)// ctrl-c or q
            break;

        if (k == 65361)
        {
            cout << "left Clicked!" << endl;
            canvas(button_left) = Vec3b(dark,dark,dark);
            addButtonText(button_left, button_left_text);
            steer = steer - steer_step;

        }
        else if (k == 65363)
        {
            cout << "right Clicked!" << endl;
            canvas(button_right) = Vec3b(dark,dark,dark);
            addButtonText(button_right, button_right_text);
            steer = steer + steer_step;
        }
        else if (k == 65362)
        {
            cout << "up Clicked!" << endl;
            canvas(button_up) = Vec3b(dark,dark,dark);
            addButtonText(button_up, button_up_text);
        }
        else if (k == 65364)
        {
            cout << "down Clicked!" << endl;
            canvas(button_down) = Vec3b(dark,dark,dark);
            addButtonText(button_down, button_down_text);
        }
        else if (k == 10){
            cout << "stop!" << endl;
        }
        else{
            if (steer < 0){
                steer = steer + 4*steer_step;
                if (steer > 0){
                    steer = 0;
                }
            }
            if (steer > 0){
                steer = steer - 4*steer_step;
                if (steer < 0){
                    steer = 0;
                }
            }
            refreshButtons(200, 200, 200);
        }
        s_s = "s: " + to_string(steer);
        servoMsg.data = steer + SERVOMID;
        servo_pub.publish(servoMsg);

        canvas(info) = Vec3b(0,0,0);
        putText(canvas(info), s_v, Point(info.width/16, info.height/8), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_i, Point(info.width/16, info.height/8 + info.height/4), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_r, Point(info.width/16, info.height/8 + info.height/2), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_s, Point(info.width/16, info.height/8 + 3*info.height/4), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        ros::spinOnce();
        imshow(winName, canvas);
    }
    

    return 0;
}