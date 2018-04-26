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

double ave(vector<double> &vec){
	double result = 0;
	for(size_t i = 0; i < vec.size(); i++){
		result += vec[i];
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
		A << 0.918574963163329,	-0.0564036736735143,
			 0.0564036736735143,	0.998341803440079;
		B << 0.0797668402767503,
			 0.00234504406412091;
		C << 0.0199417100691876,	0.706520520170517;
		D = 0.000829098279960579;
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


butterworth my_h_filter;
butterworth my_v_filter;

void lidarCallback(const std_msgs::UInt16::ConstPtr& msg)
{
	t_prev = t_now;
	t_now = ros::Time::now().toSec() - t_begin;
	h_prev = h_now;
	h_now = (msg->data - STARTRANGE)/100.0;
	h_now = my_h_filter.filt(h_now);
	v = (h_now - h_prev)/(t_now - t_prev);
	v_filtered = my_v_filter.filt(v);
	lidarMsg.h = h_now;
	lidarMsg.hdot = v_filtered;
	cout << t_now << "; " << h_now << "; " << h_filtered << "; " << v_filtered << endl;
	lidar_pub.publish(lidarMsg);
    ros::spinOnce();
}



int main(int argc, char **argv)
{

	ros::init(argc, argv, "lidar_node");

	ros::NodeHandle n;

	lidar_pub = n.advertise<um_acc::Lidar_data>("Lidar_data", 1);
	lidar_sub = n.subscribe("lidar", 1, lidarCallback);

	t_begin = ros::Time::now().toSec();

	cout << "start" << endl;

	while (ros::ok())
	{
		ros::spinOnce();
	}

	cout << "Stopped" << endl;
	return 0;
}