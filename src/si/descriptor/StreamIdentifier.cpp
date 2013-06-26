/*
 * StreamIdentifier.cpp
 *
 *  Created on: 25/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/StreamIdentifier.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

StreamIdentifier::StreamIdentifier() : MpegDescriptor(0x52) {
	componentTag = 0x00;
}

StreamIdentifier::~StreamIdentifier() {

}

int StreamIdentifier::process() {
	int pos = MpegDescriptor::process();

	componentTag = stream[pos++];

	return pos;
}

int StreamIdentifier::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = componentTag & 0xFF;

	return pos;
}

unsigned int StreamIdentifier::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	return pos + 1;
}

unsigned char StreamIdentifier::getComponentTag() {
	return componentTag;
}

void StreamIdentifier::setComponentTag(unsigned char ct) {
	componentTag = ct;
}

}
}
}
}


