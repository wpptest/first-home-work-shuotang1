#include"kjplagsimilarity.h"

#include<fstream>
#include<vector>
#include<algorithm>
#include<iostream>
#include <codecvt> 

#include <direct.h>  
#include <windows.h>

KJplagSimilarity::KJplagSimilarity()
{

}

KJplagSimilarity::~KJplagSimilarity()
{

}

bool compareSimilarity(const Comparison& c1, const Comparison& c2)
{
    return c1.similarity > c2.similarity;
}

void removeBomCharacter(std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file)
        return;

    // ��ȡ�ļ�ǰ3���ֽ�  
    char bom[3];
    file.read(bom, 3);

    // ����Ƿ���UTF-8 BOM  
    if (!(file && bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF))
        return;              // �ļ�����UTF-8 BOM���� 
    
    // �ļ���UTF-8 BOM����,��ȡ�ļ�����  
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::wstring utf16_content = converter.from_bytes(content);
    file.close();

    // ����ת��������ݵ��ļ�  
    std::ofstream outfile(filePath, std::ios::binary);
    if (!outfile)
        return;
    outfile << converter.to_bytes(utf16_content);
    outfile.close();
}

void KJplagSimilarity::processDirectory(const std::string& directoryPath)
{
    WIN32_FIND_DATAA fileData;
    HANDLE directoryHandle = FindFirstFileA((directoryPath + "/*").c_str(), &fileData);
    if (directoryHandle != INVALID_HANDLE_VALUE) 
    {
        do {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
                if (std::strcmp(fileData.cFileName, ".") != 0 && std::strcmp(fileData.cFileName, "..") != 0) 
                {
                    std::string subdirectoryPath = directoryPath + "\\" + fileData.cFileName;
                    processDirectory(subdirectoryPath);  // �ݹ鴦����Ŀ¼  
                }
            }
            else 
            {
                std::string fileName = fileData.cFileName;
                std::string filePath = directoryPath + "\\" + fileName;
                removeBomCharacter(filePath);
            }
        } while (FindNextFileA(directoryHandle, &fileData));

        FindClose(directoryHandle);
    }
    else 
    {
        std::cerr << "Failed to open directory: " << directoryPath << std::endl;
    }
}

void KJplagSimilarity::compareTwoDirectoryFileSimilarity(std::string oldFilePath, std::string newFilePath,int n)
{
    char fileBuffer[_MAX_PATH];
    if (_fullpath(fileBuffer, __FILE__, _MAX_PATH) != nullptr)
    {
        std::string filePath = fileBuffer;

        // ��ȡ��Ŀ¼·��  
        std::size_t lastSlash = filePath.find_last_of("/\\");
        std::string parentPath = filePath.substr(0, lastSlash);

        // �л�������·��  
        if (_chdir(parentPath.c_str()) != 0)
            return;
    }

    std::string command = "D:\\jdk-17\\jdk-17_windows-x64_bin\\jdk-17.0.8\\bin\\java -jar jplag-4.3.0-jar-with-dependencies.jar -l cpp -r result -old " + oldFilePath + " -new " + newFilePath;

	FILE* pipe = _popen(command.c_str(), "r");
	if (!pipe)
		return;

    std::vector<Comparison> comparisons;
    char buffer[256];
    while (!feof(pipe)) 
    {
        if (fgets(buffer, 256, pipe) != nullptr)
        {
            std::string line(buffer);

            if (line.find("[INFO] ParallelComparisonStrategy - Comparing") != std::string::npos)
            {
                // ��ȡ�ļ��������ƶ�  
                size_t colonPos = line.find_last_of(":");
                std::string fileName = line.substr((line.find("Comparing") + 10), (colonPos - line.find("Comparing") - 10));
                double similarity = std::stod(line.substr(colonPos + 1));

                // ����Comparison�ṹ�岢��ӵ��ȽϽ��������  
                Comparison comparison;
                comparison.fileName = fileName;
                comparison.similarity = similarity;
                comparisons.push_back(comparison);
            }
        }
    }

    _pclose(pipe);

    // �����ƶȽ�������ȽϽ��  
    std::sort(comparisons.begin(), comparisons.end(), compareSimilarity);

    // ������ƶ���ߵ�ǰn���ļ��������ƶ�  
    int len = min(comparisons.size(), n);
    for (int i = 0; i < len; ++i) 
    {
        std::cout << "�ļ���: " << comparisons[i].fileName << ", ���ƶ�: " << comparisons[i].similarity << std::endl;
    }
}

void KJplagSimilarity::checkDirectorySimilarity(std::string filePath)
{
    char fileBuffer[_MAX_PATH];
    if (_fullpath(fileBuffer, __FILE__, _MAX_PATH) == nullptr)
        return;

    std::string filePathTemp = fileBuffer;

    // ��ȡ��Ŀ¼·��  
    std::size_t lastSlash = filePathTemp.find_last_of("/\\");
    std::string parentPath = filePathTemp.substr(0, lastSlash);

    // �л�������·��  
    if (_chdir(parentPath.c_str()) != 0)
        return;

    //�Ƚ����ƶ�ǰ��������
    std::string command = "D:\\jdk-17\\jdk-17_windows-x64_bin\\jdk-17.0.8\\bin\\java -jar jplag-4.3.0-jar-with-dependencies.jar -l cpp " + filePath;

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe)
        return;

    std::vector<Comparison> comparisons;
    char buffer[256];
    while (!feof(pipe))
    {
        if (fgets(buffer, 256, pipe) != nullptr)
        {
            std::string line(buffer);

            if (line.find("[INFO] ParallelComparisonStrategy - Comparing") != std::string::npos)
            {
                // ��ȡ�ļ��������ƶ�  
                size_t colonPos = line.find_last_of(":");
                std::string fileName = line.substr((line.find("Comparing") + 10), (colonPos - line.find("Comparing") - 10));
                double similarity = std::stod(line.substr(colonPos + 1));

                // ����Comparison�ṹ�岢��ӵ��ȽϽ��������  
                Comparison comparison;
                comparison.fileName = fileName;
                comparison.similarity = similarity;
                comparisons.push_back(comparison);
            }
        }
    }

    _pclose(pipe);

    // �����ƶȽ�������ȽϽ��  
    std::sort(comparisons.begin(), comparisons.end(), compareSimilarity);

    // �Ӹߵ�������ļ��������ƶ�  
    int len = comparisons.size();
    for (int i = 0; i < len; ++i)
    {
        std::cout << "�ļ���: " << comparisons[i].fileName << ", ���ƶ�: " << comparisons[i].similarity << std::endl;
    }
}
