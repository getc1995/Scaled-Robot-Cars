#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"

#include <ros/ros.h>
#include <um_acc/Motor_data.h>
#include <um_acc/IMU.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt16.h>
#include <um_acc/CentralSignal.h>
#include "Car_parameters.h"


#include <iostream>
#include <sstream>
#include <unistd.h>
#include <algorithm>

using namespace cv;
using namespace std;

#define steer_step 10
#define sleep_time 50
#define speed_step 0.05

ros::Publisher servo_pub;
ros::Publisher motor_pub;
ros::Publisher control_pub;
/*display messages*/
float v = 0;
float i = 0;
float r = 0;
int steer = 0;

/*Control Signal*/
bool manual_steer = true;
bool right_lane = true;
bool manual_v = true;
bool stop = false;
float command_v = 0.0;

/*button states*/
bool press_left = false;
bool press_right = false;
bool press_up = false;
bool press_down = false;

string s_v, s_i, s_r, s_s, s_v_command;

// Mat3b img(300, 300, Vec3b(0, 255, 0));
Mat3b canvas;
string winName = "Manual Control 1";

/*button areas*/
Rect button_left;
Rect button_right;
Rect button_up;
Rect button_down;
Rect button_stop;
Rect button_v;
Rect button_s;
Rect info;

string button_left_text("<-");
string button_right_text("->");
string button_up_text("^");
string button_down_text("v");
string button_stop_text("STOP");
string button_v_text("manual speed");
string button_s_text("manual steer");

int button_contour = 1;
int dark = 150;

void refreshButtons(int r, int g, int b);
void callBackFunc(int event, int x, int y, int flags, void* userdata);
void addButtonText(Rect &button, string &buttonText);
void IMUCallback(const um_acc::IMU::ConstPtr& msg);
void motorCallback(const um_acc::Motor_data::ConstPtr& msg);
void servoCallback(const std_msgs::UInt16::ConstPtr& msg);
int clampServo(int steerPWM);
float clampVelocity(float v);

int clampServo(int steerPWM){
    steerPWM = std::max(-SERVOMAXAMP, steerPWM);
    steerPWM = std::min(SERVOMAXAMP, steerPWM);
    return steerPWM;
}

float clampVelocity(float v){
    v = std::max(-1.0f, v);
    v = std::min(1.0f, v);
    return v;
}

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
            press_left = true;
        }
        else if (button_right.contains(Point(x, y)))
        {
            cout << "right Clicked!" << endl;
            press_right = true;
        }
        else if (button_up.contains(Point(x, y)))
        {
            cout << "up Clicked!" << endl;
            press_up = true;
        }
        else if (button_down.contains(Point(x, y)))
        {
            cout << "down Clicked!" << endl;
            press_down = true;
        }
        else if (button_stop.contains(Point(x, y)))
        {
            cout << "stop Clicked!" << endl;
            stop = !stop;
        }
        else if (button_v.contains(Point(x, y)))
        {
            cout << "manual_v Clicked!" << endl;
            manual_v = !manual_v;
        }
        else if (button_s.contains(Point(x, y)))
        {
            cout << "manual_steer Clicked!" << endl;
            manual_steer = !manual_steer;
        }
        else{
            
        }
    }
    if (event == EVENT_LBUTTONUP)
    {
        refreshButtons(200,200,200);
        press_left = false;
        press_right = false;
        press_up = false;
        press_down = false;
    }

    imshow(winName, canvas);
    waitKey(1);
}

void addButtonText(Rect &button, string &buttonText){
    putText(canvas(button), buttonText, Point(button.width*0.35, button.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0,0,0));
}

void addButtonTextControl(Rect &button, string &buttonText){
    putText(canvas(button), buttonText, Point(0, button.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));
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
    control_pub = nh.advertise<um_acc::CentralSignal>("Control_command1", 1);

    ros::Subscriber IMU_sub = nh.subscribe("IMU_data1", 2, IMUCallback);
    ros::Subscriber motor_sub = nh.subscribe("Motor_data1", 2, motorCallback);
    std_msgs::UInt16 servoMsg;
    um_acc::CentralSignal controlMsg;

    ros::Rate loop_rate(100);
    // An image
    int canvas_h = 180*2; // 750;
    int canvas_w = 320*2; // 1334

    // The canvas
    canvas = Mat3b(canvas_h, canvas_w, Vec3b(0,0,0));

    // Your button
    button_left = Rect(0, canvas_h/2, canvas_w/4, canvas_h/2);
    button_right = Rect(canvas_w/4, canvas_h/2, canvas_w/4, canvas_h/2);
    button_up = Rect(3*canvas_w/4, 0, canvas_w/4, canvas_h/2);
    button_down = Rect(3*canvas_w/4, canvas_h/2, canvas_w/4, canvas_h/2);
    button_stop = Rect(canvas_w/2, 0, canvas_w/4, canvas_h/2);
    button_v = Rect(canvas_w/2, canvas_h/2, canvas_w/4, canvas_h/4);
    button_s = Rect(canvas_w/2, 3*canvas_h/4, canvas_w/4, canvas_h/4);
    info = Rect(0, 0, canvas_w/2, canvas_h/2);

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
        int k = waitKey(sleep_time);
        // cout << k << endl;
        if (k == 262243 || k == 113 || k == 27)// ctrl-c or q
            break;

        if (k == 65361 || press_left)
        {
            cout << "left Clicked!" << endl;
            canvas(button_left) = Vec3b(dark,dark,dark);
            addButtonText(button_left, button_left_text);
            if (manual_steer){
                steer -= steer_step;
                steer = clampServo(steer);
            }
            else{
                right_lane = false;
            }
        }
        else if (k == 65363 || press_right)
        {
            cout << "right Clicked!" << endl;
            canvas(button_right) = Vec3b(dark,dark,dark);
            addButtonText(button_right, button_right_text);
            if (manual_steer){
                steer += steer_step;
                steer = clampServo(steer);
            }
            else{
                right_lane = true;
            }
        }
        else if (k == 65362 || press_up)
        {
            cout << "up Clicked!" << endl;
            canvas(button_up) = Vec3b(dark,dark,dark);
            addButtonText(button_up, button_up_text);
            if (manual_v){
                command_v += speed_step;
                command_v = clampVelocity(command_v);
            }
        }
        else if (k == 65364 || press_down)
        {
            cout << "down Clicked!" << endl;
            canvas(button_down) = Vec3b(dark,dark,dark);
            addButtonText(button_down, button_down_text);
            if (manual_v){
                command_v -= speed_step;
                command_v = clampVelocity(command_v);
            }
        }
        else if (k == 10){
            cout << "stop!" << endl;
            stop = !stop;
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
        /*update control buttons*/
        if (stop){
            canvas(button_stop) = Vec3b(dark, dark, dark);
            command_v = 0.0;
            manual_steer = true;
            manual_v = true;
        }
        else
            canvas(button_stop) = Vec3b(200, 200, 200);
        addButtonTextControl(button_stop, button_stop_text);

        if (manual_steer)
            canvas(button_s) = Vec3b(dark, dark, dark);
        else
            canvas(button_s) = Vec3b(200, 200, 200);
        addButtonTextControl(button_s, button_s_text);

        if (manual_v)
            canvas(button_v) = Vec3b(dark, dark, dark);
        else{
            canvas(button_v) = Vec3b(200, 200, 200);
            command_v = 0.0;
        }
        addButtonTextControl(button_v, button_v_text);


        /*publish messages*/
        if (manual_steer){
            servoMsg.data = steer + SERVOMID;
            servo_pub.publish(servoMsg);
        }

        controlMsg.manual_steer = manual_steer;
        controlMsg.right_lane = right_lane;
        controlMsg.manual_v = manual_v;
        controlMsg.stop = stop;
        controlMsg.command_v = command_v;

        control_pub.publish(controlMsg);

        /*update info*/
        s_s = "s: " + to_string(steer);
        s_v_command = "send v: " + to_string(command_v);
        canvas(info) = Vec3b(0,0,0);
        putText(canvas(info), s_v, Point(info.width/16, info.height/8), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_i, Point(info.width/16, info.height/8 + info.height/5), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_r, Point(info.width/16, info.height/8 + 2*info.height/5), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_s, Point(info.width/16, info.height/8 + 3*info.height/5), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        putText(canvas(info), s_v_command, Point(info.width/16, info.height/8 + 4*info.height/5), FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        ros::spinOnce();
        imshow(winName, canvas);
    }
    

    return 0;
}