// ----------------------------------------------
// main.cpp
// �������������Դ���
// ----------------------------------------------

#include"kjplagsimilarity.h"

#include<iostream>

int main()
{
	KJplagSimilarity* kJplagSimilarity = new KJplagSimilarity();

	//kJplagSimilarity->processDirectory("D:\\cpp-professional-23-tasks\\huangqiping\\ClassTask\\KMemoryMapProject\\processB");

	//kJplagSimilarity->checkDirectorySimilarity("D:\\test");
	kJplagSimilarity->compareTwoDirectoryFileSimilarity("D:\\cpp-professional-23-tasks\\chengda1\\ClassTask", "D:\\cpp-professional-23-tasks\\chenhao21\\ClassTask",3);
	
	delete kJplagSimilarity;
	kJplagSimilarity = nullptr;

	return 0;
}