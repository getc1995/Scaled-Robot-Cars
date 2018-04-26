#include "opencv2/opencv.hpp"
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    cap >> frame; // get a new frame from camera
    
    imwrite("picture.jpg", frame);

    fprintf(stdout, "Saved JPG file.\n");

    return 0;
}
