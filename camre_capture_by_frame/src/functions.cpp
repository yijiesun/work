#include "cam_cap.hpp"
#include <sys/time.h>
using namespace cv;
using namespace std;

void *DeleteDir(void *threadarg)
{
   pthread_detach(pthread_self());
   struct thread_data *my_data;
 
   my_data = (struct thread_data *) threadarg;

   cout << "\033[31m" << log_Time() << "DeleteDir Message : " << my_data->message << endl;
   DeleteFile(my_data->message.c_str());
   rmdir(my_data->message.c_str());
   pthread_exit(NULL);
}

double moveDetect(Mat &cur, Mat &last, int wid, int hgt)
{
	cv::Mat cur_, last_;
	cur_ = cur.clone();
	last_ = last.clone();
	cv::Mat frame_cur_gray, frame_last_gray;
	cv::Mat diff(cv::Size(wid, hgt), CV_8UC1);
	cvtColor(cur, frame_cur_gray, CV_BGR2GRAY);
	cvtColor(last, frame_last_gray, CV_BGR2GRAY);
	absdiff(frame_cur_gray, frame_last_gray, diff);
	int blockSize = 25;
	int constValue = 10;
	cv::Mat diff_thresh;
	cv::adaptiveThreshold(diff, diff_thresh, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	//medianBlur(src, dst, 7);
	//GaussianBlur(dst, dst, Size(3, 1), 0.0);
	erode(diff_thresh, diff_thresh, element);
	dilate(diff_thresh, diff_thresh, element);
	//calc white percent
	Mat diff01;
	normalize(diff_thresh, diff01, 0, 1, cv::NORM_MINMAX);
	double sum_diff_roi = countNonZero(diff01);
	double percent = sum_diff_roi / ((double)wid * hgt);

	cout << "\033[32m" << log_Time() << "move percent: " << percent << endl;

	return percent;

}

void Getfilepath(const char *path, const char *filename, char *filepath)
{
	strcpy(filepath, path);
	if (filepath[strlen(path) - 1] != '/')
		strcat(filepath, "/");
	strcat(filepath, filename);
	cout << "\33[36m" << "clear path is =" << filepath << endl;
}

bool DeleteFile(const char* path)
{
	DIR *dir;
	struct dirent *dirinfo;
	struct stat statbuf;
	char filepath[256] = { 0 };
	lstat(path, &statbuf);

	if (S_ISREG(statbuf.st_mode))//判断是否是常规文件
	{
		remove(path);
	}
	else if (S_ISDIR(statbuf.st_mode))//判断是否是目录
	{
		if ((dir = opendir(path)) == NULL)
			return 1;
		while ((dirinfo = readdir(dir)) != NULL)
		{
			Getfilepath(path, dirinfo->d_name, filepath);
			if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0)//判断是否是特殊目录
				continue;
			DeleteFile(filepath);
			rmdir(filepath);
		}
		closedir(dir);
	}
	return 0;
}

string  log_Time()
{
        struct  tm      *ptm;
        struct  timeb   stTimeb;
        static  char    szTime[21];
		string stime;
        ftime(&stTimeb);
        ptm = localtime(&stTimeb.time);
        sprintf(szTime, "%s%02d-%02d %02d:%02d:%02d.%03d%s","[",
                ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm,"]");
        szTime[20] = 0;
		stime = szTime;
        return stime;
}

void getTimesDay(char *param, tm* t)
{
	sprintf(param, "%d-%02d-%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday);
}

void getTimesMin(char *param, tm* t)
{
	sprintf(param, "%d-%02d-%02d-%02d-%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min);
}

void getTimesSec(char *param, tm* t)
{
	sprintf(param, "%d-%02d-%02d-%02d-%02d-%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
}
void getTimesSecf(char *param, tm* t)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	sprintf(param, "%d-%02d-%02d-%02d-%02d-%ld",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		tv.tv_sec * 1000 + tv.tv_usec / 1000);
}