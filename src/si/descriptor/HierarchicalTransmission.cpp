/*
 * HierarchicalTransmission.cpp
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#include "si/descriptor/HierarchicalTransmission.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

HierarchicalTransmission::HierarchicalTransmission() : MpegDescriptor(0xC0) {
	qualityLevel = 0x01;
}

HierarchicalTransmission::~HierarchicalTransmission() {

}

int HierarchicalTransmission::process() {
	int pos = MpegDescriptor::process();

	qualityLevel = stream[pos++] & 0x01;
	referencePID = ((stream[pos] & 0x1F) << 8) |
 	 	 	 	 	 (stream[pos + 1] & 0xFF);

	pos += 2;

	return pos;
}

int HierarchicalTransmission::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = qualityLevel & 0x01;

	stream[pos++] = (referencePID >> 8) & 0x1F;
	stream[pos++] = referencePID & 0xFF;

	return pos;
}

unsigned int HierarchicalTransmission::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 3;

	return pos;
}

void HierarchicalTransmission::setQualityLevel(unsigned char ql) {
	qualityLevel = ql;
}

unsigned char HierarchicalTransmission::getQualityLevel() {
	return qualityLevel & 0x01;
}

void HierarchicalTransmission::setReferencePID(unsigned short pid) {
	referencePID = pid;
}

unsigned short HierarchicalTransmission::getReferencePID() {
	return referencePID;
}


}
}
}
}


