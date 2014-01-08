/*
 * DataStreamAlignment.cpp
 *
 *  Created on: 08/01/2014
 *      Author: Felippe Nagato
 */

#include "si/descriptor/DataStreamAlignment.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

DataStreamAlignment::DataStreamAlignment() : MpegDescriptor(0x06) {
	alignmentType = 0x01;
}

DataStreamAlignment::~DataStreamAlignment() {

}

int DataStreamAlignment::process() {
	int pos = MpegDescriptor::process();

	alignmentType = stream[pos++] & 0xFF;

	return pos;
}

int DataStreamAlignment::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = alignmentType;

	return pos;
}

unsigned int DataStreamAlignment::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 1;

	return pos;
}

void DataStreamAlignment::setAlignmentType(unsigned char at) {
	alignmentType = at;
}

unsigned char DataStreamAlignment::getAlignmentType() {
	return alignmentType;
}

}
}
}
}


