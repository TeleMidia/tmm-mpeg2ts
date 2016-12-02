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
 * GingaApplication.cpp
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/GingaApplication.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

GingaApplication::GingaApplication(unsigned char descriptorTag) {
	this->descriptorTag = descriptorTag;
	//Ginga-J 	= 0x03
	//Ginga-NCL = 0x06
}

GingaApplication::~GingaApplication() {

}

int GingaApplication::process() {
	unsigned char len = 0;
	string value;
	int pos = MpegDescriptor::process();

	while (len < descriptorLength) {
		len += stream[pos++];
		value.assign(stream + pos, stream[pos-1]);
		pos += stream[pos-1];
		parameterList.push_back(value);
	}

	return pos;
}

int GingaApplication::updateStream() {
	vector<string>::iterator it;
	int pos = MpegDescriptor::updateStream();

	it = parameterList.begin();
	while (it != parameterList.end()) {
		stream[pos++] = (*it).size();
		memcpy(stream + pos, (*it).c_str(), (*it).size());
		pos += (*it).size();
		++it;
	}

	return pos;
}

unsigned int GingaApplication::calculateDescriptorSize() {
	vector<string>::iterator it;
	int pos = MpegDescriptor::calculateDescriptorSize();

	it = parameterList.begin();
	while (it != parameterList.end()) {
		pos += (*it).size() + 1;
		++it;
	}
	return pos;
}

void GingaApplication::addParameter(string param) {
	parameterList.push_back(param);
}

vector<string>* GingaApplication::getParameterList() {
	return &parameterList;
}

}
}
}
}


