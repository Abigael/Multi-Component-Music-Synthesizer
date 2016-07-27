#pragma once
#include "PianoInstrument.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

class CPianoFactory
{
public:
	CPianoFactory();
	virtual ~CPianoFactory();
	CPianoInstrument *CreatePiano();
	void SetNote(CNote *);
	bool LoadFile(const char*);

private:
	std::vector<short> m_pianowave;
	bool m_pedal;
	double m_amp;
	std::map<string, std::vector<short>>m_stored_notes;
};

