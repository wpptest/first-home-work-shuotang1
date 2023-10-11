#include"kcppcheckxml.h"

#include<stdio.h>
#include <fstream> 
#include<iostream>
#include<cstring>

KCppCheckXML::KCppCheckXML(std::string path)
	:m_path(path)
{

}

KCppCheckXML::~KCppCheckXML()
{

}

void KCppCheckXML::runCppCheck()
{ 
    char fileBuffer[_MAX_PATH];
    if (_fullpath(fileBuffer, __FILE__, _MAX_PATH) != nullptr)
    {
        std::cout << "Absolute path of the current file: " << fileBuffer << std::endl;
    }
    std::string filePathString(fileBuffer);    // ���ַ�����ת��Ϊ std::string  
    std::string cppcheckPath = "./CPPcheck";  // ��ѹ�����cppcheck��ִ���ļ�����ڵ�ǰ����Ŀ¼�����·��  
    std::size_t lastSlashPos = filePathString.find_last_of("\\"); // �������һ����б�ܵ�λ�� 
    if (lastSlashPos != std::string::npos) 
    {    
        std::string pathWithoutLastPart = filePathString.substr(0, lastSlashPos);// ��ȡ���һ����б��֮ǰ�Ĳ��� 
        cppcheckPath = pathWithoutLastPart + "\\CPPcheck";// ƴ��ָ�����ַ���  
        std::cout << "Modified path: " << cppcheckPath << std::endl;
    }

    std::string command = "\"" + cppcheckPath + "\\cppcheck --platform=win64 --enable=warning,style,performance,portability  --verbose --xml \"" + " " + m_path + " 2>&1";
    std::string result = "";
    char buffer[128];

    FILE* pipe = _popen(command.c_str(), "r");   //����һ������ִ��ָ��ָ�������һ��ָ��ܵ�ָ��
    if (pipe == nullptr)
        return;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        result += buffer;
    }

    _pclose(pipe);

    //������xml�ļ���
    std::ofstream outputFile("output.xml");
    if (outputFile.is_open()) 
    {
        // ����ļ������Ƿ�Ϊ��  
        outputFile.seekp(0, std::ios::end);
        if (outputFile.tellp() != 0) 
        {
            // �ļ���Ϊ�գ����ļ��������  
            outputFile.close();
            std::ofstream clearFile("output.xml", std::ios::trunc);
            if (!clearFile.is_open())
                return;
            clearFile.close();
        }

        // ��XML�ַ���д���ļ�  
        outputFile << result;
        outputFile.close();
    }
}