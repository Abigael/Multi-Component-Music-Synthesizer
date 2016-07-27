#pragma once
#include <vector>

class CQueue
{
public:
	CQueue();
	virtual ~CQueue();

	//effect stuff
	std::vector<double> mQueue;
	int mWrloc = 0;
	void WriteFrameOriginal(double *input);

};

