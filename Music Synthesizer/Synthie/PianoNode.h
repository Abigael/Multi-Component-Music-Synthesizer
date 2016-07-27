#pragma once
#include "AudioNote.h"
#include <vector>
class CPianoNode :
	public CAudioNote
{
public:
	CPianoNode();
	virtual ~CPianoNode();

	virtual void Start();
	virtual bool Generate();

	
	void SetAmplitude(double a) {m_amp = a; }
	void SetDuration(double d) { m_duration = d * (60.0/m_bpm); }
	void SetSamples(std::vector<short> samples) { m_samples = samples; m_numsamples = samples.size(); }
	void SetPedal(bool pedal) { m_pedal=pedal; }
	void PedalNoise(bool pedalNote){ m_pedal_noise = pedalNote; }
	
private:
	std::vector<short> m_samples;
	int    m_numsamples;
	int    m_position;
	double m_amp;
	double m_duration;
	double m_time;
	bool m_pedal;
	bool m_pedal_noise;

};

