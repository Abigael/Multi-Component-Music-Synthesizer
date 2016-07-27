#include "stdafx.h"
#include "AudioEffects.h"
#include "Instrument.h"
#include <cmath>

const double M_PI = 3.14159265359;
const int MAXQUEUESIZE = 200000;

using namespace std;
CAudioEffects::CAudioEffects()
{
}


CAudioEffects::~CAudioEffects()
{
}


void CAudioEffects::SetEffects(CAudioEffects* effect)
{
	mChorusEffect = effect->mChorusEffect;
	mCEdry = effect->mCEdry;
	mCEwet = effect->mCEwet;

	mReverberatonEffect = effect->mReverberatonEffect;
	mREdry = effect->mREdry;
	mREwet = effect->mREwet;

	mFlangingEffect = effect->mFlangingEffect;
	mFEdry = effect->mFEdry;
	mFEwet = effect->mFEwet;

	mRingEffect = effect->mRingEffect;
	mRingEdry = effect->mRingEdry;
	mRingEwet = effect->mRingEwet;

	return;
}


void CAudioEffects::ClearEffects(void)
{
	mChorusEffect = false;
	mCEdry = 0.0;
	mCEwet = 0.0;

	mReverberatonEffect = false;
	mREdry = 0.0;
	mREwet = 0.0;

	mFlangingEffect = false;
	mFEdry = 0.0;
	mFEwet = 0.0;

	mRingEffect = false;
	mRingEdry = 0.0;
	mRingEwet = 0.0;

	return;
}

void CAudioEffects::ProcessChorus(double *input, double* output, double time)
{
	double delay = 2;
	double SampleRate = mInstrument->GetSampleRate();
	int delaylength = int((0.006 + sin(0.25 * 2 * M_PI * time) * 0.004) * delay * SampleRate + 0.5) * 2;
	int wrloc = mQueuePtr->mWrloc;
	int rdloc = (wrloc + MAXQUEUESIZE - delaylength) % MAXQUEUESIZE;

	output[0] = mCEdry * input[0] + mCEwet * mQueuePtr->mQueue[(rdloc++) % MAXQUEUESIZE];
	output[1] = mCEdry * input[1] + mCEwet * mQueuePtr->mQueue[rdloc % MAXQUEUESIZE];

	return;
}

void CAudioEffects::ProcessFlange(double *input, double* output, double time)
{
	double SampleRate = mInstrument->GetSampleRate();
	int delaylength = int((0.006 + sin(0.25 * 2 * M_PI * time) * 0.004) * SampleRate + 0.5) * 2;
	int wrloc = mQueuePtr->mWrloc;
	int rdloc = (wrloc + MAXQUEUESIZE - delaylength) % MAXQUEUESIZE;

	output[0] = mFEdry * input[0] + mFEwet * mQueuePtr->mQueue[(rdloc++) % MAXQUEUESIZE];
	output[1] = mFEdry * input[1] + mFEwet * mQueuePtr->mQueue[rdloc % MAXQUEUESIZE];

	return;
}


void CAudioEffects::ProcessReverb(double *input, double* output, double time)
{
	double delayTime = 0.25;
	double SampleRate = mInstrument->GetSampleRate();
	int delaylength = int((delayTime * SampleRate) + 0.5);
	int wrloc = mQueuePtr->mWrloc;
	int rdloc = (wrloc + MAXQUEUESIZE - delaylength) % MAXQUEUESIZE;

	output[0] = mREdry * input[0] + mREwet * mQueuePtr->mQueue[rdloc];
	output[1] = mREdry * input[1] + mREwet * mQueuePtr->mQueue[(rdloc + 1) % MAXQUEUESIZE];

	int delayfactor = 2;
	double ampFactor = 0.5;
	for (int i = 0; i < 5; i++)
	{
		rdloc = (wrloc + MAXQUEUESIZE - delayfactor * delaylength) % MAXQUEUESIZE;

		output[0] += ampFactor * mREwet * mQueuePtr->mQueue[rdloc];
		output[1] += ampFactor * mREwet * mQueuePtr->mQueue[(rdloc + 1) % MAXQUEUESIZE];

		delayfactor += 2;
		ampFactor /= 2;
	}
}

void CAudioEffects::ProcessRing(double* input, double* output, double time)
{
	double SampleRate = mInstrument->GetSampleRate();
	double freq = 2000.0; // hz
	double Amp = mRingEwet; //amp of sinewave
	double sample = Amp * (sin(time * 2 * M_PI * freq));
	output[0] = mRingEdry * input[0] * sample;
	output[1] = mRingEdry * input[1] * sample;	
	return;
}