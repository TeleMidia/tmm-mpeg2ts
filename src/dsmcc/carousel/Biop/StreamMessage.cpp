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
		objectInfoByteLength = 0;
	}

	StreamMessage::~StreamMessage() {
		if (objectInfo != NULL) {
			delete (objectInfo);
		}
		clearTapsList();
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
		stream[pos++] = tapsList.size() & 0xFF;

		Tap* tp;
		vector<Tap*>::iterator tpIt;
		tpIt = tapsList.begin();
		while (tpIt != tapsList.end()) {
			tp = *tpIt;
			tp->setSelectorLength(0);
			tempStrLen = tp->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos = pos + tempStrLen;
			++tpIt;
		}

		streamSize = pos;

		return pos;
	}

	unsigned int StreamMessage::calculateMessageSize() {
		unsigned int pos = BiopMessage::calculateMessageSize();
		vector<Tap*>::iterator i;

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

		messageBodyLength = 1;
		i = tapsList.begin();
		while (i != tapsList.end()) {
			pos += (*i)->getStreamLength();
			messageBodyLength += (*i)->getStreamLength();
			++i;
		}

		return pos + 6;
	}

	StreamInfoT* StreamMessage::getObjectInfo() {
		return objectInfo;
	}

	int StreamMessage::getObjectInfoByte(char* data) {
		memcpy(data, objectInfoByte, objectInfoByteLength);
		return objectInfoByteLength;
	}

	unsigned char StreamMessage::getTapsCount() {
		return tapsList.size();
	}

	vector<Tap*>* StreamMessage::getTapsList() {
		return &tapsList;
	}

	void StreamMessage::setObjectInfo(StreamInfoT* oi) {
		if (objectInfo != NULL) {
			delete (objectInfo);
		}
		objectInfo = oi;
		if (objectInfo != NULL) {
			objectInfoLength = objectInfoByteLength +
				objectInfo->getStreamLength();
		} else {
			objectInfoLength = objectInfoByteLength + 12;
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
				objectInfo->getStreamLength();
		} else {
			objectInfoLength = objectInfoByteLength + 12;
		}

		return objectInfoByteLength;
	}

	void StreamMessage::addTap(Tap* tap) {
		tapsList.push_back(tap);
	}

	void StreamMessage::clearTapsList() {
		vector<Tap*>::iterator i;
		i = tapsList.begin();
		while (i != tapsList.end()) {
			delete (*i);
			++i;
		}
		tapsList.clear();
	}

}
}
}
}
}
