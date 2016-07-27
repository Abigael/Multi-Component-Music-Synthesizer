#pragma once
#include "AudioNote.h"
class CAR :
	public CAudioNote
{
public:
	CAR();
	virtual ~CAR();
	virtual void Start();
	virtual bool Generate();
	void SetSource(CAudioNote *source){ m_source = source; }
	void SetDuration(double dur){ m_duration = dur; } //might not need
private: 
	double m_attack;
	double m_release;
	double m_duration;
	double m_time;
	CAudioNote * m_source; 
};

