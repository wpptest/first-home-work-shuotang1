// ------------------------------------------------------------
// 2023/10/10
// kjplagsimilarity.h
// 功能描述：检测给定目录下C++文件的文件相似度
// -------------------------------------------------------------

#ifndef __KJPLAGSIMILARITY_KJPLAGSIMILARITY_H__
#define __KJPLAGSIMILARITY_KJPLAGSIMILARITY_H__

#include<string>

struct Comparison
{
	std::string fileName;
	double similarity;
};

bool compareSimilarity(const Comparison& c1, const Comparison& c2);
void removeBomCharacter(std::string& filePath);   //如果该文件是UTF-8 BOM编码，将其转换为UTF-8编码

class KJplagSimilarity
{
public:
	KJplagSimilarity();
	~KJplagSimilarity();
	
	void processDirectory(const std::string& directoryPath);      //递归处理目录，如果该文件是UTF-8 BOM编码，将其转换为UTF-8编码

	//比较两个目录下文件的相似性,输出相似度最高的n个文件
	void compareTwoDirectoryFileSimilarity(std::string oldFilePath, std::string newFilePath,int n);                    
	void checkDirectorySimilarity(std::string filePath);          //给定指定目录，检测该目录各一级子目录之间的相似性
};

#endif 
