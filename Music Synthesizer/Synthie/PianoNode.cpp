#include "stdafx.h"
#include "PianoNode.h"


CPianoNode::CPianoNode()
{
	m_numsamples = 0;
	m_amp = 1.0;
	m_time = 0.0;
}


CPianoNode::~CPianoNode()
{
}

void CPianoNode::Start()
{
	m_position = 0;
	m_amp = 1.0;
}

bool CPianoNode::Generate()
{
	double current_gain;
	if (m_time < 0.05){
		current_gain = m_time / 0.05;

	}
	else{
		if (!m_pedal){

			current_gain = (m_duration - m_time) / (m_duration - 0.05);
				
		}
		else{

			current_gain = 1.0;
			
		}
	if (m_time >2.0){
		current_gain *= 0.5;
		
	}

	}
	
	if (m_position > 352796){
		m_position = 308700;
		
	}
	m_frame[0] = current_gain*(double(m_samples[m_position++]) * m_amp + double(m_samples[m_position++]) * (1.0 - m_amp));
	m_frame[1] = current_gain*(double(m_samples[m_position++]) * m_amp + double(m_samples[m_position++]) * (1.0 - m_amp));
	

	m_time += GetSamplePeriod();
	

	return m_time < m_duration;

}