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
