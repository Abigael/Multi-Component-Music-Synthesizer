#include "stdafx.h"
#include "RecordPrefSynth.h"
#include "audio/DirSoundSource.h"
#include <string>
#include <fstream>

CRecordPrefSynth::CRecordPrefSynth()
{	
//	m_compression =  m_noiseGate= false;
}


CRecordPrefSynth::~CRecordPrefSynth()
{
}
void CRecordPrefSynth::Start(){
//	m_position = 0;
	m_time = 0;
}
bool CRecordPrefSynth::Generate(){ 

	if (m_noiseGateRecord)
		noiseGate.Process(&m_soundL[index], &m_soundL[index]); //TIS GETS A ERRORRR
	if (m_compressionRecord)
		compressor.Process(&m_soundL[index], &m_soundL[index]);

	m_frame[0] = m_soundL[index ++]; //NEED THIS TO BE REAL
	m_frame[1] = m_soundL[index];

	
	m_time += GetSamplePeriod();
	return m_time < m_duration;
 }
void CRecordPrefSynth::SetNote(CNote *note){
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i < len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		// Get the value of the attribute.  A CComVariant is a variable
		// that can have any type. It loads the attribute value as a
		// string (UNICODE), but we can then change it to an integer 
		// (VT_I4) or double (VT_R8) using the ChangeType function 
		// and then read its integer or double value from a member variable.
		CComVariant value;
		attrib->get_nodeValue(&value);


		if (name == L"filename"){
			//	LoadFile("audio/something.mp3");
			std::string myString = _bstr_t(value.bstrVal, false);
			LoadFile(myString.c_str()); //this is where we load file
		}

		if (name == L"effect"){
			CComBSTR typeEffect = value.bstrVal;
			if (typeEffect == L"NoiseGate"){
				m_noiseGateRecord = true;
			}
			if (typeEffect == L"Compression"){
				m_compressionRecord = true;
			}
		}
		if (name == L"threshold"){
			CComBSTR typeEffect = value.bstrVal;
			value.ChangeType(VT_R8);
			if (m_noiseGateRecord == true)
				noiseGate.SetThreshold(value.dblVal);
			if (m_compressionRecord == true)
				compressor.SetThreshold(value.dblVal);
		}
		if (name == L"ratio"){
			CComBSTR typeEffect = value.bstrVal;
			value.ChangeType(VT_R8);
			if (m_compressionRecord == true)
				compressor.SetRatio(value.dblVal);
		}
		if (name == L"duration"){
			CComBSTR typeEffect = value.bstrVal;
			value.ChangeType(VT_R8);
			if (m_duration > value.dblVal) //for part that makes sure we are just trying to get a smaller part
				m_duration = value.dblVal;
			
		}
		if (name == L"dry"){
			CComBSTR typeEffect = value.bstrVal;
			value.ChangeType(VT_R8);
			if (m_noiseGateRecord == true)
				noiseGate.SetDry(value.dblVal);
			if (m_compressionRecord == true)
				compressor.SetDry(value.dblVal);
		}
		if (name == L"wet"){
			CComBSTR typeEffect = value.bstrVal;
			value.ChangeType(VT_R8);
			if (m_noiseGateRecord == true)
				noiseGate.SetWet(value.dblVal);
			if (m_compressionRecord == true)
				compressor.SetWet(value.dblVal);
		}
	}
}

bool CRecordPrefSynth::LoadFile(const char *filename){
	m_soundL.clear();
	m_soundR.clear();
	CDirSoundSource file;
	
	if (!file.Open(filename))
	{
		CString message = L"Unable to open audio file: ";
		message += filename;
		AfxMessageBox(message);
		return false;
	}
	//file.Rewind();

	double time = 0; 
	for (int i = 0; i < file.NumSampleFrames() * .9 ; i++, time += 1.0 / file.SampleRate())
	{
		short frame[2];
		file.ReadFrame(frame);
		m_soundL.push_back(frame[0]);
		m_soundR.push_back(frame[1]);
	}
	m_duration = file.GetDuration() *.9; //set variable for .9 times.
	
	file.Close();
	return true;
}