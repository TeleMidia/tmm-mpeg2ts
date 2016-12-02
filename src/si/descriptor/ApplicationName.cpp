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
 * ApplicationName.cpp
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/ApplicationName.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

ApplicationName::ApplicationName() : MpegDescriptor(0x01) {

}

ApplicationName::~ApplicationName() {
	vector<AppName*>::iterator it;

	it = appNameList.begin();
	while (it != appNameList.end()) {
		delete (*it);
		++it;
	}
}

int ApplicationName::process() {
	unsigned char cdlength = 0;
	int pos = MpegDescriptor::process();

	while (cdlength < descriptorLength) {
		AppName *aname = new AppName();
		aname->ISO639LanguageCode.assign(stream + pos, 3);
		pos+= 3;
		aname->appNameLength = stream[pos++] & 0xFF;
		aname->name.assign(stream + pos, aname->appNameLength);
		appNameList.push_back(aname);
		cdlength += 5;
	}

	return pos;
}

int ApplicationName::updateStream() {
	vector<AppName*>::iterator it;
	int pos = MpegDescriptor::updateStream();

	it = appNameList.begin();
	while (it != appNameList.end()) {
		memcpy(stream + pos, (*it)->ISO639LanguageCode.c_str(), 3);
		pos += 3;
		stream[pos++] = (*it)->appNameLength & 0xFF;
		for (int i = 0; i < (*it)->appNameLength; pos++, i++) {
			stream[pos] = (*it)->name[i];
		}
		++it;
	}

	return pos;
}

unsigned int ApplicationName::calculateDescriptorSize() {
	vector<AppName*>::iterator it;
	int pos = MpegDescriptor::calculateDescriptorSize();

	it = appNameList.begin();
	while (it != appNameList.end()) {
		pos += 4;
		pos += (*it)->appNameLength;
		++it;
	}
	return pos;
}

void ApplicationName::setAppName(string ISO639LanguageCode, string name) {
	AppName *aname = new AppName();
	aname->ISO639LanguageCode = ISO639LanguageCode;
	aname->appNameLength = name.size();
	aname->name = name;
	appNameList.push_back(aname);
}

vector<AppName*>* ApplicationName::getAppNameList() {
	return &appNameList;
}

}
}
}
}


