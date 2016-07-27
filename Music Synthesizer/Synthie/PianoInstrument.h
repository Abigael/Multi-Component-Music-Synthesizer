#pragma once
#include "Instrument.h"
#include "AR.h"
#include "PianoNode.h"

class CPianoInstrument :
	public CInstrument
{
public:
	CPianoInstrument();
	virtual ~CPianoInstrument();

	virtual void Start();
	virtual bool Generate();
	virtual void SetNote(CNote *note);

	void SetDuration(double d) { m_duration = d; }
	void SetPedal(bool state) { pedal_pressed = state; }
	void SetAmplitude(double amp){ m_amp = amp; }
	CPianoNode *GetPlayer(){ return &m_pianonode; }

private:
	double m_duration;
	double  m_time;
	bool pedal_pressed;
	double m_amp;
	CPianoNode m_pianonode;

};

