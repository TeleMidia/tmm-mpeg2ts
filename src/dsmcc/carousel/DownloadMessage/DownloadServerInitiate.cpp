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
 * DownloadServerInitiate.cpp
 *
 *  Created on: Jun 25, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/DownloadMessage/DownloadServerInitiate.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	DownloadServerInitiate::DownloadServerInitiate() : MessageHeader() {
		protocolDiscriminator = 0x11;
		dsmccType = 0x03;
		messageId = 0x1006;
		extensionId = 0x80000002;
		privateDataByte = NULL;
		privateDataLength = 0;
	}

	DownloadServerInitiate::~DownloadServerInitiate() {
		if (privateDataByte != NULL) {
			delete (privateDataByte);
		}
	}

	int DownloadServerInitiate::updateStream() {
		int pos = MessageHeader::updateStream();

		if (serverId != NULL) {
			memcpy(stream + pos, serverId, 20);
			pos += 20;
		}

		//compatibilityDescriptorLength
		stream[pos++] = 0x00;
		stream[pos++] = 0x00;

		stream[pos++] = (privateDataLength >> 8) & 0xFF;
		stream[pos++] = privateDataLength & 0xFF;

		if (privateDataByte != NULL) {
			memcpy(stream + pos, privateDataByte, privateDataLength);
			pos += privateDataLength;
		}

		return pos;
	}

	unsigned int DownloadServerInitiate::calculateStreamSize() {
		unsigned int pos = MessageHeader::calculateStreamSize();
		//include compatibilityDescriptor if compatibilityDescriptorLength > 0
		return pos + privateDataLength + 22 + 2;
	}

	int DownloadServerInitiate::getServerId(char** dataStream) {
		*dataStream = serverId;
		return 20;
	}

	unsigned short DownloadServerInitiate::getPrivateDataLength() {
		return privateDataLength;
	}

	int DownloadServerInitiate::getPrivateDataByte(char** dataStream) {
		if (privateDataByte != NULL) {
			*dataStream = privateDataByte;
			return privateDataLength;
		} else {
			return 0;
		}
	}

	void DownloadServerInitiate::setServerId(char* data, unsigned char length) {
		if (length > 20) length = 20;
		memcpy(serverId, data, length);
		messageLength = calculateStreamSize() - 12;
	}

	int DownloadServerInitiate::setPrivateDataByte(char* data,
			unsigned short length) {
		if (privateDataByte != NULL) {
			delete (privateDataByte);
		}
		privateDataLength = length;
		messageLength = calculateStreamSize() - 12;
		try {
			privateDataByte = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(privateDataByte, data, length);
		return 0;
	}

}
}
}
}
}
