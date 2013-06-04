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


