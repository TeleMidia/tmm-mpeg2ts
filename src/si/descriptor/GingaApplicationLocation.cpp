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
 * GingaApplicationLocation.cpp
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/GingaApplicationLocation.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

GingaApplicationLocation::GingaApplicationLocation(unsigned char descriptorTag) {
	this->descriptorTag = descriptorTag;
	//Ginga-J 	= 0x04
	//Ginga-NCL = 0x07
}

GingaApplicationLocation::~GingaApplicationLocation() {

}

int GingaApplicationLocation::process() {
	unsigned char len = 0;
	int pos = MpegDescriptor::process();

	pos++;
	len += stream[pos-1];
	baseDirectory.assign(stream + pos, stream[pos-1]);
	pos += stream[pos-1];

	pos++;
	len += stream[pos-1];
	classExtension.assign(stream + pos, stream[pos-1]);
	pos += stream[pos-1];

	len += 2;
	initialClass.assign(stream + pos, descriptorLength - len);
	pos += descriptorLength - len;

	return pos;
}

int GingaApplicationLocation::updateStream() {
	vector<string>::iterator it;
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = baseDirectory.size();
	memcpy(stream + pos, baseDirectory.c_str(), baseDirectory.size());
	pos += baseDirectory.size();

	stream[pos++] = classExtension.size();
	memcpy(stream + pos, classExtension.c_str(), classExtension.size());
	pos += classExtension.size();

	memcpy(stream + pos, initialClass.c_str(), initialClass.size());
	pos += initialClass.size();

	return pos;
}

unsigned int GingaApplicationLocation::calculateDescriptorSize() {
	vector<string>::iterator it;
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += (baseDirectory.size() + classExtension.size() +
			initialClass.size() + 2);

	return pos;
}

void GingaApplicationLocation::setBaseDirectory(string dir) {
	baseDirectory = dir;
}

string GingaApplicationLocation::getBaseDirectory() {
	return baseDirectory;
}

void GingaApplicationLocation::setClassExtension(string ext) {
	classExtension = ext;
}

string GingaApplicationLocation::getClassExtension() {
	return classExtension;
}

void GingaApplicationLocation::setInitialClass(string init) {
	initialClass = init;
}

string GingaApplicationLocation::getInitialClass() {
	return initialClass;
}

}
}
}
}


