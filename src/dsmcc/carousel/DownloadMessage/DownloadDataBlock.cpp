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
 * DownloadDataBlock.cpp
 *
 *  Created on: Jun 25, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/DownloadMessage/DownloadDataBlock.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	DownloadDataBlock::DownloadDataBlock() : MessageHeader() {
		protocolDiscriminator = 0x11;
		dsmccType = 0x03;
		messageId = 0x1003;
		blockDataByte = NULL;
		blockDataByteLength = 0;
		moduleId = 0;
		moduleVersion = 0;
		blockNumber = 0;
	}

	DownloadDataBlock::~DownloadDataBlock() {
		if (blockDataByte != NULL) {
			delete (blockDataByte);
		}
	}

	int DownloadDataBlock::updateStream() {
		int pos = MessageHeader::updateStream();

		stream[pos++] = (moduleId >> 8) & 0xFF;
		stream[pos++] = moduleId & 0xFF;
		stream[pos++] = moduleVersion & 0xFF;
		stream[pos++] = 0xFF;
		stream[pos++] = (blockNumber >> 8) & 0xFF;
		stream[pos++] = blockNumber & 0xFF;

		if (blockDataByte != NULL) {
			memcpy(stream + pos, blockDataByte, blockDataByteLength);
			pos += blockDataByteLength;
		}

		return pos;
	}

	unsigned int DownloadDataBlock::calculateStreamSize() {
		unsigned int pos = MessageHeader::calculateStreamSize();
		//include compatibilityDescriptor if compatibilityDescriptorLength > 0
		return pos + blockDataByteLength + 6;
	}

	unsigned short DownloadDataBlock::getModuleId() {
		return moduleId;
	}

	unsigned char DownloadDataBlock::getModuleVersion() {
		return moduleVersion;
	}

	unsigned short DownloadDataBlock::getBlockNumber() {
		return blockNumber;
	}

	unsigned int DownloadDataBlock::getBlockDataByte(char** dataStream) {
		if (blockDataByte != NULL) {
			*dataStream = blockDataByte;
			return blockDataByteLength;
		} else {
			return 0;
		}
	}

	unsigned int DownloadDataBlock::getBlockDataByteLength() {
		return blockDataByteLength;
	}

	void DownloadDataBlock::setModuleId(unsigned short id) {
		moduleId = id;
	}

	void DownloadDataBlock::setModuleVersion(unsigned char version) {
		moduleVersion = version;
	}

	void DownloadDataBlock::setBlockNumber(unsigned short number) {
		blockNumber = number;
	}

	int DownloadDataBlock::setBlockDataByte(char* data, int length) {
		if (blockDataByte != NULL) {
			delete (blockDataByte);
		}
		blockDataByteLength = length;
		messageLength = calculateStreamSize() - 12;
		try {
			blockDataByte = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(blockDataByte, data, length);
		return 0;
	}

}
}
}
}
}
