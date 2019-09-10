#include <iostream>
#include <stdio.h>
#include <time.h>
#include "stdlib.h"
#include <sstream>
#include <fstream>
#include <string>
#include <sys/stat.h> 
#include <sys/vfs.h>
#include <sys/timeb.h>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include "opencv2/core.hpp"
#include <opencv2/video.hpp>
#include "opencv2/opencv.hpp"  
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "SqQueue.hpp"
using namespace cv;
using namespace std;

struct thread_data{
   string message;
};
string  log_Time();
void Getfilepath(const char *path, const char *filename, char *filepath);
bool DeleteFile(const char* path);

void *DeleteDir(void *threadarg);

double moveDetect(Mat &cur, Mat &last, int wid, int hgt);

void getTimesMin(char *param, tm* t);

void getTimesSec(char *param, tm* t);

void getTimesDay(char *param, tm* t);

void getTimesSecf(char *param, tm* t);