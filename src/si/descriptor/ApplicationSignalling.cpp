/*
 * ApplicationSignalling.cpp
 *
 *  Created on: 01/02/2018
 *      Author: Felippe Nagato
 */

#include "si/descriptor/ApplicationSignalling.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

ApplicationSignalling::ApplicationSignalling() : MpegDescriptor(0x6F) {

}

ApplicationSignalling::~ApplicationSignalling() {

}

int ApplicationSignalling::process() {
	int pos = MpegDescriptor::process();

	applicationType = ((stream[pos] & 0xFF) << 8) |
	 	 	 	 	 (stream[pos + 1] & 0xFF);
	pos = pos + 2;
	AITVersionNumber = stream[pos++] & 0x1F;

	return pos;
}

int ApplicationSignalling::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = (applicationType >> 8) & 0xFF;
	stream[pos++] = applicationType & 0xFF;
	stream[pos] = 0xE0;
	stream[pos] = stream[pos] | (AITVersionNumber & 0x1F);
	pos++;

	return pos;
}

unsigned int ApplicationSignalling::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 3;

	return pos;
}

void ApplicationSignalling::setApplicationType(unsigned short type) {
	applicationType = type;
}

unsigned short ApplicationSignalling::getApplicationType() {
	return applicationType;
}

void ApplicationSignalling::setAITVersionNumber(unsigned char ver) {
	AITVersionNumber = ver;
}

unsigned char ApplicationSignalling::getAITVersionNumber() {
	return AITVersionNumber;
}


}
}
}
}
