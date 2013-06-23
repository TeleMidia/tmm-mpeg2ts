/*
 * NetworkName.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/NetworkName.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

NetworkName::NetworkName() : MpegDescriptor(0x40) {

}

NetworkName::~NetworkName() {

}

int NetworkName::process() {
	int pos = MpegDescriptor::process();

	networkName.assign(stream + pos, descriptorLength);
	pos += descriptorLength;

	return pos;
}

int NetworkName::updateStream() {
	int pos = MpegDescriptor::updateStream();

	memcpy(stream + pos, networkName.c_str(), networkName.length());
	pos += networkName.length();

	return pos;
}

unsigned int NetworkName::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += networkName.length();

	return pos;
}

void NetworkName::setNetworkName(string name) {
	networkName = name;
}

string NetworkName::getNetworkName() {
	return networkName;
}

}
}
}
}


