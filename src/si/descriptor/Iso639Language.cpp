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


