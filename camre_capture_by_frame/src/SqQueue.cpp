#include "SqQueue.hpp"
using namespace std;
using namespace cv;
/*������ʵ��ѭ������
(1)����һ��־λ����������ǡ��ա����ǡ�����
(2)������һ�ռ䣬Լ��������ͷָ���ڶ�βָ�����һλ�á�����Ϊ���гʡ�����״̬�ı�־
*/

SqQueue::SqQueue(int n)
{
	base = (Mat*)new Mat[n];
	assert(base != NULL);
	//memset(base, 0, n*sizeof(Mat));
	front = rear = 0; //��ͷ�Ͷ�βָ��ֱ�ָ���һ��Ԫ��
	maxsize = n;
	step = false;
}

bool SqQueue::EnterQueue(Mat &element)
{

	if (step) //�ж϶����Ƿ���
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
	if (front == rear)//�ж϶����Ƿ�Ϊ��
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
