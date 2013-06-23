/*
 * SystemManagement.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/SystemManagement.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

SystemManagement::SystemManagement() : MpegDescriptor(0xFE) {
	broadcastingFlag = SM_BROADCASTING;
	broadcastingIdentifier = ST_ISDB_SYSTEM;
	additionalBroadcastingId = 1; // What's it?
	additionalIdentificationInfoLength = 0;
	additionalIdentificationInfo = NULL;
}

SystemManagement::~SystemManagement() {
	if (additionalIdentificationInfo) delete additionalIdentificationInfo;
}

int SystemManagement::process() {
	char streamLen;
	int pos = MpegDescriptor::process();

	broadcastingFlag = ((stream[pos] >> 6) & 0xFF);
	broadcastingIdentifier = (stream[pos] & 0x3F);
	pos++;
	additionalBroadcastingId = stream[pos] & 0xFF;
	pos++;

	if (additionalIdentificationInfo) delete additionalIdentificationInfo;
	additionalIdentificationInfo = NULL;

	streamLen = descriptorLength - 2;
	if (streamLen > 0) {
		additionalIdentificationInfo = new char[streamLen];
		memcpy(additionalIdentificationInfo, stream + pos, streamLen);
		pos += streamLen;
	}

	return pos;
}

int SystemManagement::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos] = (broadcastingFlag & 0xC0) << 6;
	stream[pos] = stream[pos] | (broadcastingIdentifier & 0x3F);
	pos++;
	stream[pos] = additionalBroadcastingId;
	pos++;

	if (additionalIdentificationInfo) {
		memcpy(stream + pos, additionalIdentificationInfo, additionalIdentificationInfoLength);
		pos += additionalIdentificationInfoLength;
	}

	return pos;
}

unsigned int SystemManagement::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 2 + additionalIdentificationInfoLength;

	return pos;
}

void SystemManagement::setBroadcastingFlag(unsigned char bf) {
	broadcastingFlag = bf;
}

unsigned char SystemManagement::getBroadcastingFlag() {
	return broadcastingFlag;
}

void SystemManagement::setBroadcastingIdentifier(unsigned char bi) {
	broadcastingIdentifier = bi;
}

unsigned char SystemManagement::getBroadcastingIdentifier() {
	return broadcastingIdentifier;
}

void SystemManagement::setAdditionalBroadcastingId(unsigned char abi) {
	additionalBroadcastingId = abi;
}

unsigned char SystemManagement::getAdditionalBroadcastingId() {
	return additionalBroadcastingId;
}

void SystemManagement::setAdditionalIdentificationInfo(char* stream, unsigned char len) {
	additionalIdentificationInfoLength = len;
	if (additionalIdentificationInfo) delete additionalIdentificationInfo;
	if (len == 0) {
		additionalIdentificationInfo = NULL;
		return;
	}
	additionalIdentificationInfo = new char[len];
	memcpy(stream, additionalIdentificationInfo, len);
}

unsigned char SystemManagement::getAdditionalIdentificationInfo(char** stream) {
	*stream = additionalIdentificationInfo;
	return additionalIdentificationInfoLength;
}

}
}
}
}


