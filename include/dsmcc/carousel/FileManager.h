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

/*
 * FileManager.h
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include "util/functions.h"
#include "Biop/ObjectLocation.h"
using namespace br::pucrio::telemidia::mpeg2::util;

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <dirent.h>
#include <utility>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	class FileManager {

		private:

		protected:
			map<string,InternalIor*>* folderList;
			vector<string>* orderedFolderList;
			map<string,InternalIor*>* fileList;
			vector<pair<unsigned int, string> >* fileSizeList;
			map<string,unsigned int>* allItems;
			unsigned int key;

			void createOrderedFolderList();
			void clearIorList(map<string,InternalIor*>* list);

		public:
			FileManager();
			~FileManager();

			string readSrg(string path);
			bool setItemIor(const string& item, unsigned int carousel,
							unsigned short moduleId);
			bool getItemIor(const string& item, unsigned int* c, unsigned short* m,
					unsigned int* k);
			bool getItemIor(const string& item, InternalIor** iior);
			unsigned int getItemKey(const string& item);
			unsigned int getNumberOfItems();
			map<string,unsigned int>* getAllItems();
			vector<pair<unsigned int, string> >* getFileSizeList();
			vector<string>* getOrderedFolderList();
			unsigned int getNextAvailableKey();

			map<string,InternalIor*>* getFileList();
			void showAllItems();

			static vector<string> travelDir(const string& directory);
			static void travelAllDir(string directory,vector<string>* fullList);
			static bool removeFiles(const string& folder);
			static bool comparisonStringInt(const pair<string, int> &p1,
									 const pair<string, int> &p2);
			static bool comparisonUintString(const pair<unsigned int, string> &p1,
									 const pair<unsigned int, string> &p2);

	 };
}
}
}
}
}

#endif /* FILEMANAGER_H_ */
