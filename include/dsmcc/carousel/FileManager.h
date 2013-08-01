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

			void createOrderedFolderList();
			void clearIorList(map<string,InternalIor*>* list);

		public:
			FileManager();
			~FileManager();

			string readSrg(string path);
			bool setItemIor(string item, unsigned int carousel,
							unsigned short moduleId);
			bool getItemIor(string item, unsigned int* c, unsigned short* m,
					unsigned int* k);
			bool getItemIor(string item, InternalIor** iior);
			unsigned int getItemKey(string item);
			unsigned int getNumberOfItems();
			map<string,unsigned int>* getAllItems();
			vector<pair<unsigned int, string> >* getFileSizeList();
			vector<string>* getOrderedFolderList();

			map<string,InternalIor*>* getFileList();
			void showAllItems();

			static vector<string> travelDir(string directory);
			static void travelAllDir(string directory,vector<string>* fullList);
			static bool removeFiles(string folder);
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
