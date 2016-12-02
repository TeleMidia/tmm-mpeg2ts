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
 * TaggedProfile.cpp
 *
 *  Created on: Jun 17, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Iop/TaggedProfile.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	TaggedProfile::TaggedProfile() {
		stream = NULL;
		liteComponentCount = 2;
		profileIdTag = 0;
		profileDataLength = 0;
		profileDataByteOrder = 0;
	}

	TaggedProfile::~TaggedProfile() {
		if (stream != NULL) {
			delete (stream);
		}
	}

	int TaggedProfile::updateStream() {
		int pos = 0;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			int len = calculateStreamSize();
			stream = new char[len];
		} catch(...) {
			return -1;
		}
		stream[pos++] = (profileIdTag >> 24) & 0xFF;
		stream[pos++] = (profileIdTag >> 16) & 0xFF;
		stream[pos++] = (profileIdTag >> 8) & 0xFF;
		stream[pos++] = profileIdTag & 0xFF;
		stream[pos++] = (profileDataLength >> 24) & 0xFF;
		stream[pos++] = (profileDataLength >> 16) & 0xFF;
		stream[pos++] = (profileDataLength >> 8) & 0xFF;
		stream[pos++] = profileDataLength & 0xFF;
		stream[pos++] = profileDataByteOrder & 0xFF;
		stream[pos++] = liteComponentCount & 0xFF;

		return pos;
	}

	unsigned int TaggedProfile::calculateStreamSize() {
		return 10 /*+ liteComponents*/;
	}

	unsigned int TaggedProfile::getProfileIdTag() {
		return profileIdTag;
	}

	unsigned int TaggedProfile::getProfileDataLength() {
		profileDataLength = calculateStreamSize() - 8;
		return profileDataLength;
	}

	unsigned char TaggedProfile::getProfileDataByteOrder() {
		return profileDataByteOrder;
	}

	unsigned char TaggedProfile::getLiteComponentCount() {
		return liteComponentCount;
	}

	void TaggedProfile::setProfileDataByteOrder(unsigned char pdbo) {
		profileDataByteOrder = pdbo;
	}

	void TaggedProfile::setLiteComponentCount(unsigned char count) {
		liteComponentCount = count;
	}

	int TaggedProfile::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int TaggedProfile::getStream(char** dataStream) {
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
