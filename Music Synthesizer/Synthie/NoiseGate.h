#pragma once
class CNoiseGate
{
public:
	CNoiseGate();
	virtual ~CNoiseGate();
	void Process(double *fOne, double *fTwo);
	void XmlLoad(IXMLDOMNode * xml);
	void SetThreshold(short thres){ m_threshold = thres; }
	void SetDry(double dry){ m_dry = dry; }
	void SetWet(double wet){ m_wet = wet; }
	void SetLeft(double left){ m_left = left; }
	void SetRight(double right){ m_right = right; }
private:
	//for noisegate, attenuate signals below threshold. 
	short m_threshold;
	double m_dry;
	double m_wet;
	double m_left;
	double m_right;
};
