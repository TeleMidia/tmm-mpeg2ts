/*
 * ModuleInfo.cpp
 *
 *  Created on: Jun 29, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/DownloadMessage/ModuleInfo.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	ModuleInfo::ModuleInfo() {
		moduleInfoByte = NULL;
		stream = NULL;
		moduleId = 0;
		moduleSize = 0;
		moduleVersion = 0;
		moduleInfoLength = 0;
	}

	ModuleInfo::~ModuleInfo() {
		if (moduleInfoByte != NULL) {
			delete (moduleInfoByte);
		}
		if (stream != NULL) {
			delete (stream);
		}
	}

	int ModuleInfo::updateStream() {
		int pos = 0, len;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			len = getStreamLength();
			stream = new char[len];
		} catch(...) {
			return -1;
		}

		stream[pos++] = (moduleId >> 8) & 0xFF;
		stream[pos++] = moduleId & 0xFF;
		stream[pos++] = (moduleSize >> 24) & 0xFF;
		stream[pos++] = (moduleSize >> 16) & 0xFF;
		stream[pos++] = (moduleSize >> 8) & 0xFF;
		stream[pos++] = moduleSize & 0xFF;
		stream[pos++] = moduleVersion & 0xFF;
		stream[pos++] = moduleInfoLength & 0xFF;

		if (moduleInfoByte != NULL) {
			memcpy(stream + pos, moduleInfoByte, moduleInfoLength);
			pos += moduleInfoLength;
		}

		return pos;
	}

	unsigned short ModuleInfo::getModuleId() {
		return moduleId;
	}

	unsigned int ModuleInfo::getModuleSize() {
		return moduleSize;
	}

	unsigned char ModuleInfo::getModuleVersion() {
		return moduleVersion;
	}

	unsigned char ModuleInfo::getModuleInfoLength() {
		return moduleInfoLength;
	}

	int ModuleInfo::getModuleInfoByte(char** dataStream) {
		if (moduleInfoByte != NULL) {
			*dataStream = moduleInfoByte;
			return moduleInfoLength;
		} else {
			return 0;
		}
	}

	void ModuleInfo::setModuleId(unsigned short id) {
		moduleId = id;
	}

	void ModuleInfo::setModuleSize(unsigned int size) {
		moduleSize = size;
	}

	void ModuleInfo::setModuleVersion(unsigned char version) {
		moduleVersion = version;
	}

	int ModuleInfo::setModuleInfoByte(char* data, int length) {
		if (moduleInfoByte != NULL) {
			delete (moduleInfoByte);
		}
		moduleInfoLength = length;
		try {
			moduleInfoByte = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(moduleInfoByte, data, length);
		return 0;
	}

	int ModuleInfo::getStreamLength() {
		return moduleInfoLength + 8;
	}

	int ModuleInfo::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int ModuleInfo::getStream(char** dataStream) {
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
