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

    // 读取文件前3个字节  
    char bom[3];
    file.read(bom, 3);

    // 检查是否是UTF-8 BOM  
    if (!(file && bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF))
        return;              // 文件不是UTF-8 BOM编码 
    
    // 文件是UTF-8 BOM编码,读取文件内容  
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::wstring utf16_content = converter.from_bytes(content);
    file.close();

    // 保存转换后的内容到文件  
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
                    processDirectory(subdirectoryPath);  // 递归处理子目录  
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

        // 获取父目录路径  
        std::size_t lastSlash = filePath.find_last_of("/\\");
        std::string parentPath = filePath.substr(0, lastSlash);

        // 切换到工作路径  
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
                // 提取文件名和相似度  
                size_t colonPos = line.find_last_of(":");
                std::string fileName = line.substr((line.find("Comparing") + 10), (colonPos - line.find("Comparing") - 10));
                double similarity = std::stod(line.substr(colonPos + 1));

                // 创建Comparison结构体并添加到比较结果向量中  
                Comparison comparison;
                comparison.fileName = fileName;
                comparison.similarity = similarity;
                comparisons.push_back(comparison);
            }
        }
    }

    _pclose(pipe);

    // 按相似度降序排序比较结果  
    std::sort(comparisons.begin(), comparisons.end(), compareSimilarity);

    // 输出相似度最高的前n个文件名和相似度  
    int len = min(comparisons.size(), n);
    for (int i = 0; i < len; ++i) 
    {
        std::cout << "文件名: " << comparisons[i].fileName << ", 相似度: " << comparisons[i].similarity << std::endl;
    }
}

void KJplagSimilarity::checkDirectorySimilarity(std::string filePath)
{
    char fileBuffer[_MAX_PATH];
    if (_fullpath(fileBuffer, __FILE__, _MAX_PATH) == nullptr)
        return;

    std::string filePathTemp = fileBuffer;

    // 获取父目录路径  
    std::size_t lastSlash = filePathTemp.find_last_of("/\\");
    std::string parentPath = filePathTemp.substr(0, lastSlash);

    // 切换到工作路径  
    if (_chdir(parentPath.c_str()) != 0)
        return;

    //比较相似度前三的命令
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
                // 提取文件名和相似度  
                size_t colonPos = line.find_last_of(":");
                std::string fileName = line.substr((line.find("Comparing") + 10), (colonPos - line.find("Comparing") - 10));
                double similarity = std::stod(line.substr(colonPos + 1));

                // 创建Comparison结构体并添加到比较结果向量中  
                Comparison comparison;
                comparison.fileName = fileName;
                comparison.similarity = similarity;
                comparisons.push_back(comparison);
            }
        }
    }

    _pclose(pipe);

    // 按相似度降序排序比较结果  
    std::sort(comparisons.begin(), comparisons.end(), compareSimilarity);

    // 从高到低输出文件名和相似度  
    int len = comparisons.size();
    for (int i = 0; i < len; ++i)
    {
        std::cout << "文件名: " << comparisons[i].fileName << ", 相似度: " << comparisons[i].similarity << std::endl;
    }
}
