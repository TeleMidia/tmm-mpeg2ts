/*
 * DeferredAssociationTags.cpp
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#include "si/descriptor/DeferredAssociationTags.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

DeferredAssociationTags::DeferredAssociationTags() : MpegDescriptor(0x15) {
	privateDataByte = NULL;
	privateDataByteLength = 0;
}

DeferredAssociationTags::~DeferredAssociationTags() {
	if (privateDataByte) delete privateDataByte;
}

int DeferredAssociationTags::process() {
	int len;
	unsigned short value;
	int pos = MpegDescriptor::process();

	len = stream[pos++] & 0xFF;

	for (int i = 0; i < len; i++) {
		value = stream[pos++] & 0xFF;
		associationTagList.push_back(value);
	}

	transportStreamId = ((stream[pos] & 0xFF) << 8) |
			 	 	 	 (stream[pos + 1] & 0xFF);

	pos += 2;

	programNumber = ((stream[pos] & 0xFF) << 8) |
				 	 (stream[pos + 1] & 0xFF);

	pos += 2;

	originalNetworkId = ((stream[pos] & 0xFF) << 8) |
						(stream[pos + 1] & 0xFF);

	pos += 2;

	privateDataByteLength = descriptorLength - ((len * 2) + 7);
	if (privateDataByte) delete privateDataByte;
	privateDataByte = new char[privateDataByteLength];
	for (int i = 0; i < privateDataByteLength; i++) {
		privateDataByte[i] = stream[pos + i];
	}

	pos += privateDataByteLength;

	return pos;
}

int DeferredAssociationTags::updateStream() {
	int len;
	unsigned short value;
	int pos = MpegDescriptor::updateStream();

	len = associationTagList.size();
	stream[pos++] = len & 0xFF;

	for (int i = 0; i < len; i++) {
		value = associationTagList[i];
		stream[pos++] = (value >> 8) & 0xFF;
		stream[pos++] = value & 0xFF;
	}

	stream[pos++] = (transportStreamId >> 8) & 0xFF;
	stream[pos++] = transportStreamId & 0xFF;

	stream[pos++] = (programNumber >> 8) & 0xFF;
	stream[pos++] = programNumber & 0xFF;

	stream[pos++] = (originalNetworkId >> 8) & 0xFF;
	stream[pos++] = originalNetworkId & 0xFF;

	for (int i = 0; i < privateDataByteLength; i++) {
		stream[pos + i] = privateDataByte[i];
	}

	pos += privateDataByteLength;

	return pos;
}

unsigned int DeferredAssociationTags::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += ((associationTagList.size() * 2) + 7);

	return pos;
}

void DeferredAssociationTags::addAssociationTag(unsigned short at) {
	associationTagList.push_back(at);
}

void DeferredAssociationTags::setTransportStreamId(unsigned short id) {
	transportStreamId = id;
}

unsigned short DeferredAssociationTags::getTransportStreamId() {
	return transportStreamId;
}

void DeferredAssociationTags::setProgramNumber(unsigned short pn) {
	programNumber = pn;
}

unsigned short DeferredAssociationTags::getProgramNumber() {
	return programNumber;
}

void  DeferredAssociationTags::setOriginalNetworkId(unsigned short id) {
	originalNetworkId = id;
}

unsigned short DeferredAssociationTags::getOriginalNetworkId() {
	return originalNetworkId;
}

void DeferredAssociationTags::setPrivateDataByte(char* stream, int length) {
	if (stream) {
		if (privateDataByte) delete privateDataByte;
		privateDataByte = new char[length];
		memcpy(privateDataByte, stream, length);
		privateDataByteLength = length;
	}
}

int DeferredAssociationTags::getPrivateDataByte(char** stream) {
	if (privateDataByte) {
		*stream = privateDataByte;
		return privateDataByteLength;
	}
	return 0;
}

}
}
}
}


