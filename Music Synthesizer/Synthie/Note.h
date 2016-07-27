#pragma once
#include <string>
#include "AudioEffects.h"
//if add instr h then get circ ref. 
class CNote
{
public:
	CNote();
	virtual ~CNote();

	int Measure() const { return m_measure; }
	double Beat() const { return m_beat; }
	const std::wstring &Instrument() const { return m_instrument; }
	IXMLDOMNode *Node() { return m_node; }
	void XmlLoad(IXMLDOMNode * xml, std::wstring & instrument);
	bool operator<(const CNote &b);

private:
	std::wstring m_instrument;
	int m_measure;
	double m_beat;
	CComPtr<IXMLDOMNode> m_node;

//store the effect characteristics for this node
public:
	CAudioEffects* mEffects;


};

