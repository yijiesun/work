#include "opencv2/core.hpp"
#include <opencv2/video.hpp>
#include "opencv2/opencv.hpp"  
//#include "opencv2/videoio.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	VideoCapture video(0);
	video.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	video.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	//video.open("../test1.avi");
    std::cout<<"\033[33mHello \033[34mworld!\033[0m"<<std::endl;
	if(!video.isOpened())
	{
		cout << "open video file failed. " << endl;
		return -1;
	}

	while(true)
	{
		Mat frame;
		video >> frame;
		if(!frame.empty())
		{
			imshow("video", frame);

		}
 
		if(waitKey(30) > 0)
		{
			break;
		}
	}

    return 0;
}

