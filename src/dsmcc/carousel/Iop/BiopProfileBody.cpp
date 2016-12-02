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
