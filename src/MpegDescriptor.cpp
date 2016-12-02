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

#include "MpegDescriptor.h"

#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

MpegDescriptor::MpegDescriptor() {
	stream = NULL;
	descriptorTag = 0;
	descriptorLength = 0;
	currentSize = 0;
}

MpegDescriptor::~MpegDescriptor() {
	if (stream != NULL) {
		delete (stream);
	}
}

MpegDescriptor::MpegDescriptor(unsigned char tag) {
	stream = NULL;
	descriptorTag = tag;
	descriptorLength = 0;
	currentSize = 0;
}

char MpegDescriptor::addData(char* data, unsigned short length) {
	unsigned short rbytes;

	if (currentSize == 0) {

		if (length < 2) {
			return -1; //error
		}

		descriptorTag = data[0] & 0xFF;
		descriptorLength = data[1] & 0xFF;

		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[descriptorLength + 2];
		} catch(...) {
			return -1;
		}
	}

	if (currentSize + length >= MAX_DESCRIPTOR_SIZE) {
		rbytes = MAX_DESCRIPTOR_SIZE - currentSize;
	} else {
		rbytes = length;
	}

	memcpy(stream + currentSize, data, rbytes);

	currentSize = currentSize + rbytes;

	if (isConsolidated()) {
		if (rbytes > 0) {
			process();
		}
		return 1;
	} else {
		return 0;
	}
}

int MpegDescriptor::process() {
	return 2;
}

int MpegDescriptor::updateStream() {
	unsigned int len;

	if (stream != NULL) {
		delete (stream);
	}
	try {
		len = calculateDescriptorSize();
		stream = new char[len];
	} catch(...) {
		return -1;
	}

	descriptorLength = len - 2;

	stream[0] = descriptorTag & 0xFF;
	stream[1] = descriptorLength & 0xFF;
	return 2;
}

unsigned int MpegDescriptor::calculateDescriptorSize() {
	return 2;
}

unsigned char MpegDescriptor::isConsolidated() {
	unsigned short len = descriptorLength + 2;
	return (len <= currentSize);
}

unsigned char MpegDescriptor::getDescriptorTag() {
	return descriptorTag;
}

unsigned char MpegDescriptor::getDescriptorLength() {
	return descriptorLength;
}

void MpegDescriptor::setDescriptorTag(unsigned char tag) {
	descriptorTag = tag;
}

int MpegDescriptor::getStreamSize() {
	return calculateDescriptorSize();
}

int MpegDescriptor::getStream(char** dataStream) {
	int slen = updateStream();
	if (slen >= 0) {
		*dataStream = stream;
		return slen;
	} else {
		return -1;
	}
}

MpegDescriptor* MpegDescriptor::getDescriptor(
		vector<MpegDescriptor*>* descriptors, unsigned char Tag) {
	vector<MpegDescriptor*>::iterator dit;
	dit = descriptors->begin();
	while (dit != descriptors->end()) {
		if ((*dit)->getDescriptorTag() == Tag) {
			return *dit;
		}
		dit++;
	}
	return NULL;
}

vector<MpegDescriptor*>* MpegDescriptor::getDescriptors(
		vector<MpegDescriptor*>* descriptors, unsigned char Tag) {
	vector<MpegDescriptor*>* result;
	vector<MpegDescriptor*>::iterator dit;
	result = new vector<MpegDescriptor*>;
	dit = descriptors->begin();
	while (dit != descriptors->end()) {
		if ((*dit)->getDescriptorTag() == Tag) {
			result->push_back(*dit);
		}
		dit++;
	}
	if (result->size() == 0) {
		delete result;
		result = NULL;
	}
	return result;
}

int MpegDescriptor::getDescriptorsLength(vector<MpegDescriptor*>* descriptors) {
	int len = 0;
	vector<MpegDescriptor*>::iterator dit;
	dit = descriptors->begin();
	while (dit != descriptors->end()) {
		len = len + (*dit)->getDescriptorLength() + 2;
		dit++;
	}
	return len;
}
}
}
}
}
