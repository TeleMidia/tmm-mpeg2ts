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
 * ModuleManager.cpp
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/ModuleManager.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	ModuleManager::ModuleManager() {
		fm = new FileManager();
		moduleList = new map<unsigned short, Module*>;
		serviceGatewayFolder = "";
		tempFolder = "";
		seMessageList = NULL;
	}

	ModuleManager::~ModuleManager() {
		if (fm != NULL) {
			delete fm;
		}
		clearModuleList();
		if (moduleList != NULL) {
			delete moduleList;
		}
	}

	bool ModuleManager::readServiceGateway(const string& path) {
		serviceGatewayFolder = fm->readSrg(path);
		return (serviceGatewayFolder != "");
	}

	bool ModuleManager::setTempFolder(string path) {
		if ((path[path.size() - 1] == '\\') ||
				(path[path.size() - 1] == '/')) {
			path.erase(path.size() - 1, 1);
		}

		if (isDir(path) <= 0) {
			makeDir(path.c_str(), 0);
			if (isDir(path) <= 0) {
				cout << "ModuleManager::setTempFolder Invalid temp folder." << endl;
				return false;
			}
		}

		tempFolder = path;

		return true;
	}

	bool ModuleManager::saveModules() {
		Module* module = NULL;
		map<unsigned short, Module*>::iterator k;
		string filename;

		k = moduleList->begin();
		while (k != moduleList->end()) {
			module = k->second;
			filename = tempFolder + "/mod-" +
						intToStrHexa(module->getModuleId()) + ".bin";
			if (!module->saveModuleToFile(filename)) {
				return false;
			}
			++k;
		}
		return true;
	}

	void ModuleManager::createDummyIors(unsigned int serviceDomain) {
		map<string,unsigned int>* allItems;
		map<string,unsigned int>::iterator it;

		allItems = fm->getAllItems();
		it = allItems->begin();
		while (it != allItems->end()) {
			fm->setItemIor(it->first, serviceDomain, 0);
			++it;
		}
	}

	int ModuleManager::makeModules(unsigned int serviceDomain) {
		unsigned short moduleNum = 0;
		vector<pair<unsigned int, string> >* fileSizeList;
		vector<pair<unsigned int, string> >::iterator i;
		vector<StreamEventMessage*>::iterator itSem;
		vector<string>* orderedFolderList;
		vector<string>::iterator j;
		FileMessage* file;
		DirectoryMessage* dir;
		Module* module = NULL;
		unsigned int key;

		if (serviceGatewayFolder == "") {
			return -1;
		}
		if (tempFolder == "") {
			return -2;
		}

		createDummyIors(serviceDomain);

		//directories
		orderedFolderList = fm->getOrderedFolderList();
		j = orderedFolderList->begin();
		while (j != orderedFolderList->end()) {
			dir = new DirectoryMessage();
			if (createDirMessage(*j, dir)) {
				if (module == NULL) {
					module = new Module();
					moduleNum++;
					module->setModuleId(moduleNum);
				}
				if (module->addBiop(dir)) {
					//BIOP added
					fm->setItemIor(*j, serviceDomain, moduleNum);
				} else {
					//Module is full
					(*moduleList)[moduleNum] = module;
					module = new Module();
					moduleNum++;
					module->setModuleId(moduleNum);
					if (!module->addBiop(dir)) {
						delete dir;
						return -6;
					}
					fm->setItemIor(*j, serviceDomain, moduleNum);
				}
			} else {
				delete dir;
				return -5;
			}
			++j;
		}
		if (module) {
			(*moduleList)[moduleNum] = module;
			module = NULL;
		}

		//files
		fileSizeList = fm->getFileSizeList();
		i = fileSizeList->begin();
		while (i != fileSizeList->end()) {
			file = new FileMessage();
			if (createFileMessage((*i).second, file)) {
				if (module == NULL) {
					module = new Module();
					moduleNum++;
					module->setModuleId(moduleNum);
				}
				if (module->addBiop(file)) {
					//BIOP added
					fm->setItemIor((*i).second, serviceDomain, moduleNum);
				} else {
					//Module is full
					(*moduleList)[moduleNum] = module;
					module = new Module();
					moduleNum++;
					module->setModuleId(moduleNum);
					if (!module->addBiop(file)) {
						delete file;
						return -4;
					}
					fm->setItemIor((*i).second, serviceDomain, moduleNum);
				}
			} else {
				delete file;
				return -3;
			}
			++i;
		}
		if (module) {
			(*moduleList)[moduleNum] = module;
			module = NULL;
		}

		//StreamEvent
		key = fm->getNextAvailableKey();
		itSem = seMessageList->begin();
		while (itSem != seMessageList->end()) {
			(*itSem)->setObjectKeyData(key++);
			if (module == NULL) {
				module = new Module();
				moduleNum++;
				module->setModuleId(moduleNum);
			}
			if (!module->addBiop(*itSem)) {
				//Module is full
				(*moduleList)[moduleNum] = module;
				module = new Module();
				moduleNum++;
				module->setModuleId(moduleNum);
				if (!module->addBiop(*itSem)) {
					return -4;
				}
			}
			++itSem;
		}
		if (module) {
			(*moduleList)[moduleNum] = module;
			module = NULL;
		}

		if (!saveModules()) {
			return -7;
		}

		fm->showAllItems();

		return moduleList->size();
	}

	bool ModuleManager::createFileMessage(string filename, FileMessage* file) {

		file->setObjectKeyData(fm->getItemKey(filename));
		return file->setFilename(filename);
	}

	bool ModuleManager::createDirMessage(const string& path, DirectoryMessage* dir) {
		vector<string> folderContent = FileManager::travelDir(path);
		vector<string>::iterator i;
		string biopKind, value, fullFileName;
		string kindData;
		Binding* bind = NULL;
		ObjectLocation* ol = NULL;
		BiopProfileBody* bpb = NULL;
		ConnBinder* cb = NULL;
		Ior* ior = NULL;
		vector<TaggedProfile*>* tpl = NULL;
		vector<Binding*>* bindsList = NULL;
		int result;
		InternalIor* iior;

		i = folderContent.begin();
		while (i != folderContent.end()) {
			if (((*i) == ".") || ((*i) == "..")) {
				folderContent.erase(i);
				i = folderContent.begin();
				continue;
			}
			++i;
		}

		if (path == serviceGatewayFolder) {
			biopKind = "srg";
		} else {
			biopKind = "dir";
		}

		dir->setObjectKeyData(fm->getItemKey(path));
		dir->setObjectKindData((const char*) biopKind.c_str(), 4);

		bindsList = new vector<Binding*>;

		i = folderContent.begin();
		while (i != folderContent.end()) {
			bind = new Binding();
			fullFileName = path + getUriSlash() + *i;
			result = isDir(fullFileName);
			if (result == 1) {
				kindData = "dir";
				bind->setBindingType(2);
			} else if (result == 0) {
				kindData = "fil";
				bind->setBindingType(1);
			} else {
				cout << "Unexpected error!" << endl;
				delete bind;
				return false;
			}
			Name* name = new Name();
			value = *i;
			name->setIdData((const char*) value.c_str(), value.size() + 1);
			name->setKindData((const char*) kindData.c_str(), 4);
			bind->setName(name);
			ol = new ObjectLocation();
			if (!(fm->getItemIor(fullFileName, &iior))) {
				delete bind;
				cout << "Unexpected error." << endl;
				return false;
			}
			ol->setIior(iior);

			bpb = new BiopProfileBody();
			bpb->setObjectLocation(ol);

			Tap* tap = new Tap();
			tap->setTransactionId(transactionId);

			cb = new ConnBinder();
			cb->setTap(tap);
			bpb->setConnBinder(cb);

			tpl = new vector<TaggedProfile*>;
			tpl->push_back(bpb);

			ior = new Ior();

			ior->setTypeIdByte((const char*) kindData.c_str(), 4);
			ior->setTaggedProfileList(tpl);

			bind->setIor(ior);
			if (kindData == "fil") {
				bind->setContentSize(fileSize(fullFileName));
			}
			bindsList->push_back(bind);

			++i;
		}
		dir->setBindingsList(bindsList);
		return true;
	}

	int ModuleManager::getSrgIorStream(char** stream) {
		char* data;
		char* iorStream;
		int length;
		string kindData;
		ObjectLocation* ol = NULL;
		BiopProfileBody* bpb = NULL;
		ConnBinder* cb = NULL;
		Ior* ior = NULL;
		vector<TaggedProfile*>* tpl = NULL;
		InternalIor* iior;

		ol = new ObjectLocation();
		if (!(fm->getItemIor(serviceGatewayFolder, &iior))) {
			cout << "ModuleManager::getSrgIorStream - Service gateway not found." << endl;
			return -1;
		}
		ol->setIior(iior);

		bpb = new BiopProfileBody();
		bpb->setObjectLocation(ol);

		Tap* tap = new Tap();
		tap->setTransactionId(transactionId);

		cb = new ConnBinder();
		cb->setTap(tap);
		bpb->setConnBinder(cb);

		tpl = new vector<TaggedProfile*>;
		tpl->push_back(bpb);

		ior = new Ior();

		kindData = "srg";
		ior->setTypeIdByte((const char*) kindData.c_str(), 4);
		ior->setTaggedProfileList(tpl);

		length = ior->getStreamLength() + 4;
		data = new char[length];
		ior->getStream(&iorStream);
		memcpy(data, iorStream, length);
		memset(data + length - 4, 0x00, 4);

		*stream = data;
		return length;
	}

	string ModuleManager::getServiceGatewayFolder() {
		return serviceGatewayFolder;
	}

	void ModuleManager::setTransactionId(unsigned int id) {
		transactionId = id;
	}

	map<unsigned short, Module*>* ModuleManager::getModuleList() {
		return moduleList;
	}

	void ModuleManager::clearModuleList() {
		map<unsigned short, Module*>::iterator i;

		if (moduleList != NULL) {
			i = moduleList->begin();
			while (i != moduleList->end()) {
				delete i->second;
				++i;
			}
		}
		moduleList->clear();
	}

	FileManager* ModuleManager::getFileManager() {
		return fm;
	}

	void ModuleManager::setSeMessageList(vector<StreamEventMessage*>* seml) {
		seMessageList = seml;
	}

}
}
}
}
}
