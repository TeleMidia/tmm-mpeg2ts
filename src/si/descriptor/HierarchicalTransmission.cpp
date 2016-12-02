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


