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
			bool createDirMessage(string path, DirectoryMessage* dir);
			void createDummyIors(unsigned int serviceDomain);
			bool saveModules();
			void clearModuleList();

		public:
			ModuleManager();
			~ModuleManager();

			bool readServiceGateway(string path);
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
