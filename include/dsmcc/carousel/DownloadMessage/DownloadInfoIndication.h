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
