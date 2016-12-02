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
 * DirectoryMessage.cpp
 *
 *  Created on: Jun 8, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/DirectoryMessage.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	DirectoryMessage::DirectoryMessage() : BiopMessage(){
		objectKindLength = 0x04;
		objectKindData[0] = 0x64;
		objectKindData[1] = 0x69;
		objectKindData[2] = 0x72;
		objectKindData[3] = 0x00;
		bindingsList = NULL;
		objectInfoData = NULL;
		bindingsCount = 0;
		objectInfoLength = 0;
	}

	DirectoryMessage::~DirectoryMessage() {
		if (objectInfoData != NULL) {
			delete (objectInfoData);
		}
		if (bindingsList != NULL) {
			clearBindingsList();
			delete (bindingsList);
		}
	}

	int64_t DirectoryMessage::updateStream() {
		int pos = BiopMessage::updateStream();
		char* tempStr;
		int tempStrLen;
		Binding* bind;

		if (objectInfoData != NULL) {
			memcpy(stream + pos, objectInfoData, objectInfoLength);
			pos += objectInfoLength;
		}

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

		stream[pos++] = (bindingsCount >> 8) & 0xFF;
		stream[pos++] = bindingsCount & 0xFF;

		vector<Binding*>::iterator bIt;
		if ((bindingsList != NULL) && (!bindingsList->empty())) {
			bIt = bindingsList->begin();
			while (bIt != bindingsList->end()) {
				bind = *bIt;
				tempStrLen = bind->getStream(&tempStr);
				memcpy(stream + pos, tempStr, tempStrLen);
				pos = pos + tempStrLen;
				++bIt;
			}
		}

		return pos;
	}

	unsigned int DirectoryMessage::calculateMessageSize() {
		unsigned int pos = BiopMessage::calculateMessageSize();

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

		Binding* bind;
		vector<Binding*>::iterator bIt;
		if ((bindingsList != NULL) && (!bindingsList->empty())) {
			bIt = bindingsList->begin();
			while (bIt != bindingsList->end()) {
				bind = *bIt;
				pos += bind->getStreamLength();
				++bIt;
			}
		}
		return pos + 7;
	}

	unsigned int DirectoryMessage::calculateMessageBodyLength() {
		unsigned int pos = 0;
		Binding* bind;
		vector<Binding*>::iterator bIt;
		if ((bindingsList != NULL) && (!bindingsList->empty())) {
			bIt = bindingsList->begin();
			while (bIt != bindingsList->end()) {
				bind = *bIt;
				pos += bind->getStreamLength();
				++bIt;
			}
		}
		return pos + 2;
	}

	int DirectoryMessage::getObjectInfoData(char* data) {
		memcpy(data, objectInfoData, getObjectInfoLength());
		return getObjectInfoLength();
	}

	unsigned char DirectoryMessage::getBindingsCount() {
		return bindingsCount;
	}

	vector<Binding*>* DirectoryMessage::getBindingsList() {
		return bindingsList;
	}

	int DirectoryMessage::setObjectInfoData(char* data, unsigned short length) {
		if (objectInfoData != NULL) {
			delete (objectInfoData);
		}
		try {
			objectInfoData = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(objectInfoData, data, length);
		objectInfoLength = length;
		return length;
	}

	void DirectoryMessage::setBindingsList(vector<Binding*>* bl) {
		if (this->bindingsList != NULL) {
			clearBindingsList();
			delete (bindingsList);
		}
		this->bindingsList = bl;

		bindingsCount = bindingsList->size();
		messageBodyLength = calculateMessageBodyLength();
		streamSize = calculateMessageSize() + 12;
	}

	void DirectoryMessage::clearBindingsList() {
		Binding* b;
		vector<Binding*>::iterator i;
		if ((bindingsList != NULL) && (!bindingsList->empty())) {
			i = bindingsList->begin();
			while (i != bindingsList->end()) {
				b = *i;
				delete (b);
				++i;
			}
			bindingsList->clear();
		}
	}
}
}
}
}
}
