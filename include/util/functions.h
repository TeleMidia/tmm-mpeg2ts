/******************************************************************************
TeleMídia Multiplexer MPEG-2 Transport Stream library
Copyright (C) 2016 TeleMídia Lab/PUC-Rio
https://github.com/TeleMidia/tmm-mpeg2ts/graphs/contributors

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
for more details. You should have received a copy of the GNU Affero General
Public License along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

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
	string getUriSlash();
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
