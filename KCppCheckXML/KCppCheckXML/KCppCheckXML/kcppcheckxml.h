// -----------------------------------------------
// kcppcheckxml.h
// ��������������һ��·���������������.h��.cpp�ļ�,�����������������xml�ļ���
// -------------------------------------------------

#ifndef __KCPPCHECKXML_KCPPCHECKXML_H_
#define __KCPPCHECKXML_KCPPCHECKXML_H_

#include<string>

class KCppCheckXML
{
public:
	KCppCheckXML(std::string path);
	~KCppCheckXML();

	void runCppCheck();           //����·���������䱣��ΪXML�ļ�
private:
	std::string m_path;           //����·��
};

#endif
