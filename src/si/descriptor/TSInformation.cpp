/*
 * TSInformation.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/TSInformation.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

TSInformation::TSInformation() : MpegDescriptor(0xCD) {
	remoteControlKeyId = 1;
	tsName = "Unnamed TS";
}

TSInformation::~TSInformation() {

}

int TSInformation::process() {
	unsigned char tsLen, typeCount;
	unsigned short sid;
	int pos = MpegDescriptor::process();

	transmissionTypeList.clear();

	remoteControlKeyId = stream[pos++];
	tsLen = ((stream[pos] >> 2) & 0x3F);
	typeCount = (stream[pos] & 0x02);
	pos++;
	tsName.assign(stream + pos, tsLen);
	pos += tsLen;

	for (unsigned int i = 0; i < typeCount; i++) {
		TransmissionType* tt = new TransmissionType();
		tt->transmissionTypeInfo = stream[pos++] & 0xFF;
		tt->numOfService = stream[pos++] & 0xFF;
		for (unsigned int j = 0; j < tt->numOfService; j++) {
			sid = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
			pos += 2;
			tt->serviceIdList.insert(sid);
		}
	}

	return pos;
}

int TSInformation::updateStream() {
	set<TransmissionType*>::iterator it;
	set<unsigned short>::iterator itSvc;
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = remoteControlKeyId;
	stream[pos] = (tsName.size() << 2) & 0xFC;
	stream[pos] = stream[pos] | (transmissionTypeList.size() & 0x02);
	pos++;
	memcpy(stream + pos, tsName.c_str(), tsName.size());
	pos += tsName.size();

	it = transmissionTypeList.begin();
	while(it != transmissionTypeList.end()) {
		stream[pos++] = ((*it)->transmissionTypeInfo & 0xFF);
		(*it)->numOfService = transmissionTypeList.size();
		stream[pos++] = ((*it)->numOfService & 0xFF);
		itSvc = (*it)->serviceIdList.begin();
		while (itSvc != (*it)->serviceIdList.end()) {
			stream[pos++] = (((*itSvc) << 8) & 0xFF);
			stream[pos++] = ((*itSvc) & 0xFF);
			++itSvc;
		}
		++it;
	}

	return pos;
}

unsigned int TSInformation::calculateDescriptorSize() {
	set<TransmissionType*>::iterator it;
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 2 + tsName.size();

	it = transmissionTypeList.begin();
	while(it != transmissionTypeList.end()) {
		(*it)->numOfService = transmissionTypeList.size();
		pos += 2 + ((*it)->serviceIdList.size() * 2);
		++it;
	}

	return pos;
}

void TSInformation::setRemoteControlKeyId(unsigned char key) {
	remoteControlKeyId = key;
}

unsigned char TSInformation::getRemoteControlKeyId() {
	return remoteControlKeyId;
}

unsigned char TSInformation::getTransmissionTypeCount() {
	return transmissionTypeList.size();
}

void TSInformation::setTsName(string name) {
	tsName = name;
}

string TSInformation::getTsName() {
	return tsName;
}

void TSInformation::addTransmissionTypeList(TransmissionType* tt) {
	transmissionTypeList.insert(tt);
}

set<TransmissionType*>* TSInformation::getTransmissionTypeList() {
	return &transmissionTypeList;
}

}
}
}
}


