/*
 * ParentalRating.cpp
 *
 *  Created on: 30/07/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/ParentalRating.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

ParentalRating::ParentalRating() : MpegDescriptor(0x55) {

}

ParentalRating::~ParentalRating() {
	vector<ParentalRatingInfo*>::iterator it;
	it = parentalRatingList.begin();
	while (it != parentalRatingList.end()) {
		if (*it) delete (*it);
		++it;
	}
}

int ParentalRating::process() {
	int descLen;
	ParentalRatingInfo* pri;
	int pos = MpegDescriptor::process();

	descLen = descriptorLength;
	while (descLen > 0) {
		pri = new ParentalRatingInfo();
		pri->countryCode.assign(stream + pos, 3);
		pos += 3;
		pri->rating = stream[pos++];
		parentalRatingList.push_back(pri);
		descLen -= 4;
	}

	return pos;
}

int ParentalRating::updateStream() {
	vector<ParentalRatingInfo*>::iterator it;
	int pos = MpegDescriptor::updateStream();

	it = parentalRatingList.begin();
	while (it != parentalRatingList.end()) {
		if ((*it)->countryCode.length() != 3) (*it)->countryCode.assign("BRA");
		memcpy(stream + pos, (*it)->countryCode.c_str(), (*it)->countryCode.length());
		pos += 3;
		stream[pos++] = (*it)->rating;
		++it;
	}

	return pos;
}

unsigned int ParentalRating::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += parentalRatingList.size() * 4;

	return pos;
}

vector<ParentalRatingInfo*>* ParentalRating::getParentalRatingList() {
	return &parentalRatingList;
}

void ParentalRating::addParentalRating(ParentalRatingInfo* pri) {
	if (pri) parentalRatingList.push_back(pri);
}

}
}
}
}


