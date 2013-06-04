/*
 * BiopMessage.cpp
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/BiopMessage.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	BiopMessage::BiopMessage() {
		streamSize = 0;
		stream = NULL;
		biopVersionMajor = 0x01;
		biopVersionMinor = 0x00;
		byteOrder = 0x00;
		messageType = 0x00;
		serviceContextList = new vector<ServiceContext*>;
		messageSize = calculateMessageSize();
		objectKeyLength = 4;
		objectKindLength = 4;
		serviceContextListCount = 0;
		messageBodyLength = 0;
	}

	BiopMessage::~BiopMessage() {
		if (serviceContextList != NULL) {
			clearServiceContentList();
			delete (serviceContextList);
		}
		if (stream != NULL) {
			delete (stream);
		}
	}

	void BiopMessage::process() {
		//TODO: Decode stream
	}

	int64_t BiopMessage::updateStream() {
		int64_t pos = 0;
		if (stream != NULL) {
			delete (stream);
		}
		try {
			messageSize = calculateMessageSize();
			streamSize = messageSize + 12;
			stream = new char[streamSize];
		} catch(...) {
			return -1;
		}
		stream[pos++] = (START_CODE_PREFIX >> 24) & 0xFF;
		stream[pos++] = (START_CODE_PREFIX >> 16) & 0xFF;
		stream[pos++] = (START_CODE_PREFIX >> 8) & 0xFF;
		stream[pos++] = START_CODE_PREFIX & 0xFF;
		stream[pos++] = biopVersionMajor & 0xFF;
		stream[pos++] = biopVersionMinor & 0xFF;
		stream[pos++] = byteOrder & 0xFF;
		stream[pos++] = messageType & 0xFF;
		stream[pos++] = (messageSize >> 24) & 0xFF;
		stream[pos++] = (messageSize >> 16) & 0xFF;
		stream[pos++] = (messageSize >> 8) & 0xFF;
		stream[pos++] = messageSize & 0xFF;
		stream[pos++] = objectKeyLength & 0xFF;
		stream[pos++] = (objectKeyData >> 24) & 0xFF;
		stream[pos++] = (objectKeyData >> 16) & 0xFF;
		stream[pos++] = (objectKeyData >> 8) & 0xFF;
		stream[pos++] = objectKeyData & 0xFF;
		stream[pos++] = (objectKindLength >> 24) & 0xFF;
		stream[pos++] = (objectKindLength >> 16) & 0xFF;
		stream[pos++] = (objectKindLength >> 8) & 0xFF;
		stream[pos++] = objectKindLength & 0xFF;
		for (unsigned int i = 0; i < objectKindLength; i++) {
			stream[pos++] = objectKindData[i] & 0xFF;
		}
		stream[pos++] = (objectInfoLength >> 8) & 0xFF;
		stream[pos++] = objectInfoLength & 0xFF;

		return pos;
	}

	unsigned int BiopMessage::calculateMessageSize() {
		return objectInfoLength + objectKindLength + 11;
	}

	unsigned char BiopMessage::getBiopVersionMajor() {
		return biopVersionMajor;
	}

	unsigned char BiopMessage::getBiopVersionMinor() {
		return biopVersionMinor;
	}

	unsigned char BiopMessage::getByteOrder() {
		return byteOrder;
	}

	unsigned char BiopMessage::getMessageType() {
		return messageType;
	}

	unsigned int BiopMessage::getMessageSize() {
		messageSize = calculateMessageSize();
		return messageSize;
	}

	unsigned char BiopMessage::getObjectKeyLength() {
		return objectKeyLength;
	}

	unsigned int BiopMessage::getObjectKeyData() {
		return objectKeyData;
	}

	unsigned int BiopMessage::getObjectKindLength() {
		return objectKindLength;
	}

	int BiopMessage::getObjectKindData(char** data) {
		*data = objectKindData;
		return 4;
	}

	unsigned short BiopMessage::getObjectInfoLength() {
		return objectInfoLength;
	}

	unsigned char BiopMessage::getServiceContextListCount() {
		return serviceContextListCount;
	}

	vector<ServiceContext*>* BiopMessage::getServiceContextList() {
		return serviceContextList;
	}

	unsigned int BiopMessage::getMessageBodyLength() {
		return messageBodyLength;
	}

	void BiopMessage::setBiopVersionMajor(unsigned char major) {
		biopVersionMajor = major;
	}

	void BiopMessage::setBiopVersionMinor(unsigned char minor) {
		biopVersionMinor = minor;
	}

	void BiopMessage::setByteOrder(unsigned char order) {
		byteOrder = order;
	}

	void BiopMessage::setMessageType(unsigned char type) {
		messageType = type;
	}

	void BiopMessage::setObjectKeyData(unsigned int key) {
		objectKeyData = key;
	}

	void BiopMessage::setObjectKindData(const char* data, char length) {
		memcpy(objectKindData, data, length);
		objectKindLength = length;
	}

	void BiopMessage::setServiceContextList(vector<ServiceContext*>* list) {
		if (serviceContextList != NULL) {
			clearServiceContentList();
			delete (serviceContextList);
		}
		serviceContextList = list;
		serviceContextListCount = serviceContextList->size();
	}

	void BiopMessage::clearServiceContentList() {
		ServiceContext* sc;
		vector<ServiceContext*>::iterator i;
		if ((serviceContextList != NULL) && (!serviceContextList->empty())) {
			i = serviceContextList->begin();
			while (i != serviceContextList->end()) {
				sc = *i;
				delete (sc);
				++i;
			}
			serviceContextList->clear();
		}
	}

	int64_t BiopMessage::getPrimaryStreamLength() {
		return calculateMessageSize() + 12;
	}

	int64_t BiopMessage::writePrimaryStream(int fd) {
		int slen = updateStream();
		int rw;
		if (slen >= 0) {
			rw = write(fd, stream, streamSize);
			if (rw != slen) {
				return -2;
			}
			return slen;
		} else {
			return -1;
		}
	}

}
}
}
}
}
