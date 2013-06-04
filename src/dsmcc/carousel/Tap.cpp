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

		return 17;
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
		return 17;
	}

	int Tap::getStream(char* dataStream) {
		if (updateStream() >= 0) {
			memcpy(dataStream, stream, 17);
			return 17;
		} else {
			return -1;
		}
	}

	int Tap::getStream(char** dataStream) {
		if (updateStream() >= 0) {
			*dataStream = stream;
			return 17;
		} else {
			return -1;
		}
	}

}
}
}
}
}
