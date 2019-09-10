#include "SqQueue.hpp"
using namespace std;
using namespace cv;
/*用数组实现循环队列
(1)、设一标志位以区别队列是“空”还是“满”
(2)、少用一空间，约定“队列头指针在队尾指针的下一位置”上作为队列呈“满”状态的标志
*/

SqQueue::SqQueue(int n)
{
	base = (Mat*)new Mat[n];
	assert(base != NULL);
	//memset(base, 0, n*sizeof(Mat));
	front = rear = 0; //队头和队尾指针分别指向地一个元素
	maxsize = n;
	step = false;
}

bool SqQueue::EnterQueue(Mat &element)
{

	if (step) //判断队列是否满
	{
		front = (front + 1) % maxsize;
	}
	base[rear] = element.clone();
	if((rear + 1) % maxsize == front && rear != front)
		step = true;
	rear = (rear + 1) % maxsize;
	return true;
}

bool SqQueue::DeleteQueue()
{
	if (front == rear)//判断队列是否为空
	{
		return false;
	}
	else
	{
		//base[front] = 0;
		front = (front + 1) % maxsize;
		return true;
	}
}

int SqQueue::QueueLength()
{
	return (rear - front + maxsize) % maxsize;
}
