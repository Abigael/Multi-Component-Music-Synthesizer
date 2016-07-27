#include "stdafx.h"
#include "Synthesizer.h"
#include "ToneInstrument.h"
#include "AdditiveInstrument.h"
#include "RecordPrefSynth.h"
#include "SubtractiveSynthesis.h"
//#include "AdditiveSynth.h"
#include "PianoNotes.h"

#include <cmath>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "xmlhelp.h"

using namespace std; 


//files have been changed for proj01

CSynthesizer::CSynthesizer()
{
	CoInitialize(NULL); //tell windows using COM objects
	m_channels = 2;
	m_sampleRate = 44100.;
	m_samplePeriod = 1 / m_sampleRate;
	m_time = 0;
	m_bpm = 120;
	m_secperbeat = 1 / (m_bpm / 60);
	m_beatspermeasure = 4;
	mCurrentEffects = new CAudioEffects;
	mQueuePtr = new CQueue;
}


CSynthesizer::~CSynthesizer()
{
}
//! Start the synthesizer
void CSynthesizer::Start(void)
{
	m_instruments.clear();
	m_currentNote = 0;
	m_measure = 0;
	m_beat = 0;
	m_time = 0;
}


//! Generate one audio frame Build list of active instuments
bool CSynthesizer::Generate(double * frame)
{
	//
	// Phase 1: Determine if any notes need to be played.
	//

	while (m_currentNote < (int)m_notes.size())
	{
		// Get a pointer to the current note
		CNote *note = &m_notes[m_currentNote];

		// If the measure is in the future we can't play
		// this note just yet.
		if (note->Measure() > m_measure)
			break;

		// If this is the current measure, but the
		// beat has not been reached, we can't play
		// this note.
		if (note->Measure() == m_measure && note->Beat() > m_beat)
			break;

		//
		// Play the note!
		//

		// Create the instrument object
		CInstrument *instrument = NULL;
		if (note->Instrument() == L"AdditiveInstrument") {
			instrument = new CAdditiveInstrument();
		}
		if (note->Instrument() == L"RecordedSynth") {
			instrument = new CRecordPrefSynth();
		}
		if (note->Instrument() == L"ToneInstrument")
		{
			instrument = new CToneInstrument();
		}
		if (note->Instrument() == L"SubtractiveInstrument"){
			instrument = new CSubtractiveSynthesis();
		}
		if (note->Instrument() == L"Piano")
		{
			m_pianofactory.SetNote(note);
			instrument = m_pianofactory.CreatePiano();
		} 


		// Configure the instrument object
		if (instrument != NULL)
		{
			instrument->SetSampleRate(GetSampleRate());
			instrument->SetSecondsPerBeat(m_secperbeat);
			instrument->SetNote(note);
			instrument->mEffects = note->mEffects;
			//THIS IS WHERE WE BREAK --debug
			instrument->Start();

			m_instruments.push_back(instrument);
		}

		m_currentNote++;
	}
	//
	// Phase 2: Clear all channels to silence 
	//
	for (int c = 0; c<GetNumChannels(); c++)
	{
		frame[c] = 0;
	}

	//
	// Phase 3: Play an active instruments
	//

	//
	// We have a list of active (playing) instruments.  We iterate over 
	// that list.  For each instrument we call generate, then add the
	// output to our output frame.  If an instrument is done (Generate()
	// returns false), we remove it from the list.
	//
	double FrameWithOutEffects[2];
	FrameWithOutEffects[0] = 0;
	FrameWithOutEffects[1] = 0;

	for (list<CInstrument *>::iterator node = m_instruments.begin(); node != m_instruments.end();)
	{
		// Since we may be removing an item from the list, we need to know in 
		// advance, what is after it in the list.  We keep that node as "next"
		list<CInstrument *>::iterator next = node;
		next++;
		// Get a pointer to the allocated instrument
		CInstrument *instrument = *node;
		// Call the generate function
		if (instrument->Generate())
		{
			// If we returned true, we have a valid sample.  Add it 
			// to the frame.
			for (int c = 0; c<GetNumChannels(); c++)
			{
				frame[c] += instrument->Frame(c);
			}

			FrameWithOutEffects[0] += frame[0];
			FrameWithOutEffects[1] += frame[1];

		
			//Link the instrument to its effect
			// and effect to the queue
			instrument->mEffects->mQueuePtr = mQueuePtr;
			instrument->mEffects->mInstrument = instrument;

			//Build containers for effects
			//
			double ceFrames[2];
			double feFrames[2];
			double reFrames[2];
			double ringFrames[2];

			for (int i = 0; i < 2; i++)
			{
				ceFrames[i] = 0;
				feFrames[i] = 0;
				reFrames[i] = 0;
				ringFrames[i] = 0;
				
			}

			//Compute effects for this instrument
			//
			if (instrument->mEffects->mFlangingEffect == true)
			{
				instrument->mEffects->ProcessFlange(frame, feFrames, m_time);
			}
			if (instrument->mEffects->mChorusEffect == true)
			{
				instrument->mEffects->ProcessChorus(frame, ceFrames, m_time);
			}
			if (instrument->mEffects->mReverberatonEffect == true)
			{
				instrument->mEffects->ProcessReverb(frame, reFrames, m_time);
			}
			if (instrument->mEffects->mRingEffect == true)
			{
				instrument->mEffects->ProcessRing(frame, ringFrames, m_time);
			}

			//Add effects to frame
			//
			for (int i = 0; i < GetNumChannels(); i++)
			{
				frame[i] += ceFrames[i];
				frame[i] += feFrames[i];
				frame[i] += reFrames[i];
				frame[i] += ringFrames[i];
				if (frame[i] > 32767)
					frame[i] = short(32767);
				if (frame[i] < -32768)
					frame[i] = short(-32768);
			}
		}
		else
		{
			// If we returned false, the instrument is done.  Remove it
			// from the list and delete it from memory.
			m_instruments.erase(node);
			delete instrument;
		}


		// Move to the next instrument in the list
		node = next;
	}

	//Save frame to  queue
	//
	mQueuePtr->WriteFrameOriginal(FrameWithOutEffects);


	//
	// Phase 4: Advance the time and beats
	//

	// Time advances by the sample period
	m_time += GetSamplePeriod();

	// Beat advances by the sample period divided by the 
	// number of seconds per beat.  The inverse of seconds
	// per beat is beats per second.
	m_beat += GetSamplePeriod() / m_secperbeat;

	// When the measure is complete, we move to
	// a new measure.  We might be a fraction into
	// the new measure, so we subtract out rather 
	// than just setting to zero.
	if (m_beat > m_beatspermeasure)
	{
		m_beat -= m_beatspermeasure;
		m_measure++;
	}
	//
	// Phase 5: Determine when we are done
	//

	// We are done when there is nothing to play.  We'll put something more 
	// complex here later.
	return !m_instruments.empty() || m_currentNote < (int)m_notes.size();
}
void CSynthesizer::Clear(void)
{
	m_instruments.clear();
	m_notes.clear();
}
void CSynthesizer::OpenScore(CString & filename)
{
	Clear();

	//
	// Create an XML document
	//

	CComPtr<IXMLDOMDocument>  pXMLDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument, (void**)&pXMLDoc));
	if (!succeeded)
	{
		AfxMessageBox(L"Failed to create an XML document to use");
		return;
	}

	// Open the XML document
	VARIANT_BOOL ok;
	succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
	if (!succeeded || ok == VARIANT_FALSE)
	{
		AfxMessageBox(L"Failed to open XML score file");
		return;
	}

	//
	// Traverse the XML document in memory!!!!
	// Top level tag is <score>
	//
	/*The simple thing to remember is this: Instead of IXMLDOMNode *, use CComPtr<IXMLDOMNode>.  It will work the same way, but CComPtr will take care of the reference count for you.*/
	CComPtr<IXMLDOMNode> node; //wraper around pointer that releases it when value node is changed
	pXMLDoc->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);
		if (nodeName == "score")
		{
			XmlLoadScore(node); // has instruments and attr
		}
	}
	sort(m_notes.begin(), m_notes.end());
}
void CSynthesizer::XmlLoadScore(IXMLDOMNode * xml)
{
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
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

		// Get the value of the attribute.  A CComVariant is a variable
		// that can have any type. It loads the attribute value as a
		// string (UNICODE), but we can then change it to an integer 
		// (VT_I4) or double (VT_R8) using the ChangeType function 
		// and then read its integer or double value from a member variable.
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == L"bpm")
		{
			value.ChangeType(VT_R8);
			m_bpm = value.dblVal;
			m_secperbeat = 1 / (m_bpm / 60);
		}
		else if (name == L"beatspermeasure")
		{
			value.ChangeType(VT_I4);
			m_beatspermeasure = value.intVal;
		}

	}


	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);

		if (name == L"instrument")
		{
			XmlLoadInstrument(node);
		}

	}
}
void CSynthesizer::XmlLoadInstrument(IXMLDOMNode * xml)
{
	wstring instrument = L"";

	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
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

		// Get the value of the attribute.  
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "instrument")
		{
			mCurrentEffects->ClearEffects();
			instrument = value.bstrVal;
		}	
	}


	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name; //microsoft basic string.
		node->get_nodeName(&name);

		if (name == L"note")
		{
			XmlLoadNote(node, instrument);
		}
		if (name == L"effect")
		{
			XmlLoadEffects(node);

		}
	}

}
void CSynthesizer::XmlLoadNote(IXMLDOMNode * xml, std::wstring & instrument)
{


	if (instrument == L"Compressor")
		m_compression.XmlLoad(xml);
	else if (instrument == L"NoiseGate")
		m_noiseGate.XmlLoad(xml);

	m_notes.push_back(CNote());

	CAudioEffects *notesEffects = new CAudioEffects;
	notesEffects->SetEffects(mCurrentEffects);
	m_notes.back().mEffects = notesEffects;
	
	m_notes.back().XmlLoad(xml, instrument);

}

void CSynthesizer::XmlLoadEffects(IXMLDOMNode * xml)
{


	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	//Get attribute 0 (effect type)
	CComPtr<IXMLDOMNode> attrib;
	attributes->get_item(0, &attrib);

	// Get the name of the attribute
	CComBSTR name;
	attrib->get_nodeName(&name);

	// Get the value of the attribute.  
	CComVariant value;
	attrib->get_nodeValue(&value);

	if (name == L"chorus")
	{
		value.ChangeType(VT_I4);
		if (value.intVal >= 1)
		{
			mCurrentEffects->mChorusEffect = true;
			for (int i = 1; i < len; i++)
			{
				// Get attribute i
				CComPtr<IXMLDOMNode> attrib;
				attributes->get_item(i, &attrib);

				// Get the name of the attribute
				CComBSTR name;
				attrib->get_nodeName(&name);

				// Get the value of the attribute.  
				CComVariant value;
				attrib->get_nodeValue(&value);

				if (name == L"wet")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mCEwet = value.dblVal;
				}
				else if (name == L"dry")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mCEdry = value.dblVal;
				}
			}

		}
		else
		{
			mCurrentEffects->mChorusEffect = false;
		}
	} //end chorus

	else if (name == L"reverberation")
	{
		value.ChangeType(VT_I4);
		if (value.intVal >= 1)
		{
			mCurrentEffects->mReverberatonEffect = true;
			for (int i = 1; i < len; i++)
			{
				// Get attribute i
				CComPtr<IXMLDOMNode> attrib;
				attributes->get_item(i, &attrib);

				// Get the name of the attribute
				CComBSTR name;
				attrib->get_nodeName(&name);

				// Get the value of the attribute.  
				CComVariant value;
				attrib->get_nodeValue(&value);

				if (name == L"wet")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mREwet = value.dblVal;
				}
				else if (name == L"dry")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mREdry = value.dblVal;
				}
			}

		}
		else
		{
			mCurrentEffects->mReverberatonEffect = false;
		}
	} //end reverbation

	else if (name == L"flanging")
	{
		value.ChangeType(VT_I4);
		if (value.intVal >= 1)
		{
			mCurrentEffects->mFlangingEffect = true;
			for (int i = 1; i < len; i++)
			{
				// Get attribute i
				CComPtr<IXMLDOMNode> attrib;
				attributes->get_item(i, &attrib);

				// Get the name of the attribute
				CComBSTR name;
				attrib->get_nodeName(&name);

				// Get the value of the attribute.  
				CComVariant value;
				attrib->get_nodeValue(&value);

				if (name == L"wet")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mFEwet = value.dblVal;
				}
				else if (name == L"dry")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mFEdry = value.dblVal;
				}
			}

		}
		else
		{
			mCurrentEffects->mFlangingEffect = false;
		}
	} //end Flanging

	else if (name == L"ring")
	{
		value.ChangeType(VT_I4);
		if (value.intVal >= 1)
		{
			mCurrentEffects->mRingEffect = true;
			for (int i = 1; i < len; i++)
			{
				// Get attribute i
				CComPtr<IXMLDOMNode> attrib;
				attributes->get_item(i, &attrib);

				// Get the name of the attribute
				CComBSTR name;
				attrib->get_nodeName(&name);

				// Get the value of the attribute.  
				CComVariant value;
				attrib->get_nodeValue(&value);

				if (name == L"wet")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mRingEwet = value.dblVal;
				}
				else if (name == L"dry")
				{
					value.ChangeType(VT_R8);
					mCurrentEffects->mRingEdry = value.dblVal;
				}
			}

		}
		else
		{
			mCurrentEffects->mRingEffect = false;
		}
	} //end Flanging
}