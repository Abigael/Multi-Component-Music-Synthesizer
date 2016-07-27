#pragma once
#include<vector>
#include "Instrument.h"
#include "NoiseGate.h"
#include "Compression.h"
#include "Note.h"
class CRecordPrefSynth :
	public CInstrument
{
public:
	CRecordPrefSynth();
	virtual ~CRecordPrefSynth();
	virtual void SetNote(CNote* note);
	virtual void Start() ;
	virtual bool Generate();
	bool LoadFile(const char *filename);

private:
	int index = 0;
	std::vector<double> m_soundR; 
	std::vector<double> m_soundL;
	double m_duration=0; //default was 58
	double m_time;
	bool m_noiseGateRecord = false;
	bool m_compressionRecord = false;
	CNoiseGate noiseGate;
	CCompression compressor; 
};






