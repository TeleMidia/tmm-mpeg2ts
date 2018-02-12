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
 * Carousel.h
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#ifndef CAROUSEL_H_
#define CAROUSEL_H_

#include "util/functions.h"
#include "TSPacket.h"
#include "dsmcc/DSMCCSection.h"
#include "dsmcc/carousel/Module.h"
#include "dsmcc/carousel/ModuleManager.h"
#include "dsmcc/carousel/DownloadMessage/DownloadServerInitiate.h"
#include "dsmcc/carousel/DownloadMessage/DownloadInfoIndication.h"
#include "dsmcc/carousel/DownloadMessage/DownloadDataBlock.h"
#include "dsmcc/carousel/DownloadMessage/ModuleInfo.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace br::pucrio::telemidia::mpeg2::util;


namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	class Carousel {
		private:
			unsigned char continuityCounter;

		protected:
			unsigned short pid;
			int bitrate;
			unsigned short blockSize;
			unsigned int serviceDomain;
			unsigned int transactionId;
			unsigned char carouselVersion;
			vector<StreamEventMessage*> seMessageList;
			bool sectionEncapsulationMode;
			string serviceGatewayFolder;
			DownloadServerInitiate* dsi;
			DownloadInfoIndication* dii;

			ModuleManager* moduleManager;

			int makeDSI();
			int makeDII();
			int encapsulateCarousel(const string& outputFile,
									const string& tempFolder);
			/*int encapsulateCarouselNP(const string& outputFile,
									  const string& tempFolder);*/
			int writeSections(int fd, vector<DSMCCSection*>* section);
			void clearSectionList(vector<DSMCCSection*>* sectionList);
			void checkMinBlockSize();

		public:
			Carousel();
			~Carousel();

			void setPid(unsigned short pid);
			unsigned short getPid();
			void setBitrate(int bitrate);
			int getBitrate();
			unsigned short getBlockSize();
			unsigned int getServiceDomain();
			unsigned int getDownloadId();
			unsigned int getTransactionId();
			unsigned char getCarouselVersion();
			string getServiceGatewayFolder();

			void setBlockSize(unsigned short size);
			void setServiceDomain(unsigned int sd);
			void setDownloadId(unsigned int id);
			void setTransactionId(unsigned int id);
			void setCarouselVersion(unsigned char v);
			void setServiceGatewayFolder(const string& path);
			void addStreamEventMessage(StreamEventMessage* sem);

			bool getSectionEncapsulationMode();
			void setSectionEncapsulationMode(bool active);

			int createCarousel(string outputFile, const string& tempFolder);
			map<string, InternalIor*>* getFilesIor();
	 };
}
}
}
}
}

#endif /* CAROUSEL_H_ */
