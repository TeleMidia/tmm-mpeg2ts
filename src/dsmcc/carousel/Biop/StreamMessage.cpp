/*
 * StreamMessage.cpp
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/StreamMessage.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	StreamMessage::StreamMessage() : BiopMessage() {
		objectKindLength = 0x04;
		objectKindData[0] = 0x73;
		objectKindData[1] = 0x74;
		objectKindData[2] = 0x72;
		objectKindData[3] = 0x00;
		objectInfo = NULL;
		objectInfoByte = NULL;
		objectInfoLength = 0;
		tapsList = new vector<Tap*>;
		tapsCount = 0;
		objectInfoByteLength = 0;
	}

	StreamMessage::~StreamMessage() {
		if (objectInfo != NULL) {
			delete (objectInfo);
		}
		if (this->tapsList != NULL) {
			clearTapsList();
			delete (tapsList);
		}
	}

	int64_t StreamMessage::updateStream() {
		int64_t pos = BiopMessage::updateStream();

		char* tempStr;
		if (objectInfo == NULL) {
			return -1;
		}
		int tempStrLen = objectInfo->getStream(&tempStr);
		memcpy(stream + pos, tempStr, tempStrLen);
		pos = pos + tempStrLen;

		memcpy(stream + pos, objectInfoByte, objectInfoByteLength);
		pos = pos + objectInfoByteLength;

		stream[pos++] = serviceContextListCount & 0xFF;

		ServiceContext* sc;
		vector<ServiceContext*>::iterator scIt;
		if ((serviceContextList != NULL) && (!serviceContextList->empty())) {
			scIt = serviceContextList->begin();
			while (scIt != serviceContextList->end()) {
				sc = *scIt;
				tempStrLen = sc->getStream(&tempStr);
				memcpy(stream + pos, tempStr, tempStrLen);
				pos = pos + tempStrLen;
				++scIt;
			}
		}

		stream[pos++] = (messageBodyLength >> 24) & 0xFF;
		stream[pos++] = (messageBodyLength >> 16) & 0xFF;
		stream[pos++] = (messageBodyLength >> 8) & 0xFF;
		stream[pos++] = messageBodyLength & 0xFF;
		stream[pos++] = tapsCount & 0xFF;

		Tap* tp;
		vector<Tap*>::iterator tpIt;
		if ((tapsList != NULL) && (!tapsList->empty())) {
			tpIt = tapsList->begin();
			while (tpIt != tapsList->end()) {
				tp = *tpIt;
				tempStrLen = tp->getStream(&tempStr) - 10;
				memcpy(stream + pos, tempStr, tempStrLen);
				pos = pos + tempStrLen;
				++tpIt;
			}
		}

		streamSize = pos;

		return pos;
	}

	unsigned int StreamMessage::calculateMessageSize() {
		unsigned int pos = BiopMessage::calculateMessageSize();
		pos += objectInfo->getStreamLength() + objectInfoLength;
		ServiceContext* sc;
		vector<ServiceContext*>::iterator scIt;
		if ((serviceContextList != NULL) && (!serviceContextList->empty())) {
			scIt = serviceContextList->begin();
			while (scIt != serviceContextList->end()) {
				sc = *scIt;
				pos += sc->getStreamLength();
				++scIt;
			}
		}
		if ((tapsList != NULL) && (!tapsList->empty())) {
			pos += (tapsList->size() * 7); /* (17 bytes per tap) */
		}
		return pos + 6;
	}

	StreamInfoT* StreamMessage::getObjectInfo() {
		return objectInfo;
	}

	int StreamMessage::getObjectInfoByte(char* data) {
		memcpy(data, objectInfoByte, objectInfoLength);
		return objectInfoLength;
	}

	unsigned char StreamMessage::getTapsCount() {
		return tapsCount;
	}

	vector<Tap*>* StreamMessage::getTapsList() {
		return tapsList;
	}

	void StreamMessage::setObjectInfo(StreamInfoT* oi) {
		if (objectInfo != NULL) {
			delete (objectInfo);
		}
		objectInfo = oi;
		if (objectInfo != NULL) {
			objectInfoLength = objectInfoByteLength +
				objectInfo->getDescriptionLength() + 10;
		} else {
			objectInfoLength = objectInfoByteLength + 10;
		}
	}

	int StreamMessage::setObjectInfoByte(char* data, unsigned short length) {
		if (objectInfoByte != NULL) {
			delete (objectInfoByte);
		}
		try {
			objectInfoByte = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(objectInfoByte, data, length);
		objectInfoByteLength = length;

		if (objectInfo != NULL) {
			objectInfoLength = objectInfoByteLength +
				objectInfo->getDescriptionLength() + 10;
		} else {
			objectInfoLength = objectInfoByteLength + 10;
		}

		return objectInfoLength;
	}

	void StreamMessage::setTapsList(vector<Tap*>* tl) {
		if (this->tapsList != NULL) {
			clearTapsList();
			delete (tapsList);
		}
		this->tapsList = tl;
		tapsCount = tapsList->size();
	}

	void StreamMessage::clearTapsList() {
		Tap* t;
		vector<Tap*>::iterator i;
		if ((tapsList != NULL) && (!tapsList->empty())) {
			i = tapsList->begin();
			while (i != tapsList->end()) {
				t = *i;
				delete (t);
				++i;
			}
			tapsList->clear();
		}
	}

}
}
}
}
}
