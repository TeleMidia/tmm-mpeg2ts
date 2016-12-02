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
 * Application.cpp
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/Application.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Application::Application() : MpegDescriptor(0x00) {
	transportProtocolLabel = NULL;
	transportProtocolLabelLength = 0;
}

Application::~Application() {
	vector<AppProfile*>::iterator it;

	it = appProfileList.begin();
	while (it != appProfileList.end()) {
		delete (*it);
		++it;
	}

	if (transportProtocolLabel) delete transportProtocolLabel;
}

int Application::process() {
	unsigned char cdlength = 0;
	int pos = MpegDescriptor::process();

	applicationProfilesLength = stream[pos++] & 0xFF;

	while (cdlength < applicationProfilesLength) {
		AppProfile *appProfile = new AppProfile();
		appProfile->applicationProfile = (((stream[pos] & 0xFF) << 8) |
										   (stream[pos+1] & 0xFF));
		appProfile->versionMajor = stream[pos++] & 0xFF;
		appProfile->versionMicro = stream[pos++] & 0xFF;
		appProfileList.push_back(appProfile);
		cdlength += 5;
	}

	serviceBoundFlag = (stream[pos] & 0x80) > 0;
	visibility = (stream[pos++] & 0x60) >> 5;
	applicationPriority = (stream[pos++] & 0xFF);

	cdlength = descriptorLength + 2;
	if (transportProtocolLabel) delete transportProtocolLabel;
	transportProtocolLabelLength = cdlength - pos;
	transportProtocolLabel = new char[transportProtocolLabelLength];

	for (int i = 0; pos < cdlength; pos++, i++) {
		transportProtocolLabel[i] = stream[pos];
	}

	return pos;
}

int Application::updateStream() {
	vector<AppProfile*>::iterator it;
	int pos = MpegDescriptor::updateStream();

	applicationProfilesLength = appProfileList.size() * 5;
	stream[pos++] = applicationProfilesLength & 0xFF;

	it = appProfileList.begin();
	while (it != appProfileList.end()) {
		stream[pos++] = ((*it)->applicationProfile >> 8) & 0xFF;
		stream[pos++] = (*it)->applicationProfile & 0xFF;
		stream[pos++] = (*it)->versionMajor & 0xFF;
		stream[pos++] = (*it)->versionMinor & 0xFF;
		stream[pos++] = (*it)->versionMicro & 0xFF;
		++it;
	}

	stream[pos] = 0x1F;
	if (serviceBoundFlag) stream[pos] = stream[pos] | 0x80;
	stream[pos] = stream[pos] | ((visibility << 5) & 0x60);
	pos++;
	stream[pos++] = applicationPriority & 0xFF;

	for (int i = 0; i < transportProtocolLabelLength; pos++, i++) {
		stream[pos] = transportProtocolLabel[i];
	}

	return pos;
}

unsigned int Application::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 3;
	pos += appProfileList.size() * 5;
	pos += transportProtocolLabelLength;

	return pos;
}

void Application::setApplicationProfilesLength(unsigned char len) {
	applicationProfilesLength = len;
}

unsigned char Application::getApplicationProfilesLength() {
	return applicationProfilesLength;
}

void Application::setServiceBoundFlag(bool flag) {
	serviceBoundFlag = flag;
}

bool Application::getServiceBoundFlag() {
	return serviceBoundFlag;
}
void Application::setVisibility(unsigned char v) {
	visibility = v;
}

unsigned char Application::getVisibility() {
	return visibility;
}

void Application::setApplicationPriority(unsigned char priority) {
	applicationPriority = priority;
}

unsigned char Application::getApplicationPriority() {
	return applicationPriority;
}

void Application::setTransportProtocolLabel(char* label, unsigned char len) {
	if (transportProtocolLabel) delete transportProtocolLabel;
	transportProtocolLabel = new char[len];
	memcpy(transportProtocolLabel, label, len);
	transportProtocolLabelLength = len;
}

unsigned char Application::getTransportProtocolLabel(char** protocol) {
	*protocol = transportProtocolLabel;
	return transportProtocolLabelLength;
}

vector<AppProfile*>* Application::getAppProfileList() {
	return &appProfileList;
}

void Application::addAppProfile(unsigned short applicationProfile,
								unsigned char versionMajor,
								unsigned char versionMinor,
								unsigned char versionMicro) {
	AppProfile *appProfile = new AppProfile();
	appProfile->applicationProfile = applicationProfile;
	appProfile->versionMajor = versionMajor;
	appProfile->versionMicro = versionMinor;
	appProfileList.push_back(appProfile);
}

}
}
}
}


