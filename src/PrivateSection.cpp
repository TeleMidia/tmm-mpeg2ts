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

#include "PrivateSection.h"

#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

PrivateSection::PrivateSection() {
	stream = new char[MAX_SECTION_SIZE];
	tableId = 0;
	sectionSyntaxIndicator = 1;
	privateIndicator = 0;
	privateSectionLength = 0;
	tableIdExtension = 0;
	versionNumber = 0;
	currentNextIndicator = 0;
	sectionNumber = 0;
	lastSectionNumber = 0;
	crc32 = 0;
	currentSize = 0;
}

PrivateSection::~PrivateSection() {
	delete (stream);
}

int PrivateSection::addData(char* data, unsigned int length) {
	unsigned char headerSize;
	unsigned int rbytes, secSize;

	if (currentSize == 0) {

		if (length < 3) {
			return -1; //error
		}

		tableId = (data[0] & 0xFF);
		sectionSyntaxIndicator = (data[1] & 0x80) >> 7;

		if (sectionSyntaxIndicator) {
			headerSize = 8;
		} else {
			headerSize = 3;
		}

		if (length >= headerSize) {

			if (tableId == 0x00 || tableId == 0x02) {
				privateSectionLength = (((data[1] & 0x03) << 8) |
						(data[2] & 0xFF));

			} else {
				privateSectionLength = (((data[1] & 0x0F) << 8) |
						(data[2] & 0xFF));
			}

			if (sectionSyntaxIndicator) {
				tableIdExtension = (((data[3] & 0xFF) << 8) |
						(data[4] & 0xFF));
				versionNumber = (data[5] & 0x3E) >> 1;
				currentNextIndicator = (data[5] & 0x01);
				sectionNumber = (data[6] & 0xFF);
				lastSectionNumber = (data[7] & 0xFF);
			}

		} else {
			return -1;
		}
	}

	secSize = privateSectionLength + 3;
	if (currentSize + length >= secSize) {
		rbytes = secSize - currentSize;
	} else {
		rbytes = length;
	}

	memcpy(stream + currentSize, data, rbytes);

	currentSize = currentSize + rbytes;

	if (isConsolidated()) {
		if (rbytes > 0) {
			processSectionPayload();
		}
	}

	return rbytes;
}

int PrivateSection::processSectionPayload() {
	if (sectionSyntaxIndicator) {
		return 8;
	} else {
		return 3;
	}
}

int PrivateSection::getSectionPayload(char** dataStream) {
	unsigned char headerSize;
	unsigned short payloadSize;
	if (sectionSyntaxIndicator) {
		headerSize = 8;
		payloadSize = privateSectionLength - 9;
	} else {
		headerSize = 3;
		payloadSize = privateSectionLength;
	}
	if (isConsolidated()) {
		*dataStream = stream + headerSize;
		return payloadSize;
	} else {
		return -1;
	}
}

int PrivateSection::getSectionPayloadSize() {
	if (isConsolidated()) {
		if (sectionSyntaxIndicator) {
			return (privateSectionLength - 9);
		} else {
			return privateSectionLength;
		}
	} else {
		return -1;//not available
	}
}

unsigned char PrivateSection::isConsolidated() {
	unsigned int len = privateSectionLength + 3;
	return (len <= currentSize);
}

int PrivateSection::updateStream() {
	int pos = 0;

	privateSectionLength = calculateSectionSize() - 3;
	currentSize = privateSectionLength + 3;

	stream[pos++] = tableId;
	stream[pos] = stream[pos] & 0x7F;
	stream[pos] = stream[pos] |	((sectionSyntaxIndicator << 7) & 0x80);
	stream[pos] = stream[pos] & 0xBF;
	stream[pos] = stream[pos] |	((privateIndicator << 6) & 0x40);

	stream[pos] = stream[pos] |	0x30;

	stream[pos] = stream[pos] & 0xF0;
	stream[pos] = stream[pos] |	((privateSectionLength >> 8) & 0x0F);
	pos++;
	stream[pos++] = privateSectionLength & 0xFF;

	if (sectionSyntaxIndicator ||
			((tableId >= 0x3A) && (tableId <= 0x3E))) {

		stream[pos++] = (tableIdExtension >> 8) & 0xFF;
		stream[pos++] = tableIdExtension & 0xFF;

		stream[pos] = 0xC0;

		stream[pos] = stream[pos] |	((versionNumber << 1) & 0x3E);
		stream[pos] = stream[pos] |	(currentNextIndicator & 0x01);
		pos++;
		stream[pos++] = sectionNumber & 0xFF;
		stream[pos++] = lastSectionNumber & 0xFF;
	}

	return pos;
}

int PrivateSection::calculateSectionSize() {
	if (sectionSyntaxIndicator) {
		return 8;
	} else {
		return 3;
	}
}

unsigned char PrivateSection::getTableId() {
	return tableId;
}

void PrivateSection::setTableId(unsigned char id) {
	tableId = id;
}

unsigned char PrivateSection::getSectionSyntaxIndicator() {
	return sectionSyntaxIndicator;
}

void PrivateSection::setSectionSyntaxIndicator(unsigned char syntax) {
	sectionSyntaxIndicator = syntax;
}

unsigned char PrivateSection::getPrivateIndicator() {
	return privateIndicator;
}

void PrivateSection::setPrivateIndicator(unsigned char pi) {
	privateIndicator = pi;
}

unsigned short PrivateSection::getPrivateSectionLength() {
	return privateSectionLength;
}

unsigned short PrivateSection::getTableIdExtension() {
	return tableIdExtension;
}

void PrivateSection::setTableIdExtension(unsigned short id) {
	tableIdExtension = id;
}

unsigned char PrivateSection::getVersionNumber() {
	return versionNumber;
}

void PrivateSection::setVersionNumber(unsigned char ver) {
	versionNumber = ver;
}

unsigned char PrivateSection::getCurrentNextIndicator() {
	return currentNextIndicator;
}

void PrivateSection::setCurrentNextIndicator(unsigned char ni) {
	currentNextIndicator = ni;
}

unsigned char PrivateSection::getSectionNumber() {
	return sectionNumber;
}

void PrivateSection::setSectionNumber(unsigned char sec) {
	sectionNumber = sec;
}

unsigned char PrivateSection::getLastSectionNumber() {
	return lastSectionNumber;
}

void PrivateSection::setLastSectionNumber(unsigned char last) {
	lastSectionNumber = last;
}

unsigned int PrivateSection::getCrc32() {
	return crc32;
}

void PrivateSection::setCrc32(unsigned int crc) {
	crc32 = crc;
}

int PrivateSection::getStream(char** dataStream) {
	if (dataStream != NULL) *dataStream = stream;
	return currentSize;
}

}
}
}
}
