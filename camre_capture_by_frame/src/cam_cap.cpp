#include "cam_cap.hpp"

using namespace cv;
using namespace std;


 /*
* argv[1] ץȡ��ͼƬ����ĵ�ַ
* argv[2] ���������ץһ֡
* argv[3] �ֱ���width
* argv[4] �ֱ���height
* argv[5] ���ڶ����ڴ��ʱ��ʼɾ����������ݣ����̣߳�
* argv[6] ���ھ������ڴ��ʱ��ʼ���ɾ�����������
* argv[7] �������˶���ⳬ����������Ϊ���˶����壨����Ϊ1��
* argv[8] ��⵽�˶���ᱣ��һ����Ƶ�������˶�֮ǰ֮�����֡
* argv[9] ������˶���Ƶ��߲�����������
* argv[10] �����ͼƬ��ʽjpg/bmp/png
 */
int main(int argc, char* argv[])
{
	cout << "\033[36m" << "---------------------------welcome--------------------------" << endl << endl;
	bool first = true;
	fstream ioFileSaved;
	fstream ioFileAll;
	fstream ioFileWestaData;
	fstream ioFileMoveDetect;
	struct thread_data td;
	//vector<string>  dirVectorSaved;
	unsigned long long memFreeSize = 10000000;
	struct statfs diskStatfs;
	unsigned int freeBlocks = 5 * 1024;  //GB
	unsigned long long freeSize;
	int westdatapercent = 0;
	bool isCatchMove = false;
	int pushToAVICntRear = 0;
	//unsigned long long totalBlocks = diskStatfs.f_blocks;
	//unsigned long long totalSize = totalBlocks * diskStatfs.f_bsize;
	//cout << log_Time() << "Total Size: " << (totalSize >> 20) << " MB" << endl;
	std::stringstream dirsSon, dirsDay;
	std::stringstream imgFullName,videoFullName;
	char addr[100];
	char DOX[100];
	strcpy(addr, argv[1]);
	int CAPFPS = (int)atoi(argv[2]);
	int WID = (int)atoi(argv[3]);
	int HGT = (int)atoi(argv[4]);
	int SAFEMEMORY = (int)atoi(argv[5]);
	int MEMBOTTUN = (int)atoi(argv[6]);
	double PERCENT = (double)atof(argv[7]);
	int MOVEBUFFCNT = (int)atoi(argv[8]);
	int UPMINUS = (int)atoi(argv[9]);
	strcpy(DOX, argv[10]);

	cout << addr << "--" << CAPFPS << "--" << WID << "--" << HGT << "--" << SAFEMEMORY << "--" << MEMBOTTUN << "--" << PERCENT << endl;
	cout << MOVEBUFFCNT << "--" << UPMINUS << "--" << DOX << endl;
	int fileDay = -1;
	int saveBuffImgCnt = 0;
	system("sh ../sh/watchp.sh");
	string savedDirectorsList;
	string allDirectorsList;
	string westdataMemPecentFile;
	string moveDetectFile;
	savedDirectorsList = addr;
	allDirectorsList = addr;
	savedDirectorsList.append("savedDirectorsList.txt"); //��Ŀ¼�д�õ��ļ���push�����Ա㳬�ڴ��ɾ��
	allDirectorsList.append("allDirectorsList.txt"); //���α���ļ�¼
	westdataMemPecentFile.append("../log/percent.txt");

	moveDetectFile.append("../log/movePercent.txt");
	ioFileSaved.open(savedDirectorsList.c_str(), ios::app|ios::out|ios::in);
	ioFileAll.open(allDirectorsList.c_str(), ios::app|ios::out|ios::in);
	ioFileWestaData.open(westdataMemPecentFile.c_str(), ios::app | ios::out | ios::in);
	ioFileMoveDetect.open(moveDetectFile.c_str(), ios::app | ios::out | ios::in);
	ioFileSaved.seekg(ios::beg);
	ioFileAll.seekg(ios::beg);
	ioFileWestaData.seekg(ios::beg);

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
			break;
		}
		//dirVectorSaved.push_back(strbufs);
    }

	string strbufs;
	getline(ioFileWestaData, strbufs);
	westdatapercent = atoi(strbufs.c_str());
	freeSize = (unsigned long long)((double)freeBlocks * (100 - westdatapercent)/100.0);

	cout << "\033[36m" << log_Time() << "westdata memory all:" << freeBlocks << "GB" << endl;
	cout << "\033[36m" << log_Time() << "read westdata memory percent:" << westdatapercent << "%  " << "remaining: " << freeSize << "GB" << endl;
	cout << "\033[36m" << log_Time() << "save img to " << addr << "captrue gap " << CAPFPS << endl;

	//VideoCapture video;
	VideoCapture video(0);
	video.set(CV_CAP_PROP_FRAME_WIDTH, WID);
	video.set(CV_CAP_PROP_FRAME_HEIGHT, HGT);
	VideoWriter outputVideo;
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
	Mat frame_cur, frame_last;

	SqQueue FRAMESQ(MOVEBUFFCNT);
	cout << "\033[36m" << "SqQueue create successed!" << endl;
	char bufSec[100];
	//int cnnt = 0;
	//videoFullName << "/home/pi/work/ttt.avi";
	//Size sWH = Size(WID, HGT);
	//outputVideo.open(videoFullName.str(), CV_FOURCC('M', 'P', '4', '2'), 10, sWH);
	while(true)
	{
		time_t tt;
		time(&tt);
		tm* t;
		video >> frame_cur;

		if(!frame_cur.empty())
		{

			isCatchMove = false;
			if (first)
			{
				frame_last = frame_cur.clone();
				for (int i = 0; i < MOVEBUFFCNT; i++)
					FRAMESQ.EnterQueue(frame_cur);
				first = !first;
				cout << "\033[36m" << "SqQueue init successed!" << endl;
				continue;
			}

			    FRAMESQ.EnterQueue(frame_cur);


			//if (tt - lastTime >= CAPFPS)
			{
				lastTime = tt;
				tt = tt + 8 * 3600;  // transform the time zone
				t = gmtime(&tt);
		
				if (fileDay == -1)
				{//���촴�����ļ��� ��ʼ��
					fileDay = t->tm_mday;
					char bufDay[100];
					memset(bufDay, 0, 100);
					getTimesDay(bufDay,t);

					dirsDay.clear();
					dirsDay.str("");
					dirsDay << addr << bufDay << "/";
					int ret = mkdir(dirsDay.str().c_str(), 0775);//�����ļ���  
					cout <<"\033[95m"<< log_Time() << "creat day dir " << dirsDay.str() << endl;
					if (ret < 0)
					{
						cout << "\033[31m" << log_Time() << "Could not create directory " << dirsDay.str() << "ret " << ret << endl;
						//return EXIT_FAILURE;
					}
					//dirVectorSaved.push_back(dirsDay.str());
					ioFileAll<< dirsDay.str()<<endl;
					//cout << "\033[35m" << log_Time() << "write " << dirsDay.str() << "to " << allDirectorsList.c_str() << endl;
				}
				

				//create directorys by time
				if (fileDay != t->tm_mday)
				{//���촴�����ļ���
                    fileDay = t->tm_mday;
					dirsDay.clear();
					dirsDay.str("");
					char bufDay[100];
					memset(bufDay, 0, 100);
					getTimesDay(bufDay,t);
					
					dirsDay << addr << bufDay << "/";

					int ret = mkdir(dirsDay.str().c_str(), 0775);//�����ļ���  
					cout << "\033[95m" << log_Time() << "creat day dir " << dirsDay.str() << endl;
					if (ret < 0)
					{
						cout << "\033[31m" << log_Time() << "Could not create directory " << dirsDay.str() << "ret " << ret << endl;
						//return EXIT_FAILURE;
					}
					//dirVectorSaved.push_back(dirsDay.str());
					ioFileAll<< dirsDay.str()<<endl;
					//cout << "\033[35m" << log_Time() << "write " << dirsDay.str() << "to " << allDirectorsList.c_str() << endl;
				}
				
				memset(bufSec, 0, 100);
				getTimesSec(bufSec,t);
#ifdef SAVE_ALL_IMG
				imgFullName.clear();
				imgFullName.str("");
				imgFullName << dirsDay.str() << bufSec << "."<< DOX;

				//system("sh ../sh/watchp.sh");
				//string strbuftmp;
				//getline(ioFileWestaData, strbuftmp);
				//westdatapercent = atoi(strbuftmp.c_str());
				//freeSize = (unsigned long long)((double)freeBlocks * (100 - westdatapercent) / 100.0);
				//memFreeSize = freeSize;
#endif
				double whitePercent = moveDetect(frame_cur, frame_last, WID, HGT);
				
				if (whitePercent >= PERCENT)
				{
					ioFileMoveDetect << bufSec << "--" << whitePercent << endl;
					isCatchMove = true;
#ifdef SAVE_ALL_IMG
					imwrite(imgFullName.str().c_str(), frame_cur);

					cout << "\033[33m" << log_Time() << "save img: " << imgFullName.str() << endl;
					//cout << "\033[32m" << log_Time() << "Free Size: " << memFreeSize << " GB" << endl;
					system("sh ../sh/watcha.sh");
#endif
				}
				

			}
			//after some times new find move 
			if (pushToAVICntRear == 0 && isCatchMove)
			{
				saveBuffImgCnt = 0;
				{ //�������ļ�����������ļ���
					dirsSon.clear();
					dirsSon.str("");
					memset(bufSec, 0, 100);
					getTimesSec(bufSec, t);

					dirsSon << dirsDay.str() << bufSec << "/";

					int ret = mkdir(dirsSon.str().c_str(), 0775);//�����ļ���  
					cout << "\033[95m" << log_Time() << "creat sec dir " << dirsSon.str() << endl;
					if (ret < 0)
					{
						cout << "\033[31m" << log_Time() << "Could not create directory " << dirsSon.str() <<endl;

					}
					//dirVectorSaved.push_back(dirsSon.str());
					ioFileAll << dirsSon.str() << endl;
					//cout << "\033[35m" << log_Time() << "write " << dirsSon.str() << "to " << allDirectorsList.c_str() << endl;
				}
				memset(bufSec, 0, 100);
				getTimesSec(bufSec, t);
				for (int i = FRAMESQ.front; i < MOVEBUFFCNT; i++)
				{
					imgFullName.clear();
					imgFullName.str("");
					imgFullName << dirsSon.str() << bufSec << "_" << saveBuffImgCnt++ << "." << DOX;
					imwrite(imgFullName.str().c_str(), FRAMESQ.base[i]);
					cout << "\033[33m" << log_Time() << "save img: " << imgFullName.str() << endl;
					//system("sh ../sh/watcha.sh");
				}
				for (int i = 0; i < FRAMESQ.front; i++)
				{
					imgFullName.clear();
					imgFullName.str("");
					imgFullName << dirsSon.str() << bufSec << "_" << saveBuffImgCnt++ << "." << DOX;
					imwrite(imgFullName.str().c_str(), FRAMESQ.base[i]);
					cout << "\033[33m" << log_Time() << "save img: " << imgFullName.str() << endl;
					//system("sh ../sh/watcha.sh");
				}

				cout << "\033[33m" << log_Time() << "save pre "<< MOVEBUFFCNT <<"frames successed! " << endl;
			}

			if (isCatchMove)
			{
				pushToAVICntRear = MOVEBUFFCNT;
				cout << "\033[33m" << log_Time() << "move find ! record next : " << pushToAVICntRear << " frames" << endl;
			}

			if (pushToAVICntRear != 0)
			{
				memset(bufSec, 0, 100);
				getTimesSecf(bufSec, t);
				imgFullName.clear();
				imgFullName.str("");
				imgFullName << dirsSon.str() << bufSec << "." << DOX;
				imwrite(imgFullName.str().c_str(), frame_cur);
				
				pushToAVICntRear--;
				if (pushToAVICntRear == 0)
				{
					cout << "\033[33m" << log_Time() << "save move frames finish!: " << imgFullName.str() << endl;
				}	
				else
					cout << "\033[33m" << log_Time() << "save move frames: " << imgFullName.str() << endl;
			}
			
			frame_last = frame_cur.clone();
		}
		//else{
		//	cout << "\033[91m" << log_Time() << "[warnning] camera time out !! call camera firstadd!!"<< endl;
		//	system("sh ../timeout.sh");
		//}
 
		if (waitKey(30) > 0) break;
	}

 
	ioFileAll.close();
    return 0;
}
