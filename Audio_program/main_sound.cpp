#include "gen_wave.h"

#include <Windows.h>

using namespace std;

int main()
{

	ALCdevice* device;
	ALCcontext* context;
	al_init(device, context);

	unsigned int bufferid;
	
	
	unsigned int sourceid;
	alGenSources(1, &sourceid);


	/*
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
	
	setlocale(LC_ALL, "RUSSIAN");
	
	al_buf_init(bufferid, 2000, 10000, 10);
	alSourcei(sourceid, AL_BUFFER, bufferid);


	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
	alSourcePlay(sourceid);
	cout << "Движение слева направо: " << endl;
	for (float p{ -10 }; p <= 10; p += 0.1)
	{
		alSource3f(sourceid, AL_POSITION, p, 0, 0);
		Sleep(50);
		if (abs(round(p) - p) <= 0.00001)
			cout << p << " ";
	}
	cout << endl;

	Sleep(500);

	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
	alSourcePlay(sourceid);
	cout << "Движение снизу вверх: " << endl;
	for (float p{ -10 }; p <= 10; p += 0.1)
	{
		alSource3f(sourceid, AL_POSITION, 0, 0, p);
		Sleep(50);
		if (abs(round(p) - p) <= 0.00001)
			cout << p << " ";
	}
	cout << endl;

	Sleep(500);

	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
	alSourcePlay(sourceid);
	cout << "Движение сзади вперёд: " << endl;
	for (float p{ -10 }; p <= 10; p += 0.1)
	{
		alSource3f(sourceid, AL_POSITION, 0, p, 0);
		Sleep(50);
		if (abs(round(p) - p) <= 0.00001)
			cout << p << " ";
	}
	cout << endl;
	
	

	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);

	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}