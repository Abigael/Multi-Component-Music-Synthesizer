/**
* Aurthor: Alex R. Zajac
*
* file: SubtractiveSynthesis.cpp
*
*/

#include "stdafx.h"
#include "SubtractiveSynthesis.h"
#include "Notes.h"
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#define M_PI 3.14159265359


short RBound(double d)
{
	if (d < -32768)
		return -32768;
	else if (d > 32767)
		return 32767;

	return (short)d;
}

CSubtractiveSynthesis::CSubtractiveSynthesis()
{

	m_attack = m_release = m_decay = 0;
	m_sustain = 1.0;
	m_rindex = 0;
	m_windex = 0;
	m_amp = 1000.0;
	m_reson = 0;
	m_sweep = 0;
}


CSubtractiveSynthesis::~CSubtractiveSynthesis()
{
}

void CSubtractiveSynthesis::Start()
{
	m_time = 0;
	m_audio = new short[int(m_duration*GetSampleRate())];
	GenerateWaveTable();
}

bool CSubtractiveSynthesis::Generate()
{
	m_frame[0] = m_frame[1] = m_audio[m_rindex++];

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
void CSubtractiveSynthesis::SetNote(CNote *note)
{
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);
	for (int i = 0; i < len; i++)
	{
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);
		CComBSTR name;
		attrib->get_nodeName(&name);
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "duration") {
			value.ChangeType(VT_R8);
			m_duration = value.dblVal * GetSecondsPerBeat();
		}
		else if (name == "note") {
			m_freq = NoteToFrequency(value.bstrVal);
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
		else if (name == "reson")
		{
			std::wstring wide(value.bstrVal);
			std::string str(wide.begin(), wide.end());
			std::stringstream ss(str);
			std::string item;

			std::getline(ss, item, char(32));
			m_reson = atof(item.c_str());
			std::getline(ss, item, char(32));
			m_bandwidth = atof(item.c_str());
		}
		else if (name == "filter")
		{
			std::wstring wide(value.bstrVal);
			std::string str(wide.begin(), wide.end());
			std::stringstream ss(str);
			std::string item;

			std::getline(ss, item, char(32));
			m_sweep = atof(item.c_str());
		}
		else if (name == "wave")
		{
			std::wstring wide(value.bstrVal);
			std::string str(wide.begin(), wide.end());
			std::stringstream ss(str);
			std::string item;

			std::getline(ss, item, char(32));
			m_wave = item.c_str();
		}
	}
}

void CSubtractiveSynthesis::GenerateWaveTable()
{
	double sineRadians = 0; // For the sine wave
	double time = 0;
	int sign = 1;
	double rmax = m_reson + m_reson * m_sweep / 100.0;
	double rmin = m_reson - m_reson * m_sweep / 100.0;
	double m_temp = m_reson;

	for (int i = 0; i < int(m_duration*GetSampleRate()); i++, time += 1. / GetSampleRate())
	{

		double sample = 0;
		double freqMax = 22050;
		for (float n = 1.0; n*m_freq <= freqMax && n <= 8.0; n++)
		{
			if (m_wave == "square")
				sample += (m_amp*(1.0 / n)  * (sin(n * 2 * M_PI * m_freq * time)));
			else if (m_wave == "saw")
			{
				sample += (1.5*m_amp*(1.0 / n)  * (sin(n * 2 * M_PI * m_freq * time)));
				n++;
			}
			else if (m_wave == "triangle")
			{
				sample += (2*m_amp*(1.0 / pow(n, 2))  * (sin(n * 2 * M_PI * m_freq * time)));
				sign *= -1;
				n++;
			}
		}
		if (m_reson > 0 && m_bandwidth > 0 && i > 2)
		{
			double rvalue = 1.0 - m_bandwidth / 2.0;
			double costheta = (2 * rvalue * cos(2 * M_PI * m_temp)) / (1.0 + pow(rvalue, 2));
			double avalue = (1.0 - pow(rvalue, 2)) * sin(acos(costheta)) * 15;
			sample = (sample * avalue + m_audio[i-1] * 2 * rvalue * costheta - m_audio[i-2] * pow(rvalue,2));
			m_temp = m_reson + ((rmax-rmin) * sin(2 * M_PI + i/m_freq));
		}
		m_audio[i] = RBound(sample);
	}
	
}
