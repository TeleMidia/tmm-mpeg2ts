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


