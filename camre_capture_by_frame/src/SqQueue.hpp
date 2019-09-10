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
	SqQueue(int n = 10);//���캯��
	bool EnterQueue(Mat &element);//����в���Ԫ��
	bool DeleteQueue();//�Ӷ��е���Ԫ��
	int QueueLength();//���ض��еĳ���
	int front; //��ͷָ��
	int rear;  //��βָ��
	Mat *base; //��̬������ڴ�ָ��
	int maxsize; //�����г���
	bool step;
};