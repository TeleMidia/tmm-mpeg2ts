/*
 * MessageHeader.cpp
 *
 *  Created on: Jun 26, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/MessageHeader.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	MessageHeader::MessageHeader() {
		stream = NULL;
		protocolDiscriminator = 0;
		dsmccType = 0;
		messageId = 0;
		adaptationLength = 0;
		messageLength = 0;
	}

	MessageHeader::~MessageHeader() {
		if (stream != NULL) {
			delete (stream);
		}
	}

	int MessageHeader::updateStream() {
		int pos = 0, len;
		if (stream != NULL) {
			delete (stream);
		}
		len = calculateStreamSize();
		messageLength = len - 12;
		try {
			stream = new char[len];
		} catch(...) {
			return -1;
		}

		stream[pos++] = protocolDiscriminator & 0xFF;
		stream[pos++] = dsmccType & 0xFF;
		stream[pos++] = (messageId >> 8) & 0xFF;
		stream[pos++] = messageId & 0xFF;
		stream[pos++] = (extensionId >> 24) & 0xFF;
		stream[pos++] = (extensionId >> 16) & 0xFF;
		stream[pos++] = (extensionId >> 8) & 0xFF;
		stream[pos++] = extensionId & 0xFF;
		stream[pos++] = 0xFF;
		stream[pos++] = adaptationLength & 0xFF;
		stream[pos++] = (messageLength >> 8) & 0xFF;
		stream[pos++] = messageLength & 0xFF;

		return pos;
	}

	unsigned int MessageHeader::calculateStreamSize() {
		return 12;
	}

	unsigned char MessageHeader::getProtocolDiscriminator() {
		return protocolDiscriminator;
	}

	unsigned char MessageHeader::getDsmccType() {
		return dsmccType;
	}

	unsigned short MessageHeader::getMessageId() {
		return messageId;
	}

	unsigned int MessageHeader::getExtensionId() {
		return extensionId;
	}

	unsigned char MessageHeader::getAdaptationLength() {
		return adaptationLength;
	}

	unsigned short MessageHeader::getMessageLength() {
		return messageLength;
	}

	void MessageHeader::setProtocolDiscriminator(unsigned char pd) {
		protocolDiscriminator = pd;
	}

	void MessageHeader::setDsmccType(unsigned char type) {
		dsmccType = type;
	}

	void MessageHeader::setMessageId(unsigned short id) {
		messageId = id;
	}

	void MessageHeader::setExtensionId(unsigned int id) {
		extensionId = id;
	}

	int MessageHeader::getStreamLength() {
		return calculateStreamSize();
	}

	int MessageHeader::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int MessageHeader::getStream(char** dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			*dataStream = stream;
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
