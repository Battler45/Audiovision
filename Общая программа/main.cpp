
#include "Video.h"
#include "gen_wave.h"
#include <Windows.h>
#include <ctime>

using namespace cv;
using namespace std;
  
int main()
{
	
	ALCdevice* device;
	ALCcontext* context;
	al_init(device, context);

	unsigned int bufferid;
	unsigned int sourceid;
	alGenSources(1, &sourceid);

	cv::Mat frame;
	string filename;
	cout << "please, enter the path of video file" << endl;
	cin >> filename;

	cout << "please, enter color properties of object" << endl;
	int minR, maxR, minG, maxG, minB, maxB;
	cin >> minR >> maxR >> minG >> maxG >> minB >> maxB; //140 170 200 230 170 200

	double t;
	cout << endl << "enter time of video (in second)" << endl;
	cin >> t;

	al_buf_init(bufferid, 2000, 10000, int(t));
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
	alSourcePlay(sourceid);

	// получаем информацию о видео-файле
	cv::VideoCapture capture(filename);
	double fps = capture.get(CV_CAP_PROP_FPS);


	//double X1 = 0, Z1 = 0;
	time_t start = clock();
	while (capture.read(frame)) {
		auto csz = Center_and_Size(frame, minR, maxR, minG, maxG, minB, maxB, 200, (frame.cols + frame.rows) / 2.0, 11.0);
		alSource3f(sourceid, AL_POSITION, csz[0] / 100, csz[2] / 100, csz[1] / 100);
		Sleep(50);
		if ((double(clock() - start) / CLOCKS_PER_SEC) > (t - 0.5))
			break;
	}
	return 0;


	/* вриант программы 2 (с нашими разработанными функциями)
	double velocity = 50;
	double distance = 50;

	cout << "distance: ";
	alSourcePlay(sourceid);
	for (int d = 1; d <= 100; d += 10)
	{
	al_buf_init_z_v(bufferid, d, velocity, 1);
	alSourcePause(sourceid);
	alSourceQueueBuffers(sourceid, 1, &bufferid);
	alSourcePlay(sourceid);
	cout << d << " ";
	Sleep(1000);
	alSourceUnqueueBuffers(sourceid, 1, &bufferid);
	}
	cout << endl;
	cout << "velocity: " << endl;
	for (int v = 1; v <= 100; v += 10)
	{
	al_buf_init_z_v(bufferid, distance, v, 1);
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSourcePlay(sourceid);
	cout << v << " ";
	Sleep(1000);
	alSourceStop(sourceid);
	}
	cout << endl;
	*/


	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);

	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}	