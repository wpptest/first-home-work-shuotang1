// -----------------------------------------------
// kcppcheckxml.h
// 功能描述：给定一个路径，解析其里面的.h与.cpp文件,并将解析结果保存至xml文件中
// -------------------------------------------------

#ifndef __KCPPCHECKXML_KCPPCHECKXML_H_
#define __KCPPCHECKXML_KCPPCHECKXML_H_

#include<string>

class KCppCheckXML
{
public:
	KCppCheckXML(std::string path);
	~KCppCheckXML();

	void runCppCheck();           //解析路径，并将其保存为XML文件
private:
	std::string m_path;           //解析路径
};

#endif
