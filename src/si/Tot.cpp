/*
 * Tot.cpp
 *
 *  Created on: 03/05/2013
 *      Author: Felippe Nagato
 */

#include "si/Tot.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Tot::Tot() : PrivateSection() {
	tableId = 0x73;
	sectionSyntaxIndicator = 0;
}

Tot::~Tot() {
	releaseAllDescriptors();
}

void Tot::releaseAllDescriptors() {
	vector<MpegDescriptor*>::iterator itDesc;
	itDesc = descriptorList.begin();
	while (itDesc != descriptorList.end()) {
		delete (*itDesc);
		++itDesc;
	}
	descriptorList.clear();
}

int Tot::processSectionPayload() {
	unsigned short length = 0;
	unsigned int value = 0;
	MpegDescriptor *d;
	unsigned char descriptorTag;
	unsigned short descriptorSize;
	int pos = PrivateSection::processSectionPayload();

	if (tableId != 0x73) {
		return -1;
	}

	value = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
	pos += 2;

	dateTime = MJDtoDateTime(value);

	value = 0;
	value = ((stream[pos] & 0xFF) << 16) |
			((stream[pos + 1] & 0xFF) << 8) |
			(stream[pos + 2] & 0xFF);
	pos += 3;

	dateTime = dateTime + BCDtoTime(value);

	descriptorsLength = (((stream[pos] & 0x0F) << 8) |
						(stream[pos+1] & 0xFF));

	pos += 2;

	while (length < descriptorsLength) {
		descriptorTag = stream[pos] & 0xFF;
		descriptorSize = (stream[pos + 1] & 0xFF) + 2;
		length += descriptorSize;
		switch (descriptorTag) {
		case 0x58: // Local time offset descriptor
			d = new LocalTimeOffset();
			d->addData(stream + pos, descriptorSize);
			descriptorList.push_back(d);
			break;
		default:
			cout << "Tot::processSectionPayload - Descriptor unrecognized: " <<
					(descriptorTag & 0xFF) << endl;
			break;
		}
		pos = pos + descriptorSize;
	}

	crc32 = ((stream[pos] & 0xFF) << 24) |
			((stream[pos + 1] & 0xFF) << 16) |
			((stream[pos + 2] & 0xFF) << 8) |
			(stream[pos + 3] & 0xFF);

	pos += 4;

	return pos;
}

int Tot::updateStream() {
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	char* dataStream;
	int pos = PrivateSection::updateStream();
	unsigned int value = (unsigned int) dateToMJD(dateTime);

	stream[pos++] = (value >> 8) & 0xFF;
	stream[pos++] = value & 0xFF;

	value = timeToBCD(dateTime, 0);

	stream[pos++] = (value >> 16) & 0xFF;
	stream[pos++] = (value >> 8) & 0xFF;
	stream[pos++] = value & 0xFF;

	descriptorsLength = 0;
	itDesc = descriptorList.begin();
	while (itDesc != descriptorList.end()) {
		descriptorsLength += (*itDesc)->getStreamSize();
		++itDesc;
	}

	stream[pos] = 0xF0;
	stream[pos] = stream[pos] | ((descriptorsLength >> 8) & 0xFF);
	pos++;
	stream[pos++] = descriptorsLength & 0xFF;

	itDesc = descriptorList.begin();
	while (itDesc != descriptorList.end()) {
		streamLen = (*itDesc)->getStream(&dataStream);
		if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
			memcpy(stream + pos, dataStream, streamLen);
			pos += streamLen;
		} else {
			break;
		}
		++itDesc;
	}

	Crc32 crc;
	crc32 = crc.crc(stream, pos);
	stream[pos++] = (crc32 >> 24) & 0xFF;
	stream[pos++] = (crc32 >> 16) & 0xFF;
	stream[pos++] = (crc32 >> 8) & 0xFF;
	stream[pos++] = crc32 & 0xFF;

	return pos;
}

int Tot::calculateSectionSize() {
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	unsigned int pos = PrivateSection::calculateSectionSize();

	pos = pos + 7;

	itDesc = descriptorList.begin();
	while (itDesc != descriptorList.end()) {
		streamLen = (*itDesc)->getStreamSize();
		if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
			pos += streamLen;
		} else {
			break;
		}
		++itDesc;
	}

	return pos + 4;
}

void Tot::addDescriptor(MpegDescriptor *d) {
	descriptorList.push_back(d);
}

void Tot::setDateTime(time_t rawTime) {
	this->dateTime = rawTime;
}

time_t Tot::getDateTime() {
	return dateTime;
}

unsigned int Tot::dateToMJD(time_t rawTime) {
	return MJD_1970_01_01 + ((rawTime) / 86400.0);
}

time_t Tot::MJDtoDateTime(unsigned int mjd) {
	return (long) (((mjd) - MJD_1970_01_01) * 86400.0);
}

unsigned char Tot::decToBCD(unsigned char val) {
  return (val/10*16) + (val%10);
}

unsigned char Tot::BCDtoDec(unsigned char val) {
  return (val/16*10) + (val%16);
}

unsigned int Tot::timeToBCD(time_t rawTime, char utc) {
	struct tm * ptm;
	unsigned int bcd = 0;

	ptm = gmtime (&rawTime);

	bcd = decToBCD((ptm->tm_hour & 0xFF) + utc) << 16;
	bcd = bcd | (decToBCD(ptm->tm_min & 0xFF) << 8);
	bcd = bcd | decToBCD(ptm->tm_sec & 0xFF);

	return bcd;
}

time_t Tot::BCDtoTime(unsigned int bcd) {
	int hh, mm, ss;
	struct tm when = {0};

	hh = BCDtoDec(bcd >> 16) & 0xFF;
	mm = BCDtoDec(bcd >> 8) & 0xFF;
	ss = BCDtoDec(bcd & 0xFF);

	when.tm_mday = 1;
	when.tm_mon = 0;
	when.tm_year = 70;
	when.tm_hour = hh;
	when.tm_min = mm;
	when.tm_sec = ss;

	return mktime(&when) + (localTimezone() * 60);
}

short Tot::localTimezone() {
	struct tm when = {0};

	when.tm_mday = 1;
	when.tm_mon = 0;
	when.tm_year = 70;
	when.tm_hour = 0;
	when.tm_min = 0;
	when.tm_sec = 0;

	return ((0 - mktime(&when)) / 60);
}

time_t Tot::makeUtcTime(unsigned char hh, unsigned char mm,
						unsigned char ss) {
	struct tm when = {0};

	when.tm_mday = 1;
	when.tm_mon = 0;
	when.tm_year = 70;
	when.tm_hour = hh;
	when.tm_min = mm;
	when.tm_sec = ss;

	return mktime(&when) + (localTimezone() * 60);
}

}
}
}
}
