/*
 * Sdt.cpp
 *
 *  Created on: 18/06/2013
 *      Author: Felippe Nagato
 */

#include "si/Sdt.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Sdt::Sdt() : PrivateSection() {
	tableId = 0x42;
}

Sdt::~Sdt() {
	releaseAllServiceInformation();
}

void Sdt::releaseAllServiceInformation() {
	vector<ServiceInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	it = serviceList.begin();
	while (it != serviceList.end()) {
		if (*it) {
			itDesc = (*it)->descriptorList.begin();
			while (itDesc != (*it)->descriptorList.end()) {
				if (*itDesc) {
					delete (*itDesc);
				}
				++itDesc;
			}
			delete (*it);
		}
		++it;
	}

	serviceList.clear();
}

int Sdt::processSectionPayload() {
	unsigned short length;
	MpegDescriptor *d;
	unsigned char descriptorTag;
	unsigned short descriptorSize;
	int pos = PrivateSection::processSectionPayload();

	if (tableId != 0x42) {
		return -1;
	}

	originalNetworkId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
	pos += 3;

	while (pos + 4 < privateSectionLength + 3) {
		ServiceInformation *si = new ServiceInformation();
		si->serviceId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		si->eitScheduleFlag = (stream[pos] & 0x02) >> 1;
		si->eitPresentFollowingFlag = stream[pos] & 0x01;
		pos++;
		si->runningStatus = (stream[pos] & 0xE0) >> 5;
		si->freeCaMode = (stream[pos] & 0x10) >> 4;
		si->descriptorLoopLength = ((stream[pos] & 0x0F) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		length = 0;
		while (length < si->descriptorLoopLength) {
			descriptorTag = stream[pos] & 0xFF;
			descriptorSize = (stream[pos + 1] & 0xFF) + 2;
			length += descriptorSize;
			switch (descriptorTag) {
			case 0x48: // Service descriptor
				d = new Service();
				d->addData(stream + pos, descriptorSize);
				si->descriptorList.push_back(d);
				break;
			default:
				cout << "Sdt::processSectionPayload - Descriptor unrecognized. " <<
						(descriptorTag && 0xFF) << endl;
				break;
			}
			pos = pos + descriptorSize;
		}
		serviceList.push_back(si);
	}

	crc32 = ((stream[pos] & 0xFF) << 24) |
			((stream[pos + 1] & 0xFF) << 16) |
			((stream[pos + 2] & 0xFF) << 8) |
			(stream[pos + 3] & 0xFF);

	pos += 4;

	return pos;
}

int Sdt::updateStream() {
	vector<ServiceInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	char* dataStream;
	int pos = PrivateSection::updateStream();

	stream[pos++] = (originalNetworkId >> 8) & 0xFF;
	stream[pos++] = originalNetworkId & 0xFF;

	stream[pos++] = 0xFF;

	it = serviceList.begin();
	while (it != serviceList.end()) {
		(*it)->descriptorLoopLength = 0;
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			(*it)->descriptorLoopLength += (*itDesc)->getStreamSize();
			itDesc++;
		}
		stream[pos++] = ((*it)->serviceId >> 8) & 0xFF;
		stream[pos++] = (*it)->serviceId & 0xFF;
		stream[pos] = 0xFC;
		stream[pos] = stream[pos] | (((*it)->eitScheduleFlag & 0x01) << 1);
		stream[pos] = stream[pos] | ((*it)->eitPresentFollowingFlag & 0x01);
		pos++;
		stream[pos] = 0;
		stream[pos] = stream[pos] | (((*it)->runningStatus & 0x03) << 5);
		stream[pos] = stream[pos] | (((*it)->freeCaMode & 0x01) << 4);
		stream[pos] = stream[pos] | (((*it)->descriptorLoopLength >> 8) & 0x0F);
		pos++;
		stream[pos++] = ((*it)->descriptorLoopLength & 0xFF);
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			streamLen = (*itDesc)->getStream(&dataStream);
			if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
				memcpy(stream + pos, dataStream, streamLen);
				pos += streamLen;
			} else {
				cout << "Sdt::updateStream - Descriptor too long." << endl;
				break;
			}
			itDesc++;
		}
		++it;
	}

	Crc32 crc;
	crc32 = crc.crc(stream, pos);
	stream[pos++] = (crc32 >> 24) & 0xFF;
	stream[pos++] = (crc32 >> 16) & 0xFF;
	stream[pos++] = (crc32 >> 8) & 0xFF;
	stream[pos++] = crc32 & 0xFF;

	return pos;
}

int Sdt::calculateSectionSize() {
	vector<ServiceInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	unsigned int pos = PrivateSection::calculateSectionSize();

	pos = pos + 3;

	it = serviceList.begin();
	while (it != serviceList.end()) {
		pos += 5;
		(*it)->descriptorLoopLength = 0;
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			(*it)->descriptorLoopLength += (*itDesc)->getStreamSize();
			itDesc++;
		}
		pos += (*it)->descriptorLoopLength;
		++it;
	}

	return pos + 4;
}

void Sdt::setOriginalNetworkId(unsigned short id) {
	originalNetworkId = id;
}

unsigned short Sdt::getOriginalNetworkId() {
	return originalNetworkId;
}

void Sdt::addServiceInformation(ServiceInformation *si) {
	if (si) {
		serviceList.push_back(si);
	}
}

}
}
}
}


