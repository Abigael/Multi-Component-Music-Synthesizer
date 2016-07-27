#include "stdafx.h"
#include "Queue.h"

const int MAXQUEUESIZE = 200000;

CQueue::CQueue()
{
	mQueue.resize(MAXQUEUESIZE);
}


CQueue::~CQueue()
{
}


void CQueue::WriteFrameOriginal(double *input)
{
	for (int i = 0; i < 2; i++)
	{
		mQueue[mWrloc + i] = input[i];
	}

	mWrloc = (mWrloc + 2) % MAXQUEUESIZE;
	return;
}