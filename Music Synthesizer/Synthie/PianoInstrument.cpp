#include "stdafx.h"
#include "PianoInstrument.h"
#include "PianoNotes.h"



CPianoInstrument::CPianoInstrument()
{
	pedal_pressed = false;
}


CPianoInstrument::~CPianoInstrument()
{
}

void CPianoInstrument::Start(){

	m_pianonode.SetSampleRate(GetSampleRate());
	m_pianonode.SetDuration(m_duration);
	m_pianonode.Start();
	m_time = 0;


}

bool CPianoInstrument::Generate()
{
	m_pianonode.SetAmplitude(m_amp);
	m_pianonode.SetPedal(pedal_pressed);
	bool valid = m_pianonode.Generate();

	m_frame[0] = m_pianonode.Frame(0);/// 32767;
	m_frame[1] = m_pianonode.Frame(1); /// 32767;

	
	return valid;
}

void CPianoInstrument::SetNote(CNote *note)
{
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i<len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "duration")
		{
			value.ChangeType(VT_R8);
			SetDuration(value.dblVal);
		}
		else if (name == "note")
		{
			if (NoteToFile(value.bstrVal, 's') == "CompletePiano\pedald.wav"){
			
				pedal_pressed = true;
			}
			else if (NoteToFile(value.bstrVal, 's') == "CompletePiano\pedalu.wav"){

				pedal_pressed = false;
			}
		}
		
	}

}