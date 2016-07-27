#pragma once

#include<vector>
#include "Queue.h"

class CInstrument;

class CAudioEffects
{
public:
	CAudioEffects();
	virtual ~CAudioEffects();

	void SetEffects(CAudioEffects* effect);
	void ClearEffects(void);

	bool mChorusEffect = false;
	double mCEdry = 0.0;
	double mCEwet = 0.0;

	bool mReverberatonEffect = false;
	double mREdry = 0.0;
	double mREwet = 0.0;

	bool mFlangingEffect = false;
	double mFEdry = 0.0;
	double mFEwet = 0.0;

	bool mRingEffect = false;
	double mRingEdry = 0.0;
	double mRingEwet = 0.0;

	bool mNoiseGateEffect = false;
	double m_threshold= 0.0;
	bool mCompressionEffect = false; 
	double m_left=1; 
	double m_right=1;
	double m_dry = 0.0;
	double m_wet= 0.0;

	CQueue* mQueuePtr;

	//the instrument this effect is tied to
	CInstrument* mInstrument;

	//chorus effect
	void ProcessChorus(double *input, double* output, double time);

	//Flange effect
	void ProcessFlange(double *input, double* output, double time);

	//Reverb effect
	void ProcessReverb(double *input, double* output, double time);

	//Ring effect
	void ProcessRing(double* input, double* output, double time);
	
	
	


};

