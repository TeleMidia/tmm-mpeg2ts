/*
 * PartialReception.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/PartialReception.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

PartialReception::PartialReception() : MpegDescriptor(0xFB) {

}

PartialReception::~PartialReception() {

}

int PartialReception::process() {
	unsigned short id;
	int pos = MpegDescriptor::process();

	serviceIdList.clear();

	while (pos <= descriptorLength) {
		id = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		serviceIdList.insert(id);
	}

	return pos;
}

int PartialReception::updateStream() {
	set<unsigned short>::iterator it;
	int pos = MpegDescriptor::updateStream();

	it = serviceIdList.begin();
	while(it != serviceIdList.end()) {
		stream[pos++] = ((*it) >> 8) & 0xFF;
		stream[pos++] = (*it) & 0xFF;
		++it;
	}

	return pos;
}

unsigned int PartialReception::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += serviceIdList.size() * 2;

	return pos;
}

void PartialReception::addServiceId(unsigned short id) {
	serviceIdList.insert(id);
}

set<unsigned short>* PartialReception::getServiceIdList() {
	return &serviceIdList;
}

}
}
}
}


