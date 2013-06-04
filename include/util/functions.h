#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <limits>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <ctime>

#include <sys/timeb.h>
#include <sys/stat.h>
#include <wchar.h>

using namespace std;

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace util {
	long fileSize(string szFileName);
	int isDir(string szFileName);
	void makeDir(const char* dirName, unsigned int mode);
  	string intToStrHexa(int value);
  	int strHexaToInt(string value);
	string upperCase(string s);
	string lowerCase(string s);
	bool isNumeric(void* value);
	string itos(double i);
	float stof(string s);
	float itof(int i);
	string extractFilename(string filename);
	string absoluteFile(string basePath, string filename);
	string getPath(string filename);
	vector<string>* split(string str, string token);
	string trim(string str);
	float getPercentualValue(string value);
	bool isPercentualValue(string value);
	bool fileExists(string filename);
	void setDocumentPath(string docPath);
	string getDocumentPath();
	string getCurrentPath();
	double NaN();
	double infinity();
	bool isNaN(double value);
	bool isInfinity(double value);
	void initializeCurrentPath();
	double getCurrentTimeMillis();
	bool isAbsolutePath(string path);
	time_t makeDateTime(int yyyy, int mm, int dd, int hh, int nn, int ss);
	int getReadFD(string fileName);
	int getWriteFD(string fileName);
}
}
}
}
}

#endif //_FUNCTIONS_H_
