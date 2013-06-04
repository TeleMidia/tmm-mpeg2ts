/*
 * BiopProfileBody.cpp
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Iop/BiopProfileBody.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	BiopProfileBody::BiopProfileBody() : TaggedProfile() {
		profileIdTag = 0x49534F06;
		objectLocation = NULL;
		connBinder = NULL;
	}

	BiopProfileBody::~BiopProfileBody() {
		if (objectLocation != NULL) {
			delete (objectLocation);
		}
		if (connBinder != NULL) {
			delete (connBinder);
		}
	}

	int BiopProfileBody::updateStream() {
		int pos = TaggedProfile::updateStream();
		char* tempStr;
		int tempStrLen;

		if (objectLocation != NULL) {
			tempStrLen = objectLocation->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos = pos + tempStrLen;
		}

		if (connBinder != NULL) {
			tempStrLen = connBinder->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos = pos + tempStrLen;
		}

		/*if (liteComponent != NULL) {
			tempStrLen = liteComponent->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos = pos + tempStrLen;
		}*/

		return pos;
	}

	unsigned int BiopProfileBody::calculateStreamSize() {
		unsigned int pos = TaggedProfile::calculateStreamSize();
		if (objectLocation != NULL) {
			pos += objectLocation->getStreamLength();
		}

		if (connBinder != NULL) {
			pos += connBinder->getStreamLength();
		}
		return pos;
	}

	ObjectLocation* BiopProfileBody::getObjectLocation() {
		return objectLocation;
	}

	ConnBinder* BiopProfileBody::getConnBinder() {
		return connBinder;
	}

	void BiopProfileBody::setObjectLocation(ObjectLocation* ol) {
		if (objectLocation != NULL) {
			delete (objectLocation);
		}
		objectLocation = ol;
	}

	void BiopProfileBody::setConnBinder(ConnBinder* cb) {
		if (connBinder != NULL) {
			delete (connBinder);
		}
		connBinder = cb;
	}
}
}
}
}
}
