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
 * ServiceList.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/ServiceList.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

ServiceList::ServiceList() : MpegDescriptor(0x41) {

}

ServiceList::~ServiceList() {

}

int ServiceList::process() {
	unsigned short id;
	unsigned char type;
	int pos = MpegDescriptor::process();

	serviceList.clear();

	while (pos < descriptorLength) {
		id = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		type = stream[pos++];
		serviceList[id] = type;
	}

	return pos;
}

int ServiceList::updateStream() {
	map<unsigned short, unsigned char>::iterator it;
	int pos = MpegDescriptor::updateStream();

	it = serviceList.begin();
	while(it != serviceList.end()) {
		stream[pos++] = (it->first >> 8) & 0xFF;
		stream[pos++] = it->first & 0xFF;
		stream[pos++] = it->second & 0xFF;
		++it;
	}

	return pos;
}

unsigned int ServiceList::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += serviceList.size() * 3;

	return pos;
}

void ServiceList::addService(unsigned short id, unsigned char type) {
	serviceList[id] = type;
}

map<unsigned short, unsigned char>* ServiceList::getServiceList() {
	return &serviceList;
}

}
}
}
}


