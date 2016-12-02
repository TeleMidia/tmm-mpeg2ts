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
 * Name.cpp
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/Name.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	Name::Name() {
		stream = NULL;
		idData = NULL;
		kindData = NULL;
		idLength = 0;
		kindLength = 0;
	}

	Name::~Name() {
		if (stream != NULL) {
			delete (stream);
		}
		if (idData != NULL) {
			delete (idData);
		}
		if (kindData != NULL) {
			delete (kindData);
		}
	}

	int Name::updateStream() {
		int len, pos = 0;

		len = getStreamLength();

		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[len];
		} catch(...) {
			return -1;
		}

		stream[pos++] = idLength;
		if (idData != NULL) {
			memcpy(stream + pos, idData, idLength);
		}
		pos += idLength;
		stream[pos++] = kindLength;
		if (kindData != NULL) {
			memcpy(stream + pos, kindData, kindLength);
		}
		pos += kindLength;

		return pos;
	}

	unsigned char Name::getIdLength() {
		return idLength;
	}

	int Name::getIdData(char** dataStream) {
		if (idData != NULL) {
			*dataStream = idData;
			return idLength;
		} else {
			return 0;
		}
	}

	unsigned char Name::getKindLength() {
		return kindLength;
	}

	int Name::getKindData(char** dataStream) {
		if (kindData != NULL) {
			*dataStream = kindData;
			return kindLength;
		} else {
			return 0;
		}
	}

	void Name::setIdData(const char* data, unsigned char length) {
		if (idData != NULL) {
			delete (idData);
		}
		try {
			idData = new char[length];
		} catch(...) {
			return;
		}
		memcpy(idData, data, length);
		idLength = length;
	}

	void Name::setKindData(const char* data, unsigned char length) {
		if (kindData != NULL) {
			delete (kindData);
		}
		try {
			kindData = new char[length];
		} catch(...) {
			return;
		}
		memcpy(kindData, data, length);
		kindLength = length;
	}

	int Name::getStreamLength() {
		return idLength + kindLength + 2;
	}

	int Name::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int Name::getStream(char** dataStream) {
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
