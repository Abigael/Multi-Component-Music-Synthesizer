/**
 * \file AdditiveSineWave.h
 *
 * \author Cam Merrill
 *
 * 
 */

#pragma once
#include "AudioNote.h"
class CAdditiveSineWave :
	public CAudioNote
{
private:
	double m_duration;
	double m_frequency;
	double m_amp[8];
	double m_amplitude;
	double m_vibratoRate;
	double m_vibratoFrequency;
	int index;
	short *audio;

public:
	CAdditiveSineWave();
	virtual ~CAdditiveSineWave();
	
	virtual void Start();
	virtual bool Generate();
	void SetFrequency(double f) { m_frequency = f; }
	void SetAmplitude(double &a) { memcpy(&m_amp, &a, sizeof(double) * 8); }
	void SetDuration(double d){ m_duration = d; }
	void SetVibratoRate(double d){ m_vibratoRate = d; }
	void SetVibratoFreq(double d){ m_vibratoFrequency = d; }
	short CAdditiveSineWave::RangeBound(double d);

	void GenerateWaveTable();
};

