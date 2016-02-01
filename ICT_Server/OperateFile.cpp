#include "StdAfx.h"
#include "OperateFile.h"
#include "OperateFile.h"
#include <time.h>
#include <stdio.h>
#include <string>


OperateFile::OperateFile(void)
{
}


OperateFile::~OperateFile(void)
{
}


bool OperateFile::SaveDebugFile(char *ptr)
{
	fstream logOut("log.txt",ios::out);
	if (logOut.is_open())
	{
		logOut << ptr << endl;
	}
	else
	{
		logOut.close();
		return false;
	}
	logOut.close();
	return true;
}


bool OperateFile::ReadConfig(char *filePath,char *ipAddr,int &port)
{
	fstream readFile(filePath,ios::in);
	string temp;
	if (readFile.is_open())
	{
		while(getline(readFile,temp))
		{
			int index = (int)temp.find("=");
			if (index == -1)
			{
				continue;
			}
			string key = temp.substr(0,index);
			if (-1 != key.find("IPAddress"))
			{
				strcpy(ipAddr,temp.substr(index+1).c_str());
			}
			else if(strstr(temp.c_str(), "port"))
			{
				port = atoi(temp.substr(index+1).c_str());
			}
		}
	}
	else
	{
		SaveDebugFile("Read config file fail");
		readFile.close();
		return false;
	}
	readFile.close();
	return true;
}


bool OperateFile::ReadSerialFile(char *filePath,string &str)
{
	fstream readFile(filePath,ios::in);
	string temp;
	if (readFile.is_open())
	{
		while(getline(readFile,temp))
		{
			str = temp;
		}
	}
	else
	{
		readFile.close();
		return false;
	}
	readFile.close();
	return true;
}
