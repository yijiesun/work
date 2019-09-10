#include <iostream>
#include <string>
#include <cassert>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"  
using namespace std;
using namespace cv;
class SqQueue
{
public:
	SqQueue(int n = 10);//构造函数
	bool EnterQueue(Mat &element);//向队列插入元素
	bool DeleteQueue();//从队列弹出元素
	int QueueLength();//返回队列的长度
	int front; //对头指针
	int rear;  //队尾指针
	Mat *base; //动态分配的内存指针
	int maxsize; //最大队列长度
	bool step;
};