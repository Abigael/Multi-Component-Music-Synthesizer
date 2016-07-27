// Base class for effects
// author: Glen Simon

#pragma once

#include "Instrument.h"
#include <vector>

class CEffects : public CInstrument
{
public:
	virtual ~CEffects();

	void SetDelay(double delay) { mDelay = delay; }

	void SetWet(double wet) { mWet = wet; }

	void SetDry(double dry) { mDry = dry; }

	virtual void SetNote(CNote *note) override;

	virtual void Process(double *frameIn, double *frameOut, double time) = 0;


protected:
	CEffects();

	double mDelay;
	double mWet;
	double mDry;
	int mWrloc;
	int mRdloc;
	double mTime = 5.0;
	std::vector<short> mQueue;

};

