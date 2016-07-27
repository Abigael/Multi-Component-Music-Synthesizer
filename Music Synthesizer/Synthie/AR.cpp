#include "stdafx.h"
#include "AR.h"


CAR::CAR()
{
	m_duration = 0.1;
	m_attack = m_release= 0.05;
}


CAR::~CAR()
{
}

void CAR::Start()
{
	m_time = 0;
	m_source->SetSampleRate(GetSampleRate());
	m_source->Start();
	
}
 bool CAR::Generate()
{
	/*// Tell the component to generate an audio sample
	m_sinewave.Generate();

	// Read the component's sample and make it our resulting frame.
	m_frame[0] = m_sinewave.Frame(0);
	m_frame[1] = m_sinewave.Frame(1);

	// Update time
	m_time += GetSamplePeriod();

	// We return true until the time reaches the duration.
	return m_time < m_duration;*/

	
	if (m_time > m_attack && m_time < m_duration - m_release){
		m_frame[0] = m_source->Frame(0);
		m_frame[1] = m_source->Frame(1);
	}
	else if (m_time < m_attack){
		m_frame[0] = m_time * m_source->Frame(0) / m_attack;
		m_frame[1] =m_time * m_source->Frame(1) / m_attack;
	}
	else
	{
		m_frame[0] = (m_duration - m_time) * m_source->Frame(0) /m_release;
		m_frame[1] = (m_duration - m_time) * m_source->Frame(1) / m_release;
	}
	m_time += GetSamplePeriod();
	return m_time < m_duration;
 }