// ----------------------------------------------
// main.cpp
// ¹¦ÄÜÃèÊö£º²âÊÔ´úÂë
// ----------------------------------------------

#include"kjplagsimilarity.h"

#include<iostream>

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) // 从第二个参数开始遍历，排除程序名参数
    {
        std::string arg = argv[i];
        KJplagSimilarity* kJplagSimilarity = new KJplagSimilarity();

        if (arg == "--check")
        {
            // 调用函数1  
            std::string filePath = argv[i + 1]; // 获取下一个参数作为文件路径  
            kJplagSimilarity->checkDirectorySimilarity(filePath);
        }
        else if (arg == "--compare")
        {
            // 调用函数2  
            std::string directoryPath1 = argv[i + 1]; // 获取下一个参数作为第一个目录路径  
            std::string directoryPath2 = argv[i + 2]; // 获取下两个参数作为第二个目录路径  
            int similarityThreshold = std::stoi(argv[i + 3]); // 获取下三个参数作为相似度阈值  
            kJplagSimilarity->compareTwoDirectoryFileSimilarity(directoryPath1, directoryPath2, similarityThreshold);
        }

        delete kJplagSimilarity;
        kJplagSimilarity = nullptr;
    }

    return 0;
}
