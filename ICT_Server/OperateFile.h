#pragma once
#include <fstream>
using namespace std;
class OperateFile
{
public:
	OperateFile(void);
	~OperateFile(void);

private:
	
	fstream ReadOut;
public:
	bool SaveDebugFile(char *ptr);
	bool ReadConfig(char *filePath,char *ipAddr,int &port);
	bool ReadSerialFile(char *filePath,string &str);
};

