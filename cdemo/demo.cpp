#include "opencv2/core.hpp"
#include <opencv2/video.hpp>
#include "opencv2/opencv.hpp"  
//#include "opencv2/videoio.hpp"
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
#include <iostream>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
//#include <pthread.h>

using namespace cv;
using namespace std;

struct thread_data{
   string message;
};
string  log_Time();
void Getfilepath(const char *path, const char *filename, char *filepath);
bool DeleteFile(const char* path);

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
int main(int argc, char* argv[])
{
	cout << "\033[36m" << "---------------------------welcome--------------------------" << endl << endl;
	fstream ioFileSaved;
	fstream ioFileAll;
	struct thread_data td;
	vector<string>  dirVectorSaved;
	unsigned long long memFreeSize = 10000000;
	struct statfs diskStatfs;

	unsigned long long freeBlocks;
	unsigned long long freeSize;
	//unsigned long long totalBlocks = diskStatfs.f_blocks;
	//unsigned long long totalSize = totalBlocks * diskStatfs.f_bsize;
	//cout << log_Time() << "Total Size: " << (totalSize >> 20) << " MB" << endl;
	std::stringstream dirs;
	std::stringstream imgFullName;
	char addr[100];
	char dox[100];
	strcpy(addr, argv[1]);
	int gap = (int)atoi(argv[2]);
	int WID = (int)atoi(argv[3]);
	int HGT = (int)atoi(argv[4]);
	int safeMemry = (int)atoi(argv[5]);
	int MEMBOTTUN = (int)atoi(argv[6]);
	strcpy(dox, argv[7]);
	int fileMinute = -1;
	
	string savedDirectorsList;
	string allDirectorsList;
	savedDirectorsList = addr;
	allDirectorsList = addr;
	savedDirectorsList.append("savedDirectorsList.txt");
	allDirectorsList.append("allDirectorsList.txt");
	ioFileSaved.open(savedDirectorsList.c_str(), ios::app|ios::out|ios::in);
	ioFileAll.open(allDirectorsList.c_str(), ios::app|ios::out|ios::in);
	ioFileSaved.seekg(ios::beg);
	ioFileAll.seekg(ios::beg);
    while(!ioFileSaved.eof())
    {
		string strbufs;
        string strbuf;
        getline(ioFileSaved,strbuf);
		strbufs = addr;
		strbufs += strbuf;
		strbufs.append("/");
		cout <<"\033[36m"<< log_Time() << "read dirc from save file:" << strbufs << endl;
		if(strbuf.size() <=1 )
		{
			cout << "\033[31m" << log_Time() << "[invalid]read dirc from save file:" << strbufs << endl;
			continue;
		}
		dirVectorSaved.push_back(strbufs);
    }

	cout << "\033[36m" << log_Time() << "save img to " << addr << "captrue gap " << gap << endl;
		//VideoCapture video;
	VideoCapture video(0);
	video.set(CV_CAP_PROP_FRAME_WIDTH, WID);
	video.set(CV_CAP_PROP_FRAME_HEIGHT, HGT);
	//video.open("../test1.avi");
 
	if(!video.isOpened())
	{
		cout << "\033[41m" << log_Time() << "open video file failed. " << "\033[0m" << endl;
		return -1;
	}
	cout << "\033[36m" << log_Time() << "success open camera! " << endl << endl;
	cout << "\033[36m" << "--------------------start capture camera--------------------" << endl << endl;
	time_t lastTime;
	time(&lastTime);
		while(true)
	{
		time_t tt;
		time(&tt);

		Mat frame;
		video >> frame;
		if(!frame.empty())
		{
			//imshow("video", frame);
			if (tt - lastTime >= gap)
			{
				char buf[100];
				memset(buf, 0, 100);
				lastTime = tt;
				tt = tt + 8 * 3600;  // transform the time zone
				tm* t = gmtime(&tt);
		

				
				if (fileMinute == -1)
				{
					fileMinute = t->tm_min;
					char bufMinute[100];
					memset(bufMinute, 0, 100);
					sprintf(bufMinute, "%d-%02d-%02d-%02d-%02d",
						t->tm_year + 1900,
						t->tm_mon + 1,
						t->tm_mday,
						t->tm_hour,
						t->tm_min);
					dirs.clear();
					dirs.str("");
					dirs << addr << bufMinute << "/";
					int ret = mkdir(dirs.str().c_str(), 0775);//创建文件夹  
					cout <<"\033[95m"<< log_Time() << "creat dir " << dirs.str() << endl;
					if (ret < 0)
					{
						cout << "\031[41m" << log_Time() << "Could not create directory " << dirs.str() << "ret " << ret << "\033[0m" << endl;
						//return EXIT_FAILURE;
					}
					dirVectorSaved.push_back(dirs.str());
					ioFileAll<<dirs.str()<<endl;
					cout << "\033[35m" << log_Time() << "write " << dirs.str() << "to " << allDirectorsList.c_str() << endl;
				}
			
				if (fileMinute != t->tm_min)
				{
					if(memFreeSize < MEMBOTTUN && !dirVectorSaved.empty())
					{
						while(memFreeSize < MEMBOTTUN && !dirVectorSaved.empty())
						{
							statfs("/", &diskStatfs);
							freeBlocks = diskStatfs.f_bfree;
							freeSize = freeBlocks * diskStatfs.f_bsize;
							memFreeSize = freeSize >> 20;
							string tmp = dirVectorSaved.front();
							DeleteFile(tmp.c_str());
							rmdir(tmp.c_str());
							vector<string>::iterator k = dirVectorSaved.begin();
							dirVectorSaved.erase(k);
							cout << "\033[91m" << log_Time() << "[warnning] mem is small than 954MB !! memery size:" << memFreeSize << endl;
							cout << "\033[91m" << log_Time() << "delete " << tmp << " remain " << dirVectorSaved.size() << " directories" << endl;
						}
					}

					if (memFreeSize < safeMemry && !dirVectorSaved.empty())
					{
						pthread_t threads;
						int rc;
						
						string tmp = dirVectorSaved.front();
						//DeleteFile(tmp.c_str());
						vector<string>::iterator k = dirVectorSaved.begin();
						dirVectorSaved.erase(k);
						cout << "\033[31m" << log_Time() << "[warnning] mem full !! memery size:" << memFreeSize << endl;
						cout << "\033[31m" << log_Time() << "delete " << tmp << " remain " << dirVectorSaved.size() << " directories" << endl;
						td.message = tmp;
						rc = pthread_create(&threads, NULL, DeleteDir, (void *)&td);
						if (rc){
							cout << "\031[41m" << log_Time() << "Error:unable to create thread," << rc << "\033[0m" << endl;
							pthread_exit(NULL);
							pthread_join(threads, NULL);
							//exit(-1);
						}
						//pthread_join(threads, NULL);
					}

					dirs.clear();
					dirs.str("");
					fileMinute = t->tm_min;
					char bufMinute[100];
					memset(bufMinute, 0, 100);
					sprintf(bufMinute, "%d-%02d-%02d-%02d-%02d",
						t->tm_year + 1900,
						t->tm_mon + 1,
						t->tm_mday,
						t->tm_hour,
						t->tm_min);
					
					dirs << addr << bufMinute << "/";
			
					int ret = mkdir(dirs.str().c_str(), 0775);//创建文件夹  
					
					cout << "\033[95m" << log_Time() << "creat dir " << dirs.str() << endl;
				
					if (ret < 0)
					{
						cout << "\031[41m" << log_Time() << "Could not create directory " << dirs.str() << "\033[0m" << endl;
						return 1;
					}
					
					dirVectorSaved.push_back(dirs.str());
					ioFileAll<<dirs.str()<<endl;
					cout << "\033[35m" << log_Time() << "write " << dirs.str() << "to " << allDirectorsList.c_str() << endl;
				
				}

				sprintf(buf, "%d-%02d-%02d-%02d-%02d-%02d",
					t->tm_year + 1900,
					t->tm_mon + 1,
					t->tm_mday,
					t->tm_hour,
					t->tm_min,
					t->tm_sec);
				imgFullName.clear();
				imgFullName.str("");
				imgFullName << dirs.str() << buf << "."<< dox;

				cout << "\033[33m" << log_Time() << "save img: " << imgFullName.str() << endl;
				statfs("/", &diskStatfs);
				freeBlocks = diskStatfs.f_bfree;
				freeSize = freeBlocks * diskStatfs.f_bsize;
				cout << "\033[32m" << log_Time() << "Free Size: " << (freeSize >> 20) << " MB" << endl;
				memFreeSize = freeSize >> 20;

				imwrite(imgFullName.str().c_str(), frame);

			}
		}
 
		if(waitKey(30) > 0)
		{
			break;
		}
	}
    return 0;
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