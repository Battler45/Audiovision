#include "gen_wave.h"

#include <cmath>
#include <fstream>

using namespace little_endian_io;

void gen_wave(const char * filename, double frequency, double max_amplitude, double seconds)
{

	std::ofstream f(filename, std::ios::binary);

	int samples_per_second = 44100;
	int bits_per_sample = 16;
	int channels = 1;

	// Write the file headers
	f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
	write_word(f, 16, 4);  // no extension data
	write_word(f, 1, 2);  // PCM - integer samples
	write_word(f, channels, 2);  // two channels (stereo file)
	write_word(f, samples_per_second, 4);  // samples per second (Hz)
	write_word(f, samples_per_second * bits_per_sample * channels / 8, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
	write_word(f, channels * 2, 2);  // data block size (size of two integer samples, one for each channel, in bytes)
	write_word(f, bits_per_sample, 2);  // number of bits per sample (use a multiple of 8)

										// Write the data chunk header
	size_t data_chunk_pos = f.tellp();
	f << "data----";  // (chunk size to be filled in later)

					  // Write the audio samples
					  // (We'll generate a single C4 note with a sine wave, fading from left to right)
	constexpr double two_pi = 6.283185307179586476925286766559;
	//constexpr double max_amplitude = 32760;  // "volume"

	double hz = samples_per_second;    // samples per second
	//double frequency = 1561.626;  // middle C = 261.626
	//double seconds = 10.0;      // time

	int N = hz * seconds;  // total number of samples
	for (int n = 0; n < N; ++n)
	{
		//double amplitude = (double)n / N * max_amplitude;
		double amplitude = max_amplitude;
		double value = sin((two_pi * n * frequency) / hz);
		write_word(f, (int)(amplitude  * value), 2);
		//write_word(f, (int)((max_amplitude - amplitude) * value), 2);
	}

	// (We'll need the final file size to fix the chunk sizes above)
	size_t file_length = f.tellp();

	// Fix the data chunk header to contain the data size
	f.seekp(data_chunk_pos + 4);
	write_word(f, file_length - data_chunk_pos + 8);

	// Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
	f.seekp(0 + 4);
	write_word(f, file_length - 8, 4);
}

bool isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int convertToInt(char* buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i<len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i<len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "this is not a valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}


char * gen_sound(int& chan, int& samplerate, int& bps, int& size, double frequency,
				 double max_amplitude, double seconds)
{
	chan = 1;
	samplerate = 44100;
	bps = 16;

	size = int(samplerate * seconds * 2 + 16);
	char* data = new char[size];


	constexpr double two_pi = 6.283185307179586476925286766559;
	//constexpr double max_amplitude = 32760;  // "volume"

	double hz = samplerate;    // samples per second
	int N = hz * seconds;  // total number of samples
	for (int n = 0; n < N; ++n)
	{
		//double amplitude = (double)n / N * max_amplitude;
		double amplitude = max_amplitude;
		double value = sin((two_pi * n * frequency) / hz);

		//write_word(f, (int)(amplitude  * value), 2);
		int v = (int)(amplitude * value);
		for (unsigned int sz = 2; sz ; --sz, v >>= 8)
			data[n*2 + 2 - sz] = static_cast<char>(v & 0xFF);

		//write_word(f, (int)((max_amplitude - amplitude) * value), 2);
	}

	return data;
}

//Генерация звука для 2 каналов
char * gen_sound_2channels(int& chan, int& samplerate, int& bps, int& size, double frequency,
	double max_amplitude, double difference_amplitude, double seconds)
{
	chan = 2;
	samplerate = 44100;
	bps = 16;

	size = int(samplerate * seconds * 4 + 8);
	char* data = new char[size];


	constexpr double two_pi = 6.283185307179586476925286766559;
	//constexpr double max_amplitude = 32760;  // "volume"

	double hz = samplerate;    // samples per second
	int N = hz * seconds;  // total number of samples
	for (int n = 0; n < N; n++)
	{
		//double amplitude = (double)n / N * max_amplitude;
		double amplitude = max_amplitude;
		double value = sin((two_pi * n * frequency) / hz);

		//write_word(f, (int)(amplitude  * value), 2);
		int v = (int)(amplitude  * value);
		for (unsigned int sz = 2; sz; --sz, v >>= 8)
			data[n * 4 + 2 - sz] = static_cast<char>(v & 0xFF);

		//write_word(f, (int)((max_amplitude - amplitude) * value), 2);
		v = (int)((amplitude - difference_amplitude)  * value);
		for (unsigned int sz = 2; sz; --sz, v >>= 8)
			data[n * 4 + 4 - sz] = static_cast<char>(v & 0xFF);
	}

	return data;
}

//Определение звуковой карты, создание контекста, установка созданного контекста текущим
void al_init(ALCdevice* & device, ALCcontext* & context)
{
	device = alcOpenDevice(NULL);
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
		return;
	}
	context = alcCreateContext(device, NULL);
	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
		return;
	}
	alcMakeContextCurrent(context);
}

//Создать буфер, загрузить в него данные
void al_buf_init(unsigned int & bufferid, double frequency, double amplitude, 
				 double seconds)
{
	int channel, sampleRate, bps, size;
	char * data = gen_sound(channel, sampleRate, bps, size, frequency, amplitude, seconds);
	
	alGenBuffers(1, &bufferid);
	unsigned int format;

	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}

	alBufferData(bufferid, format, data, size, sampleRate);
	delete[] data;
}

//Создать буфер, загрузить в него данные
void al_buf_init_2chan(unsigned int & bufferid, double frequency, double amplitude, 
	double difference_amplitude, double seconds)
{
	int channel, sampleRate, bps, size;
	char* data = gen_sound_2channels(channel, sampleRate, bps, size, frequency, amplitude, 
									 difference_amplitude, seconds);

	alGenBuffers(1, &bufferid);
	unsigned int format;

	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}

	alBufferData(bufferid, format, data, size, sampleRate);
	delete[] data;
}

//Создать буфер на основе расстояния до объекта и его скорости
void al_buf_init_z_v(unsigned int & bufferid, double distance, 
					 double velocity, double seconds)
{
	double dist = 0.2;  // начальное расстояние в метрах
	double vol = 5000;  // начальная громкость

					   //Скорость в км/ч
	double speed_min = 0;
	double speed_max = 800;

	//Частотный диапазон в герцах
	double hz_min = 1000;
	double hz_max = 2000;

	//Амплитуда
	double amplitude = vol - 10*(distance / dist);

	//Частота
	double frequency = hz_min + (hz_max - hz_min) / (speed_max - speed_min) * velocity;

	al_buf_init(bufferid, frequency, amplitude, seconds);
}

//найти смещение в градусах
double degree(double x, double z)
{
	double tn = std::tan(z / x);
	return 90 - std::atan(tn);
}

//Создать буфер на основе расстояния до объекта, его кооринаты X и его скорости
void al_buf_init_z_v_x(unsigned int & bufferid, double distance,
	double velocity, double x, double seconds)
{
	double dist = 0.2;  // начальное расстояние в метрах
	double vol = 1000;  // начальная громкость

						//Скорость в км/ч
	double speed_min = 0;
	double speed_max = 800;

	//Частотный диапазон в герцах
	double hz_min = 1000;
	double hz_max = 2000;

	//Амплитуда
	double amplitude = vol - 10 * log2(distance / dist);

	//Частота
	double frequency = hz_min + (hz_max - hz_min) / (speed_max - speed_min) * velocity;

	//Разница амплитуды
	double deg = degree(x, distance);
	int new_x = 1;
	if (deg > 45)
		++new_x;
	double diff = log2(new_x * deg) / log2(3.5);

	al_buf_init_2chan(bufferid, frequency, amplitude, diff, seconds);
}
