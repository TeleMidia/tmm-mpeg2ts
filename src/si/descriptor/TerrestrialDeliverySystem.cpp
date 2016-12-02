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
 * TerrestrialDeliverySystem.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/TerrestrialDeliverySystem.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

TerrestrialDeliverySystem::TerrestrialDeliverySystem() : MpegDescriptor(0xFA) {
	//Divisão Regional do Brasil em Mesorregiões e Microrregiões Geográficas page 90
	areaCode = 0x992; // 19-18 (RJ-Rio de Janeiro) ABNT NBR 15603-2:2007 page 122
	guardInterval = GUARD_INTERVAL_1_16;
	transmissionMode = TRANSMISSION_MODE_3;
}

TerrestrialDeliverySystem::~TerrestrialDeliverySystem() {

}

int TerrestrialDeliverySystem::process() {
	unsigned short value;
	char streamLen;
	int pos = MpegDescriptor::process();

	frequencyList.clear();

	areaCode = ((stream[pos] << 4) & 0x0FF0);
	pos++;
	areaCode = areaCode | ((stream[pos] >> 4) & 0x0F);
	guardInterval = ((stream[pos] & 0x0C) >> 2);
	transmissionMode = (stream[pos] & 0x02);
	pos++;

	streamLen = descriptorLength - 2;
	while (streamLen) {
		value = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		frequencyList.insert(value);
		pos += 2;
		streamLen -= 2;
	}

	return pos;
}

int TerrestrialDeliverySystem::updateStream() {
	set<unsigned short>::iterator it;
	int pos = MpegDescriptor::updateStream();

	stream[pos] = ((areaCode >> 4) & 0xFF);
	pos++;
	stream[pos] = ((areaCode << 4) & 0xF0);
	stream[pos] = stream[pos] | ((guardInterval << 2) & 0x0C);
	stream[pos] = stream[pos] | (transmissionMode & 0x02);

	pos++;

	it = frequencyList.begin();
	while(it != frequencyList.end()) {
		stream[pos++] = ((*it) >> 8) & 0xFF;
		stream[pos++] = ((*it) & 0xFF);
		++it;
	}

	return pos;
}

unsigned int TerrestrialDeliverySystem::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 2 + (frequencyList.size() * 2);

	return pos;
}

void TerrestrialDeliverySystem::setAreaCode(unsigned short ac) {
	areaCode = ac;
}

unsigned short TerrestrialDeliverySystem::getAreaCode() {
	return areaCode;
}

void TerrestrialDeliverySystem::setGuardInterval(unsigned char gi) {
	guardInterval = gi;
}

unsigned char TerrestrialDeliverySystem::getGuardInterval() {
	return guardInterval;
}

void TerrestrialDeliverySystem::setTransmissionMode(unsigned char tm) {
	transmissionMode = tm;
}

unsigned char TerrestrialDeliverySystem::getTransmissionMode() {
	return transmissionMode;
}

void TerrestrialDeliverySystem::addFrequency(unsigned short freq) {
	frequencyList.insert(freq);
}

set<unsigned short>* TerrestrialDeliverySystem::getFrequencyList() {
	return &frequencyList;
}


}
}
}
}


