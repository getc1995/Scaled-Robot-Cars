#include "ros/ros.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/Float32.h"
#include <um_acc/Lidar_data.h>
#include <eigen3/Eigen/Dense>
#include <vector>
#define STARTRANGE 30 //The start range of detection is 30 cm.

using namespace Eigen;
using namespace std;

ros::Publisher lidar_pub;
ros::Subscriber lidar_sub;
um_acc::Lidar_data lidarMsg;

double t_prev = 0;
double t_begin = 0;
double t_now = 0;
double h_prev = 0;
double h_now = 0;
double h_filtered = 0;
double v = 0;
double v_filtered = 0;
double h_dev = 0;

double ave(vector<double> &vec){
	double result = 0;
	for(size_t i = 0; i < vec.size(); i++){
		result += vec[i];
	}
	return result/double(vec.size()-1.0);
}

double dev(vector<double> &vec){
	double result = 0;
	for(size_t i = 1; i < vec.size(); i++){
		result += (vec[i]-vec[i-1])*(vec[i]-vec[i-1]);
	}
	return result/double(vec.size());
}

class butterworth{
private:
	//A second order butterworth filter for lidar only with a cornering frequency of 20Hz and a sampling frequency of 213hz.
	Matrix2d A;
	Matrix<double, 2, 1> B;
	Matrix<double, 1, 2> C;
	double D;
	Vector2d x;
public:
	butterworth(){
		x << 0, 0;
		A << 0.802588037598423, -0.132685224830656,
			 0.132685224830656, 0.990233282080460;
		B << 0.187645244482036,
			 0.0138122249416863;
		C << 0.0469113111205091, 0.703653724951126;
		D = 0.00488335895977018;
	}
	double filt(double input){
		x = A*x + B*input;
		return C*x + D*input;
	}
};

class avefilter{
private:
	size_t window_length;
	vector<double> v_vec;
public:
	avefilter(){
		window_length = 30;
		v_vec.reserve(window_length);
	}
	double filt(double input){
		if (v_vec.size() < window_length){
			v_vec.push_back(input);
		}
		else{
			v_vec.erase(v_vec.begin());
			v_vec.push_back(input);
		}
		return ave(v_vec);
	}
};

class deviation{
private:
	size_t window_length;
	vector<double> v_vec;
public:
	deviation(){
		window_length = 30;
		v_vec.reserve(window_length);
	}
	double getDeviation(double input){
		if (v_vec.size() < window_length){
			v_vec.push_back(input);
		}
		else{
			v_vec.erase(v_vec.begin());
			v_vec.push_back(input);
		}
		return dev(v_vec);
	}
};

butterworth my_h_filter;
butterworth my_v_filter;
deviation my_deviation;

void lidarCallback(const std_msgs::UInt16::ConstPtr& msg)
{
	t_prev = t_now;
	t_now = ros::Time::now().toSec() - t_begin;
	h_prev = h_now;
	h_now = (msg->data - STARTRANGE)/100.0;
	h_now = my_h_filter.filt(h_now);
	v = (h_now - h_prev)/(t_now - t_prev);
	v_filtered = my_v_filter.filt(v);
	h_dev = my_deviation.getDeviation(h_now);
	lidarMsg.h = h_now;
	lidarMsg.hdot = v_filtered;
	cout << t_now << "; " << h_now << "; " << h_filtered << "; " << v_filtered << " h_dev: " << h_dev << endl;
	lidar_pub.publish(lidarMsg);
    ros::spinOnce();
}



int main(int argc, char **argv)
{

	ros::init(argc, argv, "lidar_node1");

	ros::NodeHandle n;

	lidar_pub = n.advertise<um_acc::Lidar_data>("Lidar_data1", 1);
	lidar_sub = n.subscribe("lidar1", 1, lidarCallback);

	t_begin = ros::Time::now().toSec();

	cout << "start" << endl;

	while (ros::ok())
	{
		ros::spinOnce();
	}

	cout << "Stopped" << endl;
	return 0;
}