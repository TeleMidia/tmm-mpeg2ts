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
 * ServiceContext.cpp
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/ServiceContext.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	ServiceContext::ServiceContext() {
		contextId = 0;
		contextDataLength = 0;
		contextDataByte = NULL;
		stream = NULL;
	}

	ServiceContext::ServiceContext(
			unsigned int id, char* dataByte, unsigned short length) {
		try {
			stream = new char[length];
		} catch (...) {
			cout << "Error: ServiceContext" << endl;
		}
		memcpy(stream, dataByte, length);
		contextId = id;
		contextDataLength = length;
	}

	ServiceContext::~ServiceContext() {
		if (contextDataByte != NULL) {
			delete (contextDataByte);
		}
		if (stream != NULL) {
			delete (stream);
		}
	}

	void ServiceContext::process() {
		//TODO: Decode stream
	}

	int ServiceContext::updateStream() {
		int pos = 0;
		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[contextDataLength + 6];
		} catch(...) {
			return -1;
		}
		stream[pos++] = (contextId >> 24) & 0xFF;
		stream[pos++] = (contextId >> 16) & 0xFF;
		stream[pos++] = (contextId >> 8) & 0xFF;
		stream[pos++] = contextId & 0xFF;
		stream[pos++] = (contextDataLength >> 8) & 0xFF;
		stream[pos++] = contextDataLength & 0xFF;
		memcpy(stream + pos, contextDataByte, contextDataLength);
		pos = pos + contextDataLength;
		return pos;
	}

	unsigned int ServiceContext::getContextId() {
		return contextId;
	}

	unsigned short ServiceContext::getContextDataLength() {
		return contextDataLength;
	}

	unsigned short ServiceContext::getContextDataByte(char* dataByte) {
		memcpy(dataByte, contextDataByte, contextDataLength);
		return contextDataLength;
	}

	void ServiceContext::setContextId(unsigned int id) {
		contextId = id;
	}

    int ServiceContext::setContextDataByte(
    		char *dataByte, unsigned short  length) {
    	if (contextDataByte != NULL) {
			delete (contextDataByte);
		}
		try {
			contextDataByte = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(contextDataByte, dataByte, length);
		contextDataLength = length;
		return length;
    }

    int ServiceContext::getStreamLength() {
		return contextDataLength + 6;
	}

    int ServiceContext::getStream(char* dataStream) {
		if (updateStream() >= 0) {
			memcpy(dataStream, stream, contextDataLength + 6);
			return contextDataLength + 6;
		} else {
			return -1;
		}
    }

    int ServiceContext::getStream(char** dataStream) {
		if (updateStream() >= 0) {
			*dataStream = stream;
			return contextDataLength + 6;
		} else {
			return -1;
		}
	}

}
}
}
}
}
