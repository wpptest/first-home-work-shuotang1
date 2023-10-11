// ------------------------------------------------------------
// 2023/10/10
// kjplagsimilarity.h
// ����������������Ŀ¼��C++�ļ����ļ����ƶ�
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
void removeBomCharacter(std::string& filePath);   //������ļ���UTF-8 BOM���룬����ת��ΪUTF-8����

class KJplagSimilarity
{
public:
	KJplagSimilarity();
	~KJplagSimilarity();
	
	void processDirectory(const std::string& directoryPath);      //�ݹ鴦��Ŀ¼��������ļ���UTF-8 BOM���룬����ת��ΪUTF-8����

	//�Ƚ�����Ŀ¼���ļ���������,������ƶ���ߵ�n���ļ�
	void compareTwoDirectoryFileSimilarity(std::string oldFilePath, std::string newFilePath,int n);                    
	void checkDirectorySimilarity(std::string filePath);          //����ָ��Ŀ¼������Ŀ¼��һ����Ŀ¼֮���������
};

#endif 
