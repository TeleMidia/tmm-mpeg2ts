/*
 * Binding.cpp
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Binding.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	Binding::Binding() {
		name = NULL;
		ior = NULL;
		descriptorByte = NULL;
		stream = NULL;
		nameComponentsCount = 1;
		bindingType = 0;
		objectInfoLength = 0;
		contentSize = 0;
	}

	Binding::~Binding() {
		if (name != NULL) {
			delete (name);
		}
		if (ior != NULL) {
			delete (ior);
		}
		if (descriptorByte != NULL) {
			delete (descriptorByte);
		}
		if (stream != NULL) {
			delete (stream);
		}
	}

	int Binding::updateStream() {
		char* tempStr;
		int tempStrLen;
		int pos = 0, len;
		bool filType = false;
		string kindStr;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			len = getStreamLength();
			stream = new char[len];
		} catch(...) {
			return -1;
		}

		stream[pos++] = nameComponentsCount & 0xFF;

		if (name != NULL) {
			tempStrLen = name->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos += tempStrLen;
			tempStrLen = name->getKindData(&tempStr);
			kindStr.assign(tempStr, 3);
			if (kindStr == "fil") {
				filType = true;
			}
		}

		stream[pos++] = bindingType & 0xFF;

		if (ior != NULL) {
			tempStrLen = ior->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos += tempStrLen;
		}

		stream[pos++] = (objectInfoLength >> 8) & 0xFF;
		stream[pos++] = objectInfoLength & 0xFF;

		if (objectInfoLength != 0) {
			if (filType) {
				stream[pos++] = (contentSize >> 56) & 0xFF;
				stream[pos++] = (contentSize >> 48) & 0xFF;
				stream[pos++] = (contentSize >> 40) & 0xFF;
				stream[pos++] = (contentSize >> 32) & 0xFF;
				stream[pos++] = (contentSize >> 24) & 0xFF;
				stream[pos++] = (contentSize >> 16) & 0xFF;
				stream[pos++] = (contentSize >> 8) & 0xFF;
				stream[pos++] = contentSize & 0xFF;
				if (descriptorByte != NULL) {
					memcpy(stream + pos, descriptorByte, objectInfoLength - 8);
					pos += (objectInfoLength - 8);
				}
			} else {
				if (descriptorByte != NULL) {
					memcpy(stream + pos, descriptorByte, objectInfoLength);
					pos += objectInfoLength;
				}
			}
		}

		return pos;
	}

	unsigned char Binding::getNameComponentsCount() {
		return nameComponentsCount;
	}

	Name* Binding::getName() {
		return name;
	}

	unsigned char Binding::getBindingType() {
		return bindingType;
	}

	Ior* Binding::getIor() {
		return ior;
	}

	unsigned short Binding::getObjectInfoLength() {
		return objectInfoLength;
	}

	uint64_t Binding::getContentSize() {
		return contentSize;
	}

	int Binding::getDescriptorByte(char** dataStream) {
		char* tempStr;
		if (descriptorByte != NULL) {
			*dataStream = descriptorByte;
			if (name != NULL) {
				name->getKindData(&tempStr);
				if (strcmp(tempStr, "fil") == 0) {
					return objectInfoLength - 8;
				} else {
					return objectInfoLength;
				}
			} else {
				return objectInfoLength - 8;
			}
		} else {
			return 0;
		}
	}

	void Binding::setNameComponentsCount(unsigned char count) {
		nameComponentsCount = count;
	}

	void Binding::setName(Name* name) {
		if (this->name != NULL) {
			delete (this->name);
		}
		this->name = name;
	}

	void Binding::setBindingType(unsigned char bt) {
		bindingType = bt;
	}

	void Binding::setIor(Ior* ior) {
		if (this->ior != NULL) {
			delete (this->ior);
		}
		this->ior = ior;
	}

	void Binding::setContentSize(uint64_t cs) {
		contentSize = cs;
		objectInfoLength = 8;
		//TODO: this causes a error if descriptorByte exists.
	}

	void Binding::setDescriptorByte(char* data, int length) {
		if (descriptorByte != NULL) {
			delete (descriptorByte);
			try {
				descriptorByte = new char[length];
			} catch(...) {
				return;
			}
		}
		memcpy(descriptorByte, data, length);
		if (contentSize != 0) { //this can cause a error.
			objectInfoLength = length + 8;
		} else {
			objectInfoLength = length;
		}
	}

	int Binding::getStreamLength() {
		int len = objectInfoLength + 4;
		if (name != NULL) {
			len += name->getStreamLength();
		}
		if (ior != NULL) {
			len += ior->getStreamLength();
		}
		return len;
	}

	int Binding::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int Binding::getStream(char** dataStream) {
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
