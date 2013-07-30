/*
 * Eit.cpp
 *
 *  Created on: 24/07/2013
 *      Author: Felippe Nagato
 */

#include "si/Eit.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Eit::Eit() : PrivateSection() {
	tableId = 0x4E;
}

Eit::~Eit() {
	releaseAllEvents();
}

void Eit::releaseAllEvents() {
	vector<EventInfo*>::iterator itEvent;
	vector<MpegDescriptor*>::iterator itDesc;

	itEvent = eventList.begin();
	while (itEvent != eventList.end()) {
		if (*itEvent) {
			itDesc = (*itEvent)->descriptorsList.begin();
			while (itDesc != (*itEvent)->descriptorsList.end()) {
				if (*itDesc) delete (*itDesc);
				++itDesc;
			}
			delete (*itEvent);
		}
		++itEvent;
	}

	eventList.clear();
}

int Eit::processSectionPayload() {
	unsigned short length, length2;
	MpegDescriptor *d;
	unsigned char descriptorTag;
	unsigned short descriptorSize;
	unsigned int value;
	int pos = PrivateSection::processSectionPayload();

	if (tableId != 0x4E) {
		return -1;
	}

	transportStreamId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
	pos += 2;
	originalNetworkId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
	pos += 2;

	segmentLastSectionNumber = stream[pos] & 0xFF;
	pos += 1;
	lastTableId = stream[pos] & 0xFF;
	pos += 1;

	length = privateSectionLength - 15;
	while (length > 0) {
		EventInfo* ei = new EventInfo();
		ei->eventId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		value = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		ei->startTime = Tot::MJDtoDateTime(value);
		value = ((stream[pos] & 0xFF) << 16) |
				((stream[pos + 1] & 0xFF) << 8) |
				(stream[pos + 2] & 0xFF);
		pos += 3;
		ei->startTime = ei->startTime + Tot::BCDtoTime(value);
		value = ((stream[pos] & 0xFF) << 16) |
				((stream[pos + 1] & 0xFF) << 8) |
				(stream[pos + 2] & 0xFF);
		pos += 3;
		ei->duration = Tot::BCDtoTime(value);
		ei->runningStatus = (stream[pos] >> 5) & 0x07;
		ei->freeCaMode = (stream[pos] >> 7) & 0x01;
		ei->descriptorsLoopLength = ((stream[pos] & 0x0F) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		length -= (ei->descriptorsLoopLength + 12);
		length2 = 0;
		while (length2 < ei->descriptorsLoopLength) {
			descriptorTag = stream[pos] & 0xFF;
			descriptorSize = (stream[pos + 1] & 0xFF) + 2;
			length2 += descriptorSize;
			switch (descriptorTag) {
			case 0x4D: // Short event descriptor
				d = new ShortEvent();
				d->addData(stream + pos, descriptorSize);
				ei->descriptorsList.push_back(d);
				break;
			case 0x50: // Component descriptor
				d = new Component();
				d->addData(stream + pos, descriptorSize);
				ei->descriptorsList.push_back(d);
				break;
			default:
				cout << "Eit::processSectionPayload - Descriptor unrecognized. " <<
						(descriptorTag && 0xFF) << endl;
				break;
			}
			pos = pos + descriptorSize;
		}
	}

	crc32 = ((stream[pos] & 0xFF) << 24) |
			((stream[pos + 1] & 0xFF) << 16) |
			((stream[pos + 2] & 0xFF) << 8) |
			(stream[pos + 3] & 0xFF);

	pos += 4;

	return pos;
}

int Eit::updateStream() {
	vector<EventInfo*>::iterator itEvent;
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	char* dataStream;
	unsigned int value;
	int pos = PrivateSection::updateStream();

	stream[pos++] = ((transportStreamId >> 8) & 0xFF);
	stream[pos++] = transportStreamId & 0xFF;
	stream[pos++] = ((originalNetworkId >> 8) & 0xFF);
	stream[pos++] = originalNetworkId & 0xFF;
	stream[pos++] = segmentLastSectionNumber & 0xFF;
	stream[pos++] = lastTableId & 0xFF;

	itEvent = eventList.begin();
	while (itEvent != eventList.end()) {
		stream[pos++] = (((*itEvent)->eventId >> 8) & 0xFF);
		stream[pos++] = (*itEvent)->eventId & 0xFF;
		value = Tot::dateToMJD((*itEvent)->startTime);
		stream[pos++] = ((value >> 8) & 0xFF);
		stream[pos++] = value & 0xFF;
		value = Tot::timeToBCD((*itEvent)->startTime, 0);
		stream[pos++] = ((value >> 16) & 0xFF);
		stream[pos++] = ((value >> 8) & 0xFF);
		stream[pos++] = value & 0xFF;
		value = Tot::timeToBCD((*itEvent)->duration, 0);
		stream[pos++] = ((value >> 16) & 0xFF);
		stream[pos++] = ((value >> 8) & 0xFF);
		stream[pos++] = value & 0xFF;
		stream[pos] = ((*itEvent)->runningStatus << 5) & 0xE0;
		if ((*itEvent)->freeCaMode) value = 1; else value = 0;
		stream[pos] = stream[pos] | ((value << 4) & 0x10);

		(*itEvent)->descriptorsLoopLength = 0;
		itDesc = (*itEvent)->descriptorsList.begin();
		while (itDesc != (*itEvent)->descriptorsList.end()) {
			(*itEvent)->descriptorsLoopLength += (*itDesc)->getStreamSize();
			++itDesc;
		}

		stream[pos] = stream[pos] | (((*itEvent)->descriptorsLoopLength >> 8) & 0x0F);
		pos += 1;
		stream[pos++] = (*itEvent)->descriptorsLoopLength & 0xFF;

		itDesc = (*itEvent)->descriptorsList.begin();
		while (itDesc != (*itEvent)->descriptorsList.end()) {
			streamLen = (*itDesc)->getStream(&dataStream);
			if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
				memcpy(stream + pos, dataStream, streamLen);
				pos += streamLen;
			} else {
				cout << "Eit::updateStream - Descriptor too long." << endl;
				break;
			}
			itDesc++;
		}
		++itEvent;
	}

	Crc32 crc;
	crc32 = crc.crc(stream, pos);
	stream[pos++] = (crc32 >> 24) & 0xFF;
	stream[pos++] = (crc32 >> 16) & 0xFF;
	stream[pos++] = (crc32 >> 8) & 0xFF;
	stream[pos++] = crc32 & 0xFF;

	return pos;
}

int Eit::calculateSectionSize() {
	vector<EventInfo*>::iterator itEvent;
	vector<MpegDescriptor*>::iterator itDesc;
	unsigned int pos = PrivateSection::calculateSectionSize();

	pos = pos + 6;

	itEvent = eventList.begin();
	while (itEvent != eventList.end()) {
		(*itEvent)->descriptorsLoopLength = 0;
		itDesc = (*itEvent)->descriptorsList.begin();
		while (itDesc != (*itEvent)->descriptorsList.end()) {
			(*itEvent)->descriptorsLoopLength += (*itDesc)->getStreamSize();
			++itDesc;
		}
		pos += (*itEvent)->descriptorsLoopLength + 12;
		++itEvent;
	}

	return pos + 4;
}

vector<EventInfo*>* Eit::getEventList() {
	return &eventList;
}

void Eit::setTransportStreamId(unsigned short id) {
	transportStreamId = id;
}

unsigned short Eit::getTransportStreamId() {
	return transportStreamId;
}

void Eit::setOriginalNetworkId(unsigned short id) {
	originalNetworkId = id;
}

unsigned short Eit::getOriginalNetworkId() {
	return originalNetworkId;
}

void Eit::setSegmentLastSectionNumber(unsigned char slsn) {
	segmentLastSectionNumber = slsn;
}

unsigned char Eit::getSegmentLastSectionNumber() {
	return segmentLastSectionNumber;
}

void Eit::setLastTableId(unsigned char ltid) {
	lastTableId = ltid;
}

unsigned char Eit::getLastTableId() {
	return lastTableId;
}

void Eit::addEventInfo(EventInfo *event) {
	if (event) {
		eventList.push_back(event);
	}
}

}
}
}
}


