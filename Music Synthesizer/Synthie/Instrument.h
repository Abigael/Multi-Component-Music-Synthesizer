#pragma once
#include "AudioNote.h"
#include "AudioEffects.h"
#include "Note.h"
#include <vector>
class CInstrument :
	public CAudioNote
{
public:
	CInstrument();
	virtual ~CInstrument();
	virtual void SetNote(CNote *note) = 0;
	bool GetCompression(){ return m_compressionApply; }


private: 
	bool m_noiseGateApply;
	bool m_compressionApply;

public:
	CAudioEffects* mEffects;
};

