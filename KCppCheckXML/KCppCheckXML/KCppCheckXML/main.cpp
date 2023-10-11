#include"kcppcheckxml.h"

#include<string>

int main(int argc,char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		/*std::string filePath = "G:\\CPPcheck-rules\\cppcheck-2.12.0\\test\\cfg";*/
		std::string filePath = argv[i];
		KCppCheckXML* kCppCheckXML = new KCppCheckXML(filePath);
		kCppCheckXML->runCppCheck();

		delete kCppCheckXML;
		kCppCheckXML = nullptr;
	}
	return 0;
}