/*
 * ObjectLocation.cpp
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Biop/ObjectLocation.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	ObjectLocation::ObjectLocation() {
		componentIdTag = 0x49534F50;
		versionMajor = 1;
		versionMinor = 0;
		stream = NULL;
		componentDataLength = 0x0D;
		objectKeyLength = 4;
		iior = NULL;
	}

	ObjectLocation::~ObjectLocation() {
		if (stream != NULL) {
			delete (stream);
		}
	}

	int ObjectLocation::updateStream() {
		int pos = 0;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[getStreamLength()];
		} catch(...) {
			return -1;
		}

		if (!iior) return -2;

		stream[pos++] = (componentIdTag >> 24) & 0xFF;
		stream[pos++] = (componentIdTag >> 16) & 0xFF;
		stream[pos++] = (componentIdTag >> 8) & 0xFF;
		stream[pos++] = componentIdTag & 0xFF;
		stream[pos++] = componentDataLength & 0xFF;
		stream[pos++] = (iior->carousel >> 24) & 0xFF;
		stream[pos++] = (iior->carousel >> 16) & 0xFF;
		stream[pos++] = (iior->carousel >> 8) & 0xFF;
		stream[pos++] = iior->carousel & 0xFF;
		stream[pos++] = (iior->moduleId >> 8) & 0xFF;
		stream[pos++] = iior->moduleId & 0xFF;
		stream[pos++] = versionMajor & 0xFF;
		stream[pos++] = versionMinor & 0xFF;
		stream[pos++] = objectKeyLength & 0xFF;
		stream[pos++] = (iior->key >> 24) & 0xFF;
		stream[pos++] = (iior->key >> 16) & 0xFF;
		stream[pos++] = (iior->key >> 8) & 0xFF;
		stream[pos++] = iior->key & 0xFF;

		return pos;
	}

	unsigned int ObjectLocation::getComponentIdTag() {
		return componentIdTag;
	}

	unsigned char ObjectLocation::getComponentDataLength() {
		return componentDataLength;
	}

	unsigned int ObjectLocation::getCarouselId() {
		if (iior) {
			return iior->carousel;
		} else {
			return 0;
		}
	}

	unsigned short ObjectLocation::getModuleId() {
		if (iior) {
			return iior->moduleId;
		} else {
			return 0;
		}
	}

	unsigned char ObjectLocation::getVersionMajor() {
		return versionMajor;
	}

	unsigned char ObjectLocation::getVersionMinor() {
		return versionMinor;
	}

	unsigned char ObjectLocation::getObjectKeyLength() {
		return objectKeyLength;
	}

	unsigned int ObjectLocation::getObjectKeyData() {
		if (iior) {
			return iior->key;
		} else {
			return 0;
		}
	}

	InternalIor* ObjectLocation::getIior() {
		return iior;
	}

	void ObjectLocation::setIior(InternalIor* iior) {
		this->iior = iior;
	}

	void ObjectLocation::setVersionMajor(unsigned char version) {
		versionMajor = version;
	}

	void ObjectLocation::setVersionMinor(unsigned char version) {
		versionMinor = version;
	}

	int ObjectLocation::getStreamLength() {
		return objectKeyLength + 14;
	}

	int ObjectLocation::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int ObjectLocation::getStream(char** dataStream) {
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
