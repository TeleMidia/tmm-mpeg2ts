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



