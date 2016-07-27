#include "stdafx.h"
#include "Effects.h"

const int MaxQueueSize = 500000;

CEffects::CEffects()
{
	mQueue.resize(MaxQueueSize);
}


CEffects::~CEffects()
{
}


void CEffects::SetNote(CNote *note)
{
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
 
 
 		CComVariant value; 
 		attrib->get_nodeValue(&value); 
 
 
 		if (name == "wet") 
 		{ 
 			value.ChangeType(VT_R8); 
 			SetWet(value.dblVal); 
 		} 
 		else if (name == "dry") 
 		{ 
 			value.ChangeType(VT_R8); 
 			SetDry(value.dblVal); 
 		} 
 		else if (name == "delay") 
 		{ 
 			value.ChangeType(VT_R8); 
 			SetDelay(value.dblVal); 
 		} 
 	} 
}