//#include "gen_wave.h"
//#include <Windows.h>
#include "Video.h"
#include <fstream>

using namespace std;
  
int main()
{

	cv::Mat frame;
	string filename;
	cout << "please, enter the path of video file" << endl;
	cin >> filename;
	
	cout << "please, enter color properties of object" << endl;

	int minR, maxR, minG, maxG, minB, maxB;
	cin >> minR >>  maxR >>  minG >> maxG >> minB >> maxB;
	//filename = "adv_time/test2.mp4";

	// окно для отображения картинки
	//namedWindow("original", CV_WINDOW_AUTOSIZE);

	// получаем информацию о видео-файле
	cv::VideoCapture capture(filename);
	double fps = capture.get(CV_CAP_PROP_FPS);
	
	string datafilename;
	cout << "please, enter the name of new data file" << endl;
	cin >> datafilename;
	ofstream fout(datafilename);
	
	double X1 = 0, Z1 = 0;
	while (capture.read(frame)) {
		cv::Vec3f csz = Center_and_Size(frame, minR, maxR, minG, maxG, minB, maxB, frame.cols, (frame.cols + frame.rows) / 2.0, 11.0);
		fout << csz << " | " << Speed(fps, X1, Z1, csz[0], csz[2]) << endl;
		X1 = csz[0]; Z1 = csz[2];


	}
	return 0;
}	