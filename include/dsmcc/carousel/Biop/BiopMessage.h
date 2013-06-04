/*
 * BiopMessage.h
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef BIOPMESSAGE_H_
#define BIOPMESSAGE_H_

#include "dsmcc/carousel/ServiceContext.h"
#include "util/functions.h"
using namespace br::pucrio::telemidia::mpeg2::util;

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class BiopMessage {

		public:
			static const unsigned int START_CODE_PREFIX = 0x42494F50;

		private:
			void clearServiceContentList();
			void process();

		protected:
			virtual int64_t updateStream();
			virtual unsigned int calculateMessageSize();

			unsigned int streamSize;

			unsigned char biopVersionMajor;
			unsigned char biopVersionMinor;
			unsigned char byteOrder;
			unsigned char messageType;
			unsigned int messageSize;
			unsigned char objectKeyLength; // 1 to 4
			unsigned int objectKeyData;
			unsigned int objectKindLength; //4
			char objectKindData[4];
			unsigned short objectInfoLength;
			unsigned char serviceContextListCount;
			vector<ServiceContext*>* serviceContextList;
			unsigned int messageBodyLength;
			char* stream;

		public:
			BiopMessage();
			virtual ~BiopMessage();

			unsigned char getBiopVersionMajor();
			unsigned char getBiopVersionMinor();
			unsigned char getByteOrder();
			unsigned char getMessageType();
			unsigned int getMessageSize();
			unsigned char getObjectKeyLength();
			unsigned int getObjectKeyData();
			unsigned int getObjectKindLength();
			int getObjectKindData(char** data);
			unsigned short getObjectInfoLength();
			unsigned char getServiceContextListCount();
			vector<ServiceContext*>* getServiceContextList();
			unsigned int getMessageBodyLength();
			void setBiopVersionMajor(unsigned char major);
			void setBiopVersionMinor(unsigned char minor);
			void setByteOrder(unsigned char order);
			void setMessageType(unsigned char type);
			void setObjectKeyData(unsigned int key);
			void setObjectKindData(const char* data, char length);
			void setServiceContextList(vector<ServiceContext*>* list);

			int64_t getPrimaryStreamLength();
			int64_t writePrimaryStream(int fd);
	 };
}
}
}
}
}

#endif /* BIOPMESSAGE_H_ */
