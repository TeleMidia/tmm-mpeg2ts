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
 * Service.cpp
 *
 *  Created on: 18/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/Service.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Service::Service() : MpegDescriptor(0x48) {
	serviceType = DIGITAL_TELEVISION_SERVICE;
}

Service::~Service() {

}

int Service::process() {
	int strLen;
	int pos = MpegDescriptor::process();

	serviceType = stream[pos++];
	strLen = stream[pos++];
	providerName.assign(stream + pos, strLen);
	pos += strLen;
	strLen = stream[pos++];
	serviceName.assign(stream + pos, strLen);
	pos += strLen;

	return pos;
}

int Service::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = serviceType;
	stream[pos++] = providerName.length();
	memcpy(stream + pos, providerName.c_str(), providerName.length());
	pos += providerName.length();
	stream[pos++] = serviceName.length();
	memcpy(stream + pos, serviceName.c_str(), serviceName.length());
	pos += serviceName.length();

	return pos;
}

unsigned int Service::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 3 + providerName.length() + serviceName.length();

	return pos;
}

void Service::setServiceType(unsigned char type) {
	serviceType = type;
}

unsigned char Service::getServiceType() {
	return serviceType;
}

void Service::setProviderName(string name) {
	providerName = name;
}

string Service::getProviderName() {
	return providerName;
}

void Service::setServiceName(string name) {
	serviceName = name;
}

string Service::getServiceName() {
	return serviceName;
}

}
}
}
}


