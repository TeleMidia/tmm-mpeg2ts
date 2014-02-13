/*
 * FileManager.cpp
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/FileManager.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	FileManager::FileManager() {
		folderList = new map<string,InternalIor*>;
		orderedFolderList = new vector<string>;
		fileList = new map<string,InternalIor*>;
		fileSizeList = new vector<pair<unsigned int, string> >;
		allItems = new map<string,unsigned int>;
		key = 0;
	}

	FileManager::~FileManager() {
		if (folderList != NULL) {
			clearIorList(folderList);
			delete folderList;
		}
		if (orderedFolderList != NULL) {
			delete orderedFolderList;
		}
		if (fileList != NULL) {
			clearIorList(fileList);
			delete fileList;
		}
		if (fileSizeList != NULL) {
			delete fileSizeList;
		}
		if (allItems != NULL) {
			delete allItems;
		}
	}

	string FileManager::readSrg(string path) {
		int result;
		vector<string> all;
		vector<string>::iterator it;

		if ((path[path.size() - 1] == '\\') ||
				(path[path.size() - 1] == '/')) {
			path.erase(path.size() - 1, 1);
		}

		if (path[path.size() - 1] == ':') {
			return "";
		}

		if (isDir(path) <= 0) {
			return "";
		}

		clearIorList(folderList);
		clearIorList(fileList);
		allItems->clear();
		fileSizeList->clear();
		key = 0;

		all.push_back(path);
		travelAllDir(path, &all);

		for (it = all.begin(); it < all.end(); it++) {
			key++;
			(*allItems)[*it] = key;
			result = isDir(*it);
			switch (result) {
			case 0:
				(*fileList)[*it] = NULL;
				fileSizeList->push_back(make_pair(fileSize(*it), *it));
				break;
			case 1:
				(*folderList)[*it] = NULL;
				break;
			default:
				clearIorList(folderList);
				clearIorList(fileList);
				allItems->clear();
				fileSizeList->clear();
				return "";
			}
		}

		key++;

		createOrderedFolderList();

		sort(fileSizeList->begin(), fileSizeList->end(), comparisonUintString);

		return path;
	}

	bool FileManager::setItemIor(string item, unsigned int carousel,
			unsigned short moduleId) {
		map<string,InternalIor*>::iterator i;
		InternalIor* ior = NULL;
		unsigned int key = 0;

		key = getItemKey(item);
		if (key == 0) {
			return false;
		}

		i = folderList->find(item);
		if (i == folderList->end()) {
			i = fileList->find(item);
			if (i == fileList->end()) {
				return false;
			}
		}

		if (i->second != NULL) {
			i->second->carousel = carousel;
			i->second->moduleId = moduleId;
			i->second->key = key;
		} else {
			ior = new InternalIor;
			ior->carousel = carousel;
			ior->moduleId = moduleId;
			ior->key = key;
			i->second = ior;
		}

		return true;
	}

	bool FileManager::getItemIor(string item, unsigned int* c,
			unsigned short* m, unsigned int* k) {
		map<string,InternalIor*>::iterator i;

		i = folderList->find(item);
		if (i == folderList->end()) {
			i = fileList->find(item);
			if (i == fileList->end()) {
				return false;
			}
		}

		if (i->second) {
			*c = i->second->carousel;
			*m = i->second->moduleId;
			*k = i->second->key;
		}

		return true;
	}

	bool FileManager::getItemIor(string item, InternalIor** iior) {
		map<string,InternalIor*>::iterator i;

		i = folderList->find(item);
		if (i == folderList->end()) {
			i = fileList->find(item);
			if (i == fileList->end()) {
				return false;
			}
		}

		if (!i->second) {
			InternalIor* ior = NULL;
			ior = new InternalIor;
			ior->carousel = 0;
			ior->moduleId = 0;
			ior->key = 0;
			i->second = ior;
		}

		*iior = i->second;

		return true;
	}

	unsigned int FileManager::getItemKey(string item) {
		map<string,unsigned int>::iterator i;

		if (allItems != NULL) {
			i = allItems->find(item);
			if (i != allItems->end()) {
				return i->second;
			}
		}

		return 0;

	}

	unsigned int FileManager::getNumberOfItems() {
		return allItems->size();
	}

	map<string,unsigned int>* FileManager::getAllItems() {
		return allItems;
	}

	vector<pair<unsigned int, string> >* FileManager::getFileSizeList() {
		return fileSizeList;
	}

	vector<string>* FileManager::getOrderedFolderList() {
		return orderedFolderList;
	}

	unsigned int FileManager::getNextAvailableKey() {
		return key;
	}

	void FileManager::createOrderedFolderList() {
		size_t found;
		vector<pair<string, int> > v;
		map<string,InternalIor*>::iterator it;
		int counter;
		unsigned int i;

		orderedFolderList->clear();

		it = folderList->begin();
		while (it != folderList->end()) {
			counter = 0;
			found = it->first.find_first_of("/\\");
			while (found != string::npos) {
				counter++;
				found = it->first.find_first_of("/\\", found + 1);
			}
			v.push_back(make_pair(it->first, counter));
			++it;
		}

		sort(v.begin(), v.end(), comparisonStringInt);

		for(i = 0; i < v.size(); ++i) orderedFolderList->push_back(v[i].first);

	}

	bool FileManager::comparisonStringInt(const pair<string, int> &p1,
								   const pair<string, int> &p2) {
		return p1.second < p2.second;
	}

	bool FileManager::comparisonUintString(const pair<unsigned int, string> &p1,
										   const pair<unsigned int, string> &p2) {
		return p1.first < p2.first;
	}

	void FileManager::clearIorList(map<string,InternalIor*>* list) {
		map<string,InternalIor*>::iterator i;
		if (list == NULL) {
			return;
		}
		i = list->begin();
		while (i != list->end()) {
			delete i->second;
			++i;
		}
		list->clear();
	}

	map<string,InternalIor*>* FileManager::getFileList() {
		return fileList;
	}

	void FileManager::showAllItems() {
		map<string,InternalIor*>::iterator i;

		i = fileList->begin();
		while (i != fileList->end()) {
			cout << i->first << " - ";
			if (i->second == NULL) {
				cout << "?" << endl;
			} else {
				cout << i->second->carousel << ", ";
				cout << i->second->moduleId << ", ";
				cout << i->second->key << endl;
			}
			++i;
		}

		i = folderList->begin();
		while (i != folderList->end()) {
			cout << i->first << " - ";
			if (i->second == NULL) {
				cout << "?" << endl;
			} else {
				cout << i->second->carousel << ", ";
				cout << i->second->moduleId << ", ";
				cout << i->second->key << endl;
			}
			++i;
		}
	}

	vector<string> FileManager::travelDir(string directory) {
		// travel thru a directory gathering all the file and directory names
		vector<string> fileList;
		DIR *dir;
		struct dirent *ent;

		// open a directory
		if ((dir = opendir(directory.c_str())) != NULL) {
			while((ent = readdir(dir)) != NULL) {
				// loop until the directory is traveled thru
				// push directory or filename to the list
				fileList.push_back(ent->d_name);
			}
			// close up
			closedir(dir);
		}
		//return the filelist
		return fileList;
	}

	void FileManager::travelAllDir(string directory, vector<string>* fullList) {
		// get the "root" directory's directories
		vector<string> fileList = travelDir(directory);

		if ((directory[directory.size() - 1] == '\\') ||
				(directory[directory.size() - 1] == '/')) {
			directory.erase(directory.size() - 1, 1);
		}

		// loop thru the list
		for (vector<string>::iterator i = fileList.begin();
				i != fileList.end(); ++i) {
			// test for . and .. directories (this and back)
			if (strcmp((*i).c_str(), ".") && strcmp((*i).c_str(), "..")) {
				string fullname = directory + getUriSlash() + (*i);
				fullList->push_back(fullname);
				travelAllDir(fullname, fullList);
			}
		}
	}

	bool FileManager::removeFiles(string folder) {
		struct dirent *next_file;
		DIR *theFolder;
		struct stat filestat;

		char filepath[256];

		if ((theFolder = opendir(folder.c_str())) != NULL) {
			while ((next_file = readdir(theFolder)) != NULL )
			{
				// build the full path for each file in the folder
				sprintf(filepath, "%s%s", folder.c_str(), next_file->d_name);
				if (next_file->d_name[0] == '.') continue;
				if (S_ISDIR( filestat.st_mode )) continue;
				remove(filepath);
			}
			return true;
		}
		return false;
	}

}
}
}
}
}
