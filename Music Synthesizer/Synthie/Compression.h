/** Alexandria Marone Proj01
* Effects Preformance Synthesizer
*/
#pragma once
class CCompression
{
public:
	CCompression();
	virtual ~CCompression();
	void Process(double *frameIn, double *frameOut);
	void XmlLoad(IXMLDOMNode * xml);
	void SetThreshold(short thres){ m_threshold = thres; }
	void SetDry(double dry){ m_dry = dry; }
	void SetWet(double wet){ m_wet = wet; }
	void SetRatio(double ratio){ m_ratio = ratio; }
private:
	short m_threshold;
	//amnt gain reduction <1
	double m_ratio;
	double m_dry;
	double m_wet;

};

