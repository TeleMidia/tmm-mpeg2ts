/*
 * Ior.cpp
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Iop/Ior.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	Ior::Ior() {
		stream = NULL;
		taggedProfileList = new vector<TaggedProfile*>;
		typeIdByte = NULL;
		typeIdLength = 0;
		taggedProfileCount = 0;
	}

	Ior::~Ior() {
		if (stream != NULL) {
			delete (stream);
		}
		if (typeIdByte != NULL) {
			delete (typeIdByte);
		}
		if (taggedProfileList != NULL) {
			clearTaggedProfileList();
			delete (taggedProfileList);
		}
	}

	int Ior::updateStream() {
		int len, pos = 0;
		char* tempStr;
		int tempStrLen;
		TaggedProfile* tp;
		vector<TaggedProfile*>::iterator itTp;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			len = getStreamLength();
			stream = new char[len];
		} catch(...) {
			return -1;
		}

		stream[pos++] = (typeIdLength >> 24) & 0xFF;
		stream[pos++] = (typeIdLength >> 16) & 0xFF;
		stream[pos++] = (typeIdLength >> 8) & 0xFF;
		stream[pos++] = typeIdLength & 0xFF;

		if (typeIdByte != NULL) {
			memcpy(stream + pos, typeIdByte, typeIdLength);
			pos += typeIdLength;
		}

		stream[pos++] = (taggedProfileCount >> 24) & 0xFF;
		stream[pos++] = (taggedProfileCount >> 16) & 0xFF;
		stream[pos++] = (taggedProfileCount >> 8) & 0xFF;
		stream[pos++] = taggedProfileCount & 0xFF;


		itTp = taggedProfileList->begin();
		while (itTp != taggedProfileList->end()) {
			tp = *itTp;
			tempStrLen = tp->getStream(&tempStr);
			memcpy(stream + pos, tempStr, tempStrLen);
			pos = pos + tempStrLen;
			++itTp;
		}

		return pos;
	}

	unsigned int Ior::getTypeIdLength() {
		return typeIdLength;
	}

	int Ior::getTypeIdByte(char** dataStream) {
		if (typeIdByte != NULL) {
			*dataStream = typeIdByte;
			return typeIdLength;
		} else {
			return 0;
		}
	}

	unsigned int Ior::getTaggedProfileCount() {
		return taggedProfileCount;
	}

	vector<TaggedProfile*>* Ior::getTaggedProfileList() {
		return taggedProfileList;
	}

	void Ior::setTypeIdByte(const char* data, unsigned int length) {
		if (typeIdByte != NULL) {
			delete (typeIdByte);
		}
		try {
			typeIdByte = new char[length];
		} catch(...) {
			return;
		}
		memcpy(typeIdByte, data, length);
		typeIdLength = length;
	}

	void Ior::setTaggedProfileCount(unsigned int count) {
		taggedProfileCount = count;
	}

	void Ior::setTaggedProfileList(vector<TaggedProfile*>* tp) {
		if (taggedProfileList != NULL) {
			delete (taggedProfileList);
		}
		taggedProfileList = tp;
		taggedProfileCount = taggedProfileList->size();
	}

	int Ior::getStreamLength() {
		int len = 8;
		TaggedProfile* tp;
		vector<TaggedProfile*>::iterator tpIt;
		len += typeIdLength;

		if ((taggedProfileList != NULL) && (!taggedProfileList->empty())) {
			tpIt = taggedProfileList->begin();
			while (tpIt != taggedProfileList->end()) {
				tp = *tpIt;
				len += tp->getProfileDataLength() + 8;
				++tpIt;
			}
		}
		return len;
	}

	int Ior::getStream(char* dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			memcpy(dataStream, stream, slen);
			return slen;
		} else {
			return -1;
		}
	}

	int Ior::getStream(char** dataStream) {
		int slen = updateStream();
		if (slen >= 0) {
			*dataStream = stream;
			return slen;
		} else {
			return -1;
		}
	}

	void Ior::clearTaggedProfileList() {
		TaggedProfile* tp;
		vector<TaggedProfile*>::iterator i;
		if ((taggedProfileList != NULL) && (!taggedProfileList->empty())) {
			i = taggedProfileList->begin();
			while (i != taggedProfileList->end()) {
				tp = *i;
				delete (tp);
				++i;
			}
			taggedProfileList->clear();
		}
	}
}
}
}
}
}
