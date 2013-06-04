/*
 * StreamEventMessage.cpp
 *
 *  Created on: May 18, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/StreamEventMessage.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	StreamEventMessage::StreamEventMessage() : StreamMessage(){
		objectKindData[2] = 0x65;
		eventNameList = new vector<eventName*>;
	}

	StreamEventMessage::~StreamEventMessage() {
		if (eventNameList != NULL) {
			clearEventNameList();
			delete (eventNameList);
		}

	}

	int64_t StreamEventMessage::updateStream() {
		int pos = BiopMessage::updateStream();
		eventName* en;
		vector<eventName*>::iterator enIt;

		char* tempStr;
		int tempStrLen = objectInfo->getStream(&tempStr);
		memcpy(stream + pos, tempStr, tempStrLen);
		pos = pos + tempStrLen;

		if ((eventNameList != NULL) && (!eventNameList->empty())) {
			unsigned short int size = eventNameList->size();
			stream[pos++] = (size >> 8) & 0xFF;
			stream[pos++] = size & 0xFF;
			enIt = eventNameList->begin();
			while (enIt != eventNameList->end()) {
				en = *enIt;
				stream[pos++] = en->eventNameLength & 0xFF;
				memcpy(stream+pos, en->eventNameData, en->eventNameLength);
				pos = pos + en->eventNameLength;
				++enIt;
			}
		} else {
			stream[pos++] = 0;
			stream[pos++] = 0;
		}

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
				tempStrLen = tp->getStream(&tempStr);
				memcpy(stream + pos, tempStr, tempStrLen);
				pos = pos + tempStrLen;
				++tpIt;
			}
		}

		unsigned short int size = eventNameList->size();
		stream[pos++] = size & 0xFF;

		unsigned short int eId;
		enIt = eventNameList->begin();
		while (enIt != eventNameList->end()) {
			en = *enIt;
			eId = en->eventId;
			stream[pos++] = (eId >> 8) & 0xFF;
			stream[pos++] = eId & 0xFF;
			++enIt;
		}

		streamSize = pos;

		return pos;
	}

	unsigned int StreamEventMessage::calculateMessageSize() {
		unsigned int pos = StreamMessage::calculateMessageSize();
		eventName* en;
		vector<eventName*>::iterator enIt;
		enIt = eventNameList->begin();
		while (enIt != eventNameList->end()) {
			en = *enIt;
			pos += en->eventNameLength + 1;
			++enIt;
		}
		pos += (eventNameList->size() * 2);
		return pos + 3;
	}

	int StreamEventMessage::addEvent(
			unsigned short id, char* name, unsigned char length) {
		eventName* en;
		if (eventNameList->size() >= 65535) {
			return -1;
		}
		en = new eventName();
		en->eventId = id;
		en->eventNameLength = length;
		memcpy(en->eventNameData, name, length);
		eventNameList->push_back(en);
		return eventNameList->size();
	}

	void StreamEventMessage::clearEventNameList() {
		eventName* en;
		vector<eventName*>::iterator i;
		if ((eventNameList != NULL) && (!eventNameList->empty())) {
			i = eventNameList->begin();
			while (i != eventNameList->end()) {
				en = *i;
				delete (en);
				++i;
			}
			eventNameList->clear();
		}
	}
}
}
}
}
}
