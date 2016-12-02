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
 * LocalTimeOffset.cpp
 *
 *  Created on: 03/05/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/LocalTimeOffset.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

LocalTimeOffset::LocalTimeOffset() : MpegDescriptor(0x58) {

}

LocalTimeOffset::~LocalTimeOffset() {
	vector<LocalTimeData*>::iterator it;
	it = localTimeDataList.begin();
	while (it != localTimeDataList.end()) {
		delete (*it);
		++it;
	}
}

int LocalTimeOffset::process() {
	unsigned int value;
	unsigned char length = 0;
	int pos = MpegDescriptor::process();

	while (length < descriptorLength) {
		LocalTimeData *ltd = new LocalTimeData();
		ltd->countryCode.assign(stream + pos, 3);
		pos+= 3;
		ltd->countryRegionId = (stream[pos] >> 2) & 0x3F;
		ltd->localTimeOffsetPolarity = stream[pos++] & 0x01;
		value = 0;
		value = ((stream[pos] & 0xFF) << 16) | ((stream[pos + 1] & 0xFF) << 8);
		pos += 2;
		ltd->localTimeOffset = Tot::BCDtoTime(value);
		value = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		ltd->timeOfChange = Tot::MJDtoDateTime(value);
		value = 0;
		value = ((stream[pos] & 0xFF) << 16) |
				((stream[pos + 1] & 0xFF) << 8) |
				(stream[pos + 2] & 0xFF);
		pos += 3;
		ltd->timeOfChange = ltd->timeOfChange + Tot::BCDtoTime(value);
		value = 0;
		value = ((stream[pos] & 0xFF) << 16) | ((stream[pos + 1] & 0xFF) << 8);
		pos += 2;
		ltd->nextTimeOffset = Tot::BCDtoTime(value);
		localTimeDataList.push_back(ltd);
		length += 13;
	}

	return pos;
}

int LocalTimeOffset::updateStream() {
	vector<LocalTimeData*>::iterator it;
	unsigned int value;
	int pos = MpegDescriptor::updateStream();

	it = localTimeDataList.begin();
	while (it != localTimeDataList.end()) {
		memcpy(stream + pos, (*it)->countryCode.c_str(), 3);
		pos += 3;
		stream[pos] = ((*it)->countryRegionId << 2) & 0xFC;
		stream[pos] = stream[pos] | 0x02;
		stream[pos] = stream[pos] | ((*it)->localTimeOffsetPolarity & 0x01);
		pos++;
		value = (unsigned int) Tot::timeToBCD((*it)->localTimeOffset, 0);
		stream[pos++] = (value >> 16) & 0xFF;
		stream[pos++] = (value >> 8) & 0xFF;
		value = (unsigned int) Tot::dateToMJD((*it)->timeOfChange);
		stream[pos++] = (value >> 8) & 0xFF;
		stream[pos++] = value & 0xFF;
		value = (unsigned int) Tot::timeToBCD((*it)->timeOfChange, 0);
		stream[pos++] = (value >> 16) & 0xFF;
		stream[pos++] = (value >> 8) & 0xFF;
		stream[pos++] = value & 0xFF;
		value = (unsigned int) Tot::timeToBCD((*it)->nextTimeOffset, 0);
		stream[pos++] = (value >> 16) & 0xFF;
		stream[pos++] = (value >> 8) & 0xFF;
		++it;
	}

	return pos;
}

unsigned int LocalTimeOffset::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += (13 * localTimeDataList.size());

	return pos;
}

void LocalTimeOffset::addLocalTimeData(LocalTimeData* ltd) {
	localTimeDataList.push_back(ltd);
}

vector<LocalTimeData*>* LocalTimeOffset::getLocalTimeDataList() {
	return &localTimeDataList;
}

}
}
}
}



