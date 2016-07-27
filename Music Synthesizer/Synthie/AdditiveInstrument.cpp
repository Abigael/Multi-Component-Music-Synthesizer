/**
 * \file AdditiveInstrument.cpp
 *
 * \author Cam Merrill
 */

#include "StdAfx.h"
#include "AdditiveInstrument.h"
#include "Notes.h"
#include <string>
#include <sstream>
#include <vector>



CAdditiveInstrument::CAdditiveInstrument()
{
	m_crossFadeIn = 0.0;
	m_crossFadeOut = 0.0;
	m_attack = m_release = m_decay = 0;
	m_sustain = 1.0;
}


CAdditiveInstrument::~CAdditiveInstrument()
{
}

void CAdditiveInstrument::Start()
{
	m_sinewave.SetSampleRate(GetSampleRate());
	m_sinewave.SetDuration(m_duration);
	m_sinewave.Start();
	m_time = 0;
}


bool CAdditiveInstrument::Generate()
{
	m_sinewave.Generate();

	m_frame[0] = m_sinewave.Frame(0);
	m_frame[1] = m_sinewave.Frame(1);

	double factor = 1.0;
	double sign = -1.0;
	if (m_sustain > 1.0) 
	{
		sign = 1.0;
	}

	if (m_time < m_attack)
	{
		factor = m_time * 1;
		factor /= m_attack;
	}
	else if (m_time < m_decay)
	{
		factor = sign*((1.0 - m_sustain) / (m_decay - m_attack));
		factor *= (m_time);
		factor += (1.0 - sign*((1.0 - m_sustain) / (m_decay - m_attack))*(m_attack));
	}
	else if (m_time >(m_duration - m_release) && m_release != 0)
	{
		if (m_sustain <= 0) 
		{
			m_sustain = 1.0;
		}
		factor = m_time*-m_sustain / m_release + (m_sustain / m_release)*(m_duration);

	}
	else {

		factor = m_sustain;
	}

	m_frame[0] = m_frame[1] *= factor;
	m_time += GetSamplePeriod();
	return m_time < m_duration;
}

void CAdditiveInstrument::SetNote(CNote *note)
{
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);
	for (int i = 0; i<len; i++)
	{
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);
		CComBSTR name;
		attrib->get_nodeName(&name);
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "duration") {
			value.ChangeType(VT_R8);
			SetDuration(value.dblVal);
		}
		else if (name == "note") {
			SetFreq(NoteToFrequency(value.bstrVal));
		}
		else if (name == "amplitudes") {
			std::wstring wide(value.bstrVal);
			std::string str(wide.begin(), wide.end());
			std::stringstream ss(str);
			std::string item;

			double harmonics[8] = { 0 };
			int i = 0;
			while (std::getline(ss, item, char(32)))  {
				harmonics[i++] = atof(item.c_str());
			}

			SetAmplitude(harmonics[0]);
		}
		else if (name == "crossFadeIn") {
			value.ChangeType(VT_R8);
			SetCrossFadeIn(value.dblVal * m_duration);
		}
		else if (name == "crossFadeOut") {
			value.ChangeType(VT_R8);
			SetCrossFadeOut(value.dblVal * m_duration);
		}
		else if (name == "ADSR") {
			std::wstring wide(value.bstrVal);
			std::string str(wide.begin(), wide.end());
			std::stringstream ss(str);
			std::string item;

			std::getline(ss, item, char(32));
			m_attack = atof(item.c_str()) * m_duration;
			std::getline(ss, item, char(32));
			m_decay = m_attack + atof(item.c_str()) * m_duration;
			std::getline(ss, item, char(32));
			m_sustain = atof(item.c_str());
			std::getline(ss, item, char(32));
			m_release = atof(item.c_str()) * m_duration;

		}
		else if (name == "vibrato") {
			std::wstring wide(value.bstrVal);
			std::string str(wide.begin(), wide.end());
			std::stringstream ss(str);
			std::string item;

			std::getline(ss, item, char(32));
			m_sinewave.SetVibratoRate(atof(item.c_str()));
			std::getline(ss, item, char(32));
			m_sinewave.SetVibratoFreq(atof(item.c_str()));
		}

	}

}