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
 * DownloadInfoIndication.h
 *
 *  Created on: Jun 25, 2009
 *      Author: Felippe Nagato
 */

#ifndef DOWNLOADINFOINDICATION_H_
#define DOWNLOADINFOINDICATION_H_

#include "dsmcc/carousel/MessageHeader.h"
#include "dsmcc/carousel/DownloadMessage/ModuleInfo.h"

#include <vector>
#include <iostream>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class DownloadInfoIndication : public MessageHeader {
		private:

		protected:
			unsigned int downloadId;
			unsigned short blockSize;
			unsigned char windowSize;
			unsigned char ackPeriod;
			unsigned int tCDownloadWindow;
			unsigned int tCDownloadScenario;
			unsigned short numberOfModules;

			vector<ModuleInfo*>* moduleInfoList;

			unsigned short privateDataLength;
			char* privateDataByte;

			int updateStream();
			unsigned int calculateStreamSize();
			void clearModuleInfoList();

		public:
			DownloadInfoIndication();
			~DownloadInfoIndication();

			unsigned int getDownloadId();
			unsigned short getBlockSize();
			unsigned char getWindowSize();
			unsigned char getAckPeriod();
			unsigned int getTCDownloadWindow();
			unsigned int getTCDownloadScenario();
			unsigned short getNumberOfModules();
			vector<ModuleInfo*>* getModuleInfoList();
			unsigned short getPrivateDataLength();
			int getPrivateDataByte(char** dataStream);

			void setDownloadId(unsigned int id);
			void setBlockSize(unsigned short size);
			void setWindowSize(unsigned char size);
			void setAckPeriod(unsigned char period);
			void setTCDownloadWindow(unsigned int dw);
			void setTCDownloadScenario(unsigned int dc);
			void setNumberOfModules(unsigned short num);
			void setModuleInfoList(vector<ModuleInfo*>* mil);
			int setPrivateDataByte(char* data, int length);
	 };
}
}
}
}
}

#endif /* DOWNLOADINFOINDICATION_H_ */
