#include "stdafx.h"
#include "Compression.h"



CCompression::CCompression()
{
	m_threshold = m_dry= m_wet =0;
	m_ratio = 1;
}


CCompression::~CCompression()
{
}

//altered from noiseGate
void CCompression::Process(double *fOne, double *fTwo){
	//if frame less leave alone
	if (fOne[0] < m_threshold){
		fTwo[0] = fOne[0];
	}
	//double check fromula
	else{
		fTwo[0] = m_dry*fOne[0] + m_wet*(fOne[0] - m_threshold) / m_ratio + m_wet*m_threshold; 
	}
	if (fOne[1] < m_threshold){
		fTwo[1] = fOne[1];
	}
	else{
		fTwo[1] = m_dry*fOne[1] + m_wet*(fOne[1] - m_threshold) / m_ratio + m_wet*m_threshold;
	}
}
//copied from noiseGate
void CCompression::XmlLoad(IXMLDOMNode * xml){
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
		else if (name == "ratio"){
			value.ChangeType(VT_R8);
			m_ratio = value.dblVal;
		}
	}
}
