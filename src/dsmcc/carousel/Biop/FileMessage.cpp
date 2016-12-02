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
 * FileMessage.cpp
 *
 *  Created on: Jun 8, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/FileMessage.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	FileMessage::FileMessage() : BiopMessage(){
		objectKindLength = 0x04;
		objectKindData[0] = 0x66;
		objectKindData[1] = 0x69;
		objectKindData[2] = 0x6C;
		objectKindData[3] = 0x00;
		contentSize = 0;
		contentLength = 0;
		objectInfoLength = 8;
		filename = "";
	}

	FileMessage::~FileMessage() {

	}

	int64_t FileMessage::updateStream() {
		int pos = BiopMessage::updateStream();
		char* tempStr;
		int tempStrLen;

		//rewriting messageSize
		messageSize = messageSize + contentLength;
		stream[8] = (messageSize >> 24) & 0xFF;
		stream[9] = (messageSize >> 16) & 0xFF;
		stream[10] = (messageSize >> 8) & 0xFF;
		stream[11] = messageSize & 0xFF;

		stream[pos++] = (contentSize >> 56) & 0xFF;
		stream[pos++] = (contentSize >> 48) & 0xFF;
		stream[pos++] = (contentSize >> 40) & 0xFF;
		stream[pos++] = (contentSize >> 32) & 0xFF;
		stream[pos++] = (contentSize >> 24) & 0xFF;
		stream[pos++] = (contentSize >> 16) & 0xFF;
		stream[pos++] = (contentSize >> 8) & 0xFF;
		stream[pos++] = contentSize & 0xFF;

		/* descriptors */

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

		stream[pos++] = (contentLength >> 24) & 0xFF;
		stream[pos++] = (contentLength >> 16) & 0xFF;
		stream[pos++] = (contentLength >> 8) & 0xFF;
		stream[pos++] = contentLength & 0xFF;

		return pos;
	}

	unsigned int FileMessage::calculateMessageSize() {
		unsigned int pos = BiopMessage::calculateMessageSize();

		ServiceContext* sc;
		vector<ServiceContext*>::iterator scIt;
		if ((serviceContextList != NULL) && (!serviceContextList->empty())) {
			scIt = serviceContextList->begin();
			while (scIt != serviceContextList->end()) {
				sc = *scIt;
				pos = pos + sc->getStreamLength();
				++scIt;
			}
		}

		pos = pos + 9; //contentLength not included to save memory

		return pos;
	}

	uint64_t FileMessage::getContentSize() {
		return contentSize;
	}

	unsigned int FileMessage::getContentLength() {
		return contentLength;
	}

	string FileMessage::getFilename() {
		return filename;
	}

	bool FileMessage::setFilename(const string& filename) {
		if (isDir(filename) != 0) {
			return false;
		}
		this->filename = filename;
		contentLength = fileSize(filename);
		contentSize = contentLength;
		messageBodyLength = contentLength + 4;
		messageSize = calculateMessageSize() + contentLength;
		streamSize = messageSize + 12;
		return true;
	}

	int64_t FileMessage::getSecondaryStreamLength() {
		if (filename != "") {
			return fileSize(filename);
		}
		return 0;
	}

	int64_t FileMessage::writeSecondaryStream(int fd) {
		int ofd, rw, rb;
		unsigned int size = 0;
		char buffer[1024];
		ofd = getReadFD(filename);
		if (ofd < 0) {
			return -1;
		}
		rb = read(ofd, buffer, 1024);
		while (rb != 0) {
			size = size + rb;
			rw = write(fd, buffer, rb);
			if (rw != rb) {
				close(ofd);
				return -2;
			}
			rb = read(ofd, buffer, 1024);
		}
		close(ofd);
		return size;
	}

}
}
}
}
}
