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
    std::string filePathString(fileBuffer);    // 将字符数组转换为 std::string  
    std::string cppcheckPath = "./CPPcheck";  // 解压缩后的cppcheck可执行文件相对于当前工程目录的相对路径  
    std::size_t lastSlashPos = filePathString.find_last_of("\\"); // 查找最后一个反斜杠的位置 
    if (lastSlashPos != std::string::npos) 
    {    
        std::string pathWithoutLastPart = filePathString.substr(0, lastSlashPos);// 截取最后一个反斜杠之前的部分 
        cppcheckPath = pathWithoutLastPart + "\\CPPcheck";// 拼接指定的字符串  
        std::cout << "Modified path: " << cppcheckPath << std::endl;
    }

    std::string command = "\"" + cppcheckPath + "\\cppcheck --platform=win64 --enable=warning,style,performance,portability  --verbose --xml \"" + " " + m_path + " 2>&1";
    std::string result = "";
    char buffer[128];

    FILE* pipe = _popen(command.c_str(), "r");   //创建一个进程执行指定指令，并返回一个指向管道指针
    if (pipe == nullptr)
        return;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        result += buffer;
    }

    _pclose(pipe);

    //保存至xml文件中
    std::ofstream outputFile("output.xml");
    if (outputFile.is_open()) 
    {
        // 检查文件内容是否为空  
        outputFile.seekp(0, std::ios::end);
        if (outputFile.tellp() != 0) 
        {
            // 文件不为空，将文件内容清空  
            outputFile.close();
            std::ofstream clearFile("output.xml", std::ios::trunc);
            if (!clearFile.is_open())
                return;
            clearFile.close();
        }

        // 将XML字符串写入文件  
        outputFile << result;
        outputFile.close();
    }
}