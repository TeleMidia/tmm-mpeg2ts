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

#include "dsmcc/DSMCCSection.h"

#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

DSMCCSection::DSMCCSection() : PrivateSection() {
	privateDataByte = NULL;
	dsmccDescritorList = new vector<MpegDescriptor*>;
	privateDataLength = 0;
	checksum = 0;
}

DSMCCSection::~DSMCCSection() {
	clearDsmccDescritor();
	if (dsmccDescritorList != NULL) {
		delete (dsmccDescritorList);
	}
}

int DSMCCSection::processSectionPayload() {
	int pos = PrivateSection::processSectionPayload();
	int sectionPayloadSize = PrivateSection::getSectionPayloadSize();
	int sectionSize = privateSectionLength + 3;
	unsigned char descriptorTag;
	unsigned short descriptorSize;
	MpegDescriptor *d;


	if (tableId == 0x3A) {
		//LLCSNAP()
		pos = pos + sectionPayloadSize;

	} else if ((tableId == 0x3B) || (tableId == 0x3C) || (tableId == 0x3E)) {
		//userNetworkMessage() //downloadDataMessage() //private_data_byte
		if (privateDataByte != NULL) {
			delete (privateDataByte);
		}
		try {
			privateDataByte = new char[sectionPayloadSize];
		} catch(...) {
			return -1;
		}
		memcpy(privateDataByte, stream, sectionPayloadSize);
		pos = pos + sectionPayloadSize;
	} else if (tableId == 0x3D) {
		//dsmccDescriptorList()
		while ((pos + 4) < sectionSize) {
			descriptorTag = stream[pos] & 0xFF;
			descriptorSize = (stream[pos + 1] & 0xFF) + 2;
			switch (descriptorTag) {
				case 0x17: // NPT Reference
					d = new NPTReference();
					d->addData(stream + pos, descriptorSize);
					addDsmccDescriptor(d);
					break;

				case 0x18: // NPT Endpoint
					d = new NPTEndpoint();
					d->addData(stream + pos, descriptorSize);
					addDsmccDescriptor(d);
					break;

				case 0x19: // Stream Mode
					d = new StreamMode();
					d->addData(stream + pos, descriptorSize);
					addDsmccDescriptor(d);
					break;

				case 0x1A: // Stream Event
					d = new StreamEvent();
					d->addData(stream + pos, descriptorSize);
					addDsmccDescriptor(d);
					break;

				default:
					cout << "DSMCCSection::processSectionPayload - ";
					cout << "Descriptor unrecognized: " <<
							(descriptorTag & 0xFF) << endl;
					break;
			}
			pos = pos + descriptorSize;
		}
	}

	if (!sectionSyntaxIndicator) {
		checksum = ((stream[pos] & 0xFF) << 24) |
					((stream[pos + 1] & 0xFF) << 16) |
					((stream[pos + 2] & 0xFF) << 8) |
					(stream[pos + 3] & 0xFF);

	} else {
		crc32 = ((stream[pos] & 0xFF) << 24) |
					((stream[pos + 1] & 0xFF) << 16) |
					((stream[pos + 2] & 0xFF) << 8) |
					(stream[pos + 3] & 0xFF);
	}
	pos += 4;

	return pos;
}

int DSMCCSection::updateStream() {
	int pos;

	if (sectionSyntaxIndicator) {
		privateIndicator = 0x00;
	} else {
		privateIndicator = 0x01;
	}

	pos = PrivateSection::updateStream();

	MpegDescriptor* desc;
	int streamLen;
	char* dataStream;

	if (tableId == 0x3A) {
		//LLCSNAP()
	} else if ((tableId == 0x3B) || (tableId == 0x3C) || (tableId == 0x3E)) {
		//userNetworkMessage() //downloadDataMessage() //private_data_byte
		memcpy(stream + pos, privateDataByte, privateDataLength);
		pos += privateDataLength;
	} else if (tableId == 0x3D) {
		vector<MpegDescriptor*>::iterator i;
		if ((dsmccDescritorList != NULL) &&
				(!dsmccDescritorList->empty())) {
			i = dsmccDescritorList->begin();
			while (i != dsmccDescritorList->end()) {
				desc = *i;
				streamLen = desc->getStream(&dataStream);
				if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
					memcpy(stream + pos, dataStream, streamLen);
					pos += streamLen;
				} else {
					break;
				}
				++i;
			}
		}
	}

	if (!sectionSyntaxIndicator) {
		//TODO: checksum
		stream[pos++] = (checksum >> 24) & 0xFF;
		stream[pos++] = (checksum >> 16) & 0xFF;
		stream[pos++] = (checksum >> 8) & 0xFF;
		stream[pos++] = checksum & 0xFF;
	} else {
		//crc32
		Crc32 crc;
		unsigned int value = crc.crc(stream, pos);
		crc32 = value;
		stream[pos++] = (crc32 >> 24) & 0xFF;
		stream[pos++] = (crc32 >> 16) & 0xFF;
		stream[pos++] = (crc32 >> 8) & 0xFF;
		stream[pos++] = crc32 & 0xFF;
	}
	return pos;
}

int DSMCCSection::calculateSectionSize() {
	unsigned int pos = PrivateSection::calculateSectionSize();
	MpegDescriptor* desc;
	int streamLen;
	if (tableId == 0x3A) {
		//LLCSNAP()
	} else if ((tableId == 0x3B) || (tableId == 0x3C) || (tableId == 0x3E)) {
		//userNetworkMessage() //downloadDataMessage() //private_data_byte
		pos += privateDataLength;
	} else if (tableId == 0x3D) {
		vector<MpegDescriptor*>::iterator i;
		if ((dsmccDescritorList != NULL) &&
				(!dsmccDescritorList->empty())) {
			i = dsmccDescritorList->begin();
			while (i != dsmccDescritorList->end()) {
				desc = *i;
				streamLen = desc->getStreamSize();
				if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
					pos += streamLen;
				} else {
					break;
				}
				++i;
			}
		}
	}
	return pos + 4;
}

vector<MpegDescriptor*>* DSMCCSection::getDsmccDescritorList() {
	return dsmccDescritorList;
}

unsigned int DSMCCSection::getChecksum() {
	return checksum;
}

void DSMCCSection::setChecksum(unsigned int cs) {
	checksum = cs;
}

int DSMCCSection::getPrivateDataByte(char** dataStream) {
	if (privateDataByte != NULL) {
		*dataStream = privateDataByte;
		return privateDataLength;
	} else {
		return 0;
	}
}

int DSMCCSection::setPrivateDataByte(char* data, unsigned short length) {
	if (length > 4084) {
		length = 4084;
		cout << "DSMCCSection::setPrivateDataByte - Data truncated!" << endl;
	}
	if (privateDataByte != NULL) {
		delete (privateDataByte);
	}
	try {
		privateDataByte = new char[length];
	} catch(...) {
		return -1;
	}
	memcpy(privateDataByte, data, length);
	privateDataLength = length;
	return privateDataLength;
}

void DSMCCSection::addDsmccDescriptor(MpegDescriptor* d) {
	dsmccDescritorList->push_back(d);
}

void DSMCCSection::removeDsmccDescriptor(unsigned char descriptorTag) {
	MpegDescriptor* desc;
	vector<MpegDescriptor*>::iterator i;
	if ((dsmccDescritorList != NULL) && (!dsmccDescritorList->empty())) {
		i = dsmccDescritorList->begin();
		while (i != dsmccDescritorList->end()) {
			desc = *i;
			if (desc->getDescriptorTag() == descriptorTag) {
				delete (desc);
				dsmccDescritorList->erase(i);
				break;
			}
			++i;
		}
	}
}

void DSMCCSection::clearDsmccDescritor() {
	MpegDescriptor* desc;
	vector<MpegDescriptor*>::iterator i;
	if ((dsmccDescritorList != NULL) && (!dsmccDescritorList->empty())) {
		i = dsmccDescritorList->begin();
		while (i != dsmccDescritorList->end()) {
			desc = *i;
			delete (desc);
			++i;
		}
		dsmccDescritorList->clear();
	}
}

}
}
}
}
}

