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
 * ConnBinder.cpp
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/ConnBinder.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	ConnBinder::ConnBinder() {
		componentIdTag = 0x49534F40;
		tap = NULL;
		addicionalTapByte = NULL;
		stream = NULL;
		componentDataLength = 0;
		tapsCount = 0;
	}

	ConnBinder::~ConnBinder() {
		if (tap != NULL) {
			delete (tap);
		}
		if (addicionalTapByte != NULL) {
			delete (addicionalTapByte);
		}
		if (stream != NULL) {
			delete (stream);
		}
	}

	int ConnBinder::updateStream() {
		char* tempStr;
		int pos = 0, tempStrLen;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[getStreamLength()];
		} catch(...) {
			return -1;
		}

		stream[pos++] = (componentIdTag >> 24) & 0xFF;
		stream[pos++] = (componentIdTag >> 16) & 0xFF;
		stream[pos++] = (componentIdTag >> 8) & 0xFF;
		stream[pos++] = componentIdTag & 0xFF;
		stream[pos++] = componentDataLength & 0xFF;
		stream[pos++] = tapsCount & 0xFF;

		if (tap != NULL) {
			tempStrLen = tap->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos = pos + tempStrLen;
		}
		if (addicionalTapByte != NULL) {
			memcpy(stream + pos, addicionalTapByte, componentDataLength - 18);
			pos = pos + (componentDataLength - 18);
		}

		return pos;
	}

	unsigned int ConnBinder::getComponentIdTag() {
		return componentIdTag;
	}

	unsigned char ConnBinder::getComponentDataLength() {
		return componentDataLength;
	}

	unsigned char ConnBinder::getTapsCount() {
		return tapsCount;
	}

	Tap* ConnBinder::getTap() {
		return tap;
	}

	unsigned char ConnBinder::getAddicionalTapByte(char** dataStream) {
		if (addicionalTapByte != NULL) {
			*dataStream = addicionalTapByte;
			return componentDataLength - 18;
		} else {
			return 0;
		}
	}

	void ConnBinder::setTapsCount(unsigned char count) {
		tapsCount = count;
	}

	void ConnBinder::setTap(Tap* tp) {
		if (tap != NULL) {
			delete (tap);
		}
		tap = tp;
		tapsCount = 1;
		if (addicionalTapByte == NULL) {
			componentDataLength = 18;
		}
	}

	void ConnBinder::setAddicionalTapByte(char* data, unsigned char length) {
		if (addicionalTapByte != NULL) {
			delete (addicionalTapByte);
		}
		try {
			addicionalTapByte = new char[length];
		} catch(...) {
			return;
		}
		memcpy(addicionalTapByte, data, length);
		componentDataLength = length + 18;
	}

	int ConnBinder::getStreamLength() {
		return componentDataLength + 5;
	}

	int ConnBinder::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int ConnBinder::getStream(char** dataStream) {
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
