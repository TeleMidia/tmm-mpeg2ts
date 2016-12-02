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
 * ModuleManager.h
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include "dsmcc/carousel/Module.h"
#include "dsmcc/carousel/Biop/FileMessage.h"
#include "dsmcc/carousel/Biop/DirectoryMessage.h"
#include "dsmcc/carousel/Biop/StreamMessage.h"
#include "dsmcc/carousel/Biop/StreamEventMessage.h"
#include "dsmcc/carousel/FileManager.h"

#include <iostream>
#include <map>
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	class ModuleManager {
		private:

		protected:
			FileManager* fm;
			map<unsigned short, Module*>* moduleList;
			string tempFolder, serviceGatewayFolder;
			unsigned int transactionId;
			vector<StreamEventMessage*>* seMessageList;

			bool createFileMessage(string filename, FileMessage* file);
			bool createDirMessage(const string& path, DirectoryMessage* dir);
			void createDummyIors(unsigned int serviceDomain);
			bool saveModules();
			void clearModuleList();

		public:
			ModuleManager();
			~ModuleManager();

			bool readServiceGateway(const string& path);
			string getServiceGatewayFolder();
			int getSrgIorStream(char** stream);
			bool setTempFolder(string path);
			void setTransactionId(unsigned int id);
			int makeModules(unsigned int serviceDomain);
			map<unsigned short, Module*>* getModuleList();
			FileManager* getFileManager();
			void setSeMessageList(vector<StreamEventMessage*>* seml);

	 };
}
}
}
}
}

#endif /* MODULEMANAGER_H_ */
