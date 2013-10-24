/*
 * Aac.cpp
 *
 *  Created on: 23/10/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/Aac.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Aac::Aac() : MpegDescriptor(0x7C) {
	profileAndLevel = 0x40;
	aacTypeFlag = false;
	aacType = 0;
	aditionalInfo = NULL;
	aditionalInfoLength = 0;
}

Aac::~Aac() {
	if (aditionalInfo) delete aditionalInfo;
}

int Aac::process() {
	int pos = MpegDescriptor::process();

	profileAndLevel = stream[pos++] & 0xFF;
	aacTypeFlag = (stream[pos++] & 0x80) >> 7;
	if (aacTypeFlag) {
		aacType = stream[pos++] & 0xFF;
		aditionalInfoLength = descriptorLength - 3;
		if (aditionalInfo) delete aditionalInfo;
		aditionalInfo = new char[aditionalInfoLength];
		memcpy(aditionalInfo, stream + pos, aditionalInfoLength);
		pos += aditionalInfoLength;
	}

	return pos;
}

int Aac::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = profileAndLevel;
	stream[pos++] = aacTypeFlag << 7;
	if (aacTypeFlag) {
		stream[pos++] = aacType;
		if (aditionalInfo) {
			memcpy(stream + pos, aditionalInfo, aditionalInfoLength);
			pos += aditionalInfoLength;
		}
	}

	return pos;
}

unsigned int Aac::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 2;

	if (aacTypeFlag) {
		pos += 1 + aditionalInfoLength;
	}

	return pos;
}

void Aac::setProfileAndLevel(unsigned char pl) {
	profileAndLevel = pl;
}

unsigned char Aac::getProfileAndLevel() {
	return profileAndLevel;
}

void Aac::setAacTypeFlag(bool flag) {
	aacTypeFlag = flag;
}

bool Aac::getAacTypeFlag() {
	return aacTypeFlag;
}

void Aac::setAacType(unsigned char type) {
	aacType = type;
}

unsigned char Aac::getAacType() {
	return aacType;
}

void Aac::setAditionalInfo(char* dataStream, unsigned char length) {
	if (dataStream && length) {
		if (aditionalInfo) delete aditionalInfo;
		aditionalInfo = new char[length];
		memcpy(aditionalInfo, dataStream, length);
		aditionalInfoLength = length;
	}
}

unsigned char Aac::getAditionalInfo(char** dataStream) {
	if (aditionalInfo && aditionalInfoLength) {
		*dataStream = aditionalInfo;
	}
	return aditionalInfoLength;
}

}
}
}
}


