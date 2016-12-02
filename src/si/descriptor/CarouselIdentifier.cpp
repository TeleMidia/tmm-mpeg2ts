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


