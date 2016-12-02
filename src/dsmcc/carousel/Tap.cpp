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
 * Tap.cpp
 *
 *  Created on: May 14, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Tap.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	Tap::Tap() {
		stream = NULL;
		id = 0;
		use = 0x16;
		selectorLength = 0x0A;
		selectorType = 0x0001;
		assocTag = 0x0040;
		transactionId = 0x80100002;
		timeout = 0xFFFFFFFF;
	}

	Tap::~Tap() {
		if (stream != NULL) {
			delete (stream);
		}
	}

	int Tap::updateStream() {
		int pos = 0;
		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[17];
		} catch(...) {
			return -1;
		}
		stream[pos++] = (id >> 8) & 0xFF;
		stream[pos++] = id & 0xFF;
		stream[pos++] = (use >> 8) & 0xFF;
		stream[pos++] = use & 0xFF;
		stream[pos++] = (assocTag >> 8) & 0xFF;
		stream[pos++] = assocTag & 0xFF;
		stream[pos++] = selectorLength & 0xFF;
		if (selectorLength) {
			stream[pos++] = (selectorType >> 8) & 0xFF;
			stream[pos++] = selectorType & 0xFF;
			stream[pos++] = (transactionId >> 24) & 0xFF;
			stream[pos++] = (transactionId >> 16) & 0xFF;
			stream[pos++] = (transactionId >> 8) & 0xFF;
			stream[pos++] = transactionId & 0xFF;
			stream[pos++] = (timeout >> 24) & 0xFF;
			stream[pos++] = (timeout >> 16) & 0xFF;
			stream[pos++] = (timeout >> 8) & 0xFF;
			stream[pos++] = timeout & 0xFF;
		}

		return pos;
	}

	unsigned short Tap::getId() {
		return id;
	}

	unsigned short Tap::getUse() {
		return use;
	}

	unsigned short Tap::getAssocTag() {
		return assocTag;
	}

	unsigned char Tap::getSelectorLength() {
		return selectorLength;
	}

	unsigned short Tap::getSelectorType() {
		return selectorType;
	}

	unsigned int Tap::getTransactionId() {
		return transactionId;
	}

	unsigned int Tap::getTimeout() {
		return timeout;
	}

	void Tap::setId(unsigned short id) {
		this->id = id;
	}

	void Tap::setUse(unsigned short use) {
		this->use = use;
	}

	void Tap::setAssocTag(unsigned short at) {
		assocTag = at;
	}

	void Tap::setSelectorLength(unsigned short slen) {
		selectorLength = slen;
	}

	void Tap::setSelectorType(unsigned short st) {
		selectorType = st;
	}

	void Tap::setTransactionId(unsigned int tid) {
		transactionId = tid;
	}

	void Tap::setTimeout(unsigned int to) {
		timeout = to;
	}

	int Tap::getStreamLength() {
		int pos = 7;
		if (selectorLength) {
			pos += 10;
		}
		return pos;
	}

	int Tap::getStream(char* dataStream) {
		int pos = updateStream();
		if (pos > 0) {
			memcpy(dataStream, stream, pos);
			return pos;
		} else {
			return -1;
		}
	}

	int Tap::getStream(char** dataStream) {
		int pos = updateStream();
		if (pos > 0) {
			*dataStream = stream;
			return pos;
		} else {
			*dataStream = NULL;
			return -1;
		}
	}

}
}
}
}
}
