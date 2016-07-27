#include "StdAfx.h"
#include "AdditiveSineWave.h"
#include <iostream>
#include <cmath>



CAdditiveSineWave::CAdditiveSineWave(void)
{
	m_vibratoFrequency = 0.0;
	m_vibratoRate = 0.0;
	m_amplitude = 1000.0;
	m_amp[0] = 1.0;
	for (int i = 1; i <= 7; i++) 
	{
		m_amp[i] = 0.0;
	}
	m_frequency = 440.0;

}


CAdditiveSineWave::~CAdditiveSineWave(void)
{
	delete[] audio;
	audio = NULL;
}

void CAdditiveSineWave::Start()
{
	audio = new short[int(m_duration*GetSampleRate())];
	index = 0;
	CAdditiveSineWave::GenerateWaveTable();
}

bool CAdditiveSineWave::Generate()
{
	m_frame[1] = m_frame[0] = audio[index++];

	return true;
}

short CAdditiveSineWave::RangeBound(double d)
{
	if (d < -32768)
		return -32768;
	else if (d > 32767)
		return 32767;

	return (short) d;
}

void CAdditiveSineWave::GenerateWaveTable()
{

	double sineRadians = 0; // For the sine wave
	double vibratoRadians = 0; // For the vibrato wave
	double time = 0; 

	for (int i = 0; i < int(m_duration*GetSampleRate()); i++, time += 1. / GetSampleRate())
	{

		double sample = 0;
		double freqMax = 22050;
		for (float n = 1.0; n*m_frequency <= freqMax && n <= 8.0; n++)
		{
			sample += (m_amplitude * m_amp[int(n - 1)] * (sin(n * sineRadians)));
		}

		double angularFrequency = (2 * PI*m_vibratoRate);
		angularFrequency /= GetSampleRate();
		vibratoRadians += angularFrequency;
		double modF = (m_frequency + m_vibratoFrequency * sin(vibratoRadians)); //check tihs
		double sineFrAd = (2 * PI * modF);
		sineRadians += (sineFrAd / GetSampleRate());

		audio[i] = RangeBound(sample);
	}
}

