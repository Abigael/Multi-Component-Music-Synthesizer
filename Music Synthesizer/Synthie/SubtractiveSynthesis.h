/**
* Aurthor: Alex R. Zajac
*
* file: SubtractiveSynthesis.h
*
* Header file for subtractive synthesis
*/
#pragma once
#include "Instrument.h"
#include "AudioNote.h"
#include <string>

using namespace std;

class CSubtractiveSynthesis : public CInstrument

{
public:
	CSubtractiveSynthesis();
	virtual ~CSubtractiveSynthesis();
	virtual void Start();
	virtual bool Generate();
	virtual void SetNote(CNote *note);
	void GenerateWaveTable();
private:
	double m_time;
	double m_attack;
	double m_decay;
	double m_sustain;
	double m_release;
	double m_duration;
	double m_freq;
	double m_reson;
	double m_gain;
	double m_bandwidth;
	double m_sweep;
	double m_amp;
	string m_wave;
	int m_rindex;
	int m_windex;
	short *m_audio;
};

