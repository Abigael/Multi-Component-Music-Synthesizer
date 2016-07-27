#include "stdafx.h"
#include "NoiseGate.h"


CNoiseGate::CNoiseGate()
{
	m_left = m_right = 1;
	m_dry = m_wet = m_dry = 0.0;
	m_threshold = 50; //default value
	m_wet = 1;
	
}


CNoiseGate::~CNoiseGate()
{
}
/*Noise gate lets signal pass through only when it is above set thresholf- gate is open
* if below, no signal is alowed to pass or is attenuated. 
*/
void CNoiseGate::Process(double *fOne, double *fTwo)
{
	//if frame less then attenuate; a/r
	if (fOne[0] < m_threshold){
		if (m_left > 0)
			m_left = m_left - 0.001; //attack release constant
	}
	else{
		if (m_left < 1)
			m_left += 0.001; 
	}
	if (fOne[1] < m_threshold){
		if (m_right > 0)
			m_right = m_right - 0.001; //attack release constant
	}
	else{
		if (m_right < 1)
			m_right += 0.001;
	}
	fTwo[0] = m_dry*fOne[0] + m_wet*fOne[0] * m_left;
	fTwo[1] = m_dry*fOne[1] + m_wet*fOne[1] * m_right;
}
//function taken from ToneInst
void CNoiseGate::XmlLoad(IXMLDOMNode * xml){

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

		if (name == "threshold")
		{
			value.ChangeType(VT_I4);
			m_threshold = value.intVal;
		}
		else if (name == "wet")
		{
			value.ChangeType(VT_R8);
			m_wet = value.dblVal;
		}
		else if (name == "dry")
		{
			value.ChangeType(VT_R8);
			m_dry = value.dblVal;
		}
	}
}