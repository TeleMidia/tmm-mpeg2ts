/*
 * CarouselIdentifier.cpp
 *
 *  Created on: 03/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/CarouselIdentifier.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

CarouselIdentifier::CarouselIdentifier() : MpegDescriptor(0x13) {
	carouselId = 0;
	privateDataByte = NULL;
	privateDataByteLength = 0 ;
}

CarouselIdentifier::~CarouselIdentifier() {
	if (privateDataByte) delete privateDataByte;
}

int CarouselIdentifier::process() {
	int pos = MpegDescriptor::process();


	carouselId = ((stream[pos] & 0xFF) << 24) |
				 ((stream[pos + 1] & 0xFF) << 16) |
				 ((stream[pos + 2] & 0xFF) << 8) |
				 (stream[pos + 3] & 0xFF);

	pos += 4;

	for (int i = 0; i < privateDataByteLength; i++) {
		privateDataByte[i] = stream[pos + i];
	}

	pos += privateDataByteLength;

	return pos;
}

int CarouselIdentifier::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = (carouselId >> 24) & 0xFF;
	stream[pos++] = (carouselId >> 16) & 0xFF;
	stream[pos++] = (carouselId >> 8) & 0xFF;
	stream[pos++] = carouselId & 0xFF;

	for (int i = 0; i < privateDataByteLength; i++) {
		stream[pos + i] = privateDataByte[i];
	}

	pos += privateDataByteLength;

	return pos;
}

unsigned int CarouselIdentifier::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 4 + privateDataByteLength;

	return pos;
}

unsigned int CarouselIdentifier::getCarouselId() {
	return carouselId;
}

void CarouselIdentifier::setCarouselId(unsigned int id) {
	carouselId = id;
}

}
}
}
}


