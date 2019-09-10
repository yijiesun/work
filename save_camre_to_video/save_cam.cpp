#include "opencv2/core.hpp"
//#include <opencv2/capture.hpp>
#include "opencv2/opencv.hpp"  
//#include "opencv2/videoio.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	int WID = 1920;
	int HGT = 1080;
	VideoCapture capture(0);

	if (!capture.isOpened())

	{
		cout << "未发现摄像头！" << endl;
		return -1;
	}//检测是否成功打开摄像头
	Mat frame;

	capture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));

	capture.set(CV_CAP_PROP_FPS, 30);

	capture.set(CV_CAP_PROP_FRAME_WIDTH, WID);

	capture.set(CV_CAP_PROP_FRAME_HEIGHT, HGT);
	cout << "1" << endl;
	string outputVideoPath = "../../westdata/result_temp.avi";
	Size sWH = Size(WID, HGT);
	VideoWriter outputVideo;
	//outputVideo.open(outputVideoPath, CV_FOURCC('M', 'P', '4', '2'), 25.0, sWH);
	outputVideo.open(outputVideoPath, CV_FOURCC('M', 'J', 'P', 'G'), 30.0, sWH);
	cout << "2" << endl;
	int SAVEFRAME = 200;
	//namedWindow("Image");
	int frames = 0;
	while (true)
	{
		frames++;
		Mat frame;
		capture >> frame;

		if (frames <= SAVEFRAME)
		{
			//imshow("Image", frame);
			outputVideo << frame;
			cout << frames << endl;
		}
		else
			break;

		if (waitKey(30) > 0)
		{
			break;
		}
	}
	outputVideo.release();
	return 0;
}

