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
 * Iso639Language.cpp
 *
 *  Created on: 24/10/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/Iso639Language.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Iso639Language::Iso639Language() : MpegDescriptor(0x0A) {

}

Iso639Language::~Iso639Language() {
	releaseIso639LanguageList();
}

void Iso639Language::releaseIso639LanguageList() {
	vector<Iso639Lang*>::iterator it;
	it = iso639LanguageList.begin();
	while (it != iso639LanguageList.end()) {
		if (*it) delete *it;
		++it;
	}
}

int Iso639Language::process() {
	int pos = MpegDescriptor::process();
	unsigned char rlen = descriptorLength;

	while (rlen > 0) {
		rlen -= 4;
		Iso639Lang* lang = new Iso639Lang();
		lang->iso639LanguageCode.assign(stream + pos, 3);
		pos += 3;
		lang->audioType = stream[pos++] & 0xFF;
		iso639LanguageList.push_back(lang);
	}

	return pos;
}

int Iso639Language::updateStream() {
	int pos = MpegDescriptor::updateStream();
	vector<Iso639Lang*>::iterator it;

	it = iso639LanguageList.begin();
	while (it != iso639LanguageList.end()) {
		if (*it) {
			memcpy(stream + pos, (*it)->iso639LanguageCode.c_str(),
					(*it)->iso639LanguageCode.length());
			pos += 3;
			stream[pos++] = (*it)->audioType & 0xFF;
		}
		++it;
	}

	return pos;
}

unsigned int Iso639Language::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += iso639LanguageList.size() * 4;

	return pos;
}

bool Iso639Language::addIso639Language(string language, unsigned char audioType) {
	if (language.size() == 3) {
		Iso639Lang* lang = new Iso639Lang();
		lang->iso639LanguageCode.assign(language);
		lang->audioType = audioType;
		iso639LanguageList.push_back(lang);
		return true;
	}
	return false;
}

}
}
}
}


