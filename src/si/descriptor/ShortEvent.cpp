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
 * ShortEvent.cpp
 *
 *  Created on: 30/07/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/ShortEvent.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

ShortEvent::ShortEvent() : MpegDescriptor(0x4D) {
	iso639LanguageCode.assign("por");
}

ShortEvent::~ShortEvent() {

}

int ShortEvent::process() {
	int strLen;
	int pos = MpegDescriptor::process();

	iso639LanguageCode.assign(stream + pos, 3);
	pos += 3;
	strLen = stream[pos++];
	eventName.assign(stream + pos, strLen);
	pos += strLen;
	strLen = stream[pos++];
	text.assign(stream + pos, strLen);
	pos += strLen;

	return pos;
}

int ShortEvent::updateStream() {
	int pos = MpegDescriptor::updateStream();

	memcpy(stream + pos, iso639LanguageCode.c_str(), iso639LanguageCode.length());
	pos += 3;
	stream[pos++] = eventName.length();
	memcpy(stream + pos, eventName.c_str(), eventName.length());
	pos += eventName.length();
	stream[pos++] = text.length();
	memcpy(stream + pos, text.c_str(), text.length());
	pos += text.length();

	return pos;
}

unsigned int ShortEvent::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 5 + eventName.length() + text.length();

	return pos;
}

void ShortEvent::setIso639LanguageCode(string code) {
	if (code.length() != 3) iso639LanguageCode.assign("por");
	iso639LanguageCode = code;
}

string ShortEvent::getIso639LanguageCode() {
	return iso639LanguageCode;
}

unsigned char ShortEvent::getEventNameLength() {
	return eventName.length();
}

void ShortEvent::setEventName(string name) {
	eventName = name;
}

string ShortEvent::getEventName() {
	return eventName;
}

unsigned char ShortEvent::getTextLength() {
	return text.length();
}

void ShortEvent::setText(string text) {
	this->text = text;
}

string ShortEvent::getText() {
	return text;
}

}
}
}
}


