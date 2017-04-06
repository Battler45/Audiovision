#pragma once
#ifndef GEN_WAVE_H

#include <iostream>

#include <al.h>
#include <alc.h>

namespace little_endian_io
{
	template <typename Word>
	std::ostream& write_word(std::ostream& outs, Word value, unsigned size = sizeof(Word))
	{
		for (; size; --size, value >>= 8)
			outs.put(static_cast <char> (value & 0xFF));
		return outs;
	}
}


void gen_wave(const char * filename, double frequency, double max_amplitude, double seconds);

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);

char * gen_sound(int& chan, int& samplerate, int& bps, int& size, double frequency,
				 double max_amplitude, double seconds = 1);

//Определение звуковой карты, создание контекста, установка созданного контекста текущим
void al_init(ALCdevice* & device, ALCcontext* & context);

//Создать буфер, загрузить в него данные
void al_buf_init(unsigned int & bufferid, double frequency, double amplitude, 
				 double seconds = 1);

//Создать буфер на основе расстояния до объекта и его скорости
void al_buf_init_z_v(unsigned int & bufferid, double distance,
					 double velocity, double seconds);

#endif // !GEN_WAVE_H
