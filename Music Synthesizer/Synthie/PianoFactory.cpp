#include "stdafx.h"
#include "PianoFactory.h"
#include "mmsystem.h"
#include "audio\DirSoundSource.h"
#include "PianoNotes.h"


CPianoFactory::CPianoFactory()
{
	m_pedal = false;
	m_amp = 1.0;
}


CPianoFactory::~CPianoFactory()
{
}

CPianoInstrument *CPianoFactory::CreatePiano()
{
	CPianoInstrument *piano = new CPianoInstrument();
	piano->SetPedal(m_pedal);
	piano->SetAmplitude(m_amp);
	piano->GetPlayer()->SetSamples(m_pianowave);

	return piano;
}

void CPianoFactory::SetNote(CNote *note)
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

		if (name == "dynamic"){
		
			value.ChangeType(VT_R8);
			m_amp = value.dblVal;
		
		}

		else if (name == "note"){

			LoadFile(NoteToFile(value.bstrVal, 'l'));
			LoadFile(NoteToFile(value.bstrVal, 's'));

			if (NoteToFile(value.bstrVal, 's') == "pedald.wav"){
				m_pedal = true;
			}

			else if (NoteToFile(value.bstrVal, 's') == "pedalu.wav"){
				m_pedal = false;
			}
		
		}

	}

}


bool CPianoFactory::LoadFile(const char* filename){

	string current_file = filename;
	map<string, vector<short>>::iterator it = m_stored_notes.find(current_file);
	char dyn = filename[current_file.size() - 5];

	if (it != m_stored_notes.end()){

		m_pianowave = it->second;

		return true;
	}

	CDirSoundSource m_file;

	if (!m_file.Open(filename))
	{
		return false;

	}

	m_pianowave.resize(88200 * 4);

	for (int i = 0; i<88200; i++)

	{
		short frame[2];

		m_file.ReadFrame(frame);

		if (dyn == 'l'){

			m_pianowave[i * 4] = frame[0];

			m_pianowave[i * 4 + 2] = frame[1];

		}

		else if (dyn == 's'){

			m_pianowave[i * 4 + 1] = frame[0];

			m_pianowave[i * 4 + 3] = frame[1];

		}

		else if (dyn == 'u' || dyn == 'd'){

			m_pianowave[i * 4] = frame[0];

			m_pianowave[i * 4 + 1] = frame[0];

			m_pianowave[i * 4 + 2] = frame[1];

			m_pianowave[i * 4 + 3] = frame[1];

		}
	}

	m_stored_notes.insert(pair<string, vector<short>>(current_file, m_pianowave));

	m_file.Close();

	return true;

}





