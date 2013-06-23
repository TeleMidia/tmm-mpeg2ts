/*
 * Nit.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/Nit.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Nit::Nit() : PrivateSection() {
	tableId = 0x40;
}

Nit::~Nit() {
	releaseAllDescriptors();
	releaseAllTransportInformation();
}

void Nit::releaseAllDescriptors() {
	vector<MpegDescriptor*>::iterator itDesc;

	itDesc = descriptorsList.begin();
	while (itDesc != descriptorsList.end()) {
		if (*itDesc) {
			delete (*itDesc);
		}
		++itDesc;
	}

	descriptorsList.clear();
}

void Nit::releaseAllTransportInformation() {
	vector<TransportInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	it = transportList.begin();
	while (it != transportList.end()) {
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

	transportList.clear();
}

int Nit::processSectionPayload() {
	unsigned short length;
	MpegDescriptor *d;
	unsigned char descriptorTag;
	unsigned short descriptorSize;
	int pos = PrivateSection::processSectionPayload();

	if (tableId != 0x40) {
		return -1;
	}

	networkDescriptorsLength = ((stream[pos] & 0x0F) << 8) | (stream[pos + 1] & 0xFF);
	pos += 2;

	length = 0;
	while (length < networkDescriptorsLength) {
		descriptorTag = stream[pos] & 0xFF;
		descriptorSize = (stream[pos + 1] & 0xFF) + 2;
		length += descriptorSize;
		switch (descriptorTag) {
		case 0x40: // Network name descriptor
			d = new NetworkName();
			d->addData(stream + pos, descriptorSize);
			descriptorsList.push_back(d);
			break;
		case 0xFE: // System management descriptor
			//d = new SystemManagement();
			d->addData(stream + pos, descriptorSize);
			descriptorsList.push_back(d);
			break;
		default:
			cout << "Nit::processSectionPayload - Descriptor unrecognized. " <<
					(descriptorTag && 0xFF) << endl;
			break;
		}
		pos = pos + descriptorSize;
	}

	transportStreamLoopLength = ((stream[pos] & 0x0F) << 8) | (stream[pos + 1] & 0xFF);
	pos += 2;

	while (pos + 4 < privateSectionLength + 3) {
		TransportInformation *ti = new TransportInformation();
		ti->transportStreamId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		ti->originalNetworkId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		ti->transportDescriptorsLength = ((stream[pos] & 0x0F) << 8) | (stream[pos + 1] & 0xFF);
		pos += 2;
		length = 0;
		while (length < ti->transportDescriptorsLength) {
			descriptorTag = stream[pos] & 0xFF;
			descriptorSize = (stream[pos + 1] & 0xFF) + 2;
			length += descriptorSize;
			switch (descriptorTag) {
			case 0x41: // Service list descriptor
				d = new ServiceList();
				d->addData(stream + pos, descriptorSize);
				ti->descriptorList.push_back(d);
				break;
			case 0xCD: // TS Information Descriptor
				//d = new TSInformation();
				d->addData(stream + pos, descriptorSize);
				ti->descriptorList.push_back(d);
				break;
			case 0xFA: // Terrestrial delivery system descriptor
				//d = new TerrestrialDeliverySystem();
				d->addData(stream + pos, descriptorSize);
				ti->descriptorList.push_back(d);
				break;
			case 0xFB: // Partial reception descriptor
				//d = new PartialReception();
				d->addData(stream + pos, descriptorSize);
				ti->descriptorList.push_back(d);
				break;
			case 0xFD: // Data component descriptor
				//d = new DataComponent();
				d->addData(stream + pos, descriptorSize);
				ti->descriptorList.push_back(d);
				break;
			default:
				cout << "Nit::processSectionPayload - Descriptor unrecognized. " <<
						(descriptorTag && 0xFF) << endl;
				break;
			}
			pos = pos + descriptorSize;
		}
		transportList.push_back(ti);
	}

	crc32 = ((stream[pos] & 0xFF) << 24) |
			((stream[pos + 1] & 0xFF) << 16) |
			((stream[pos + 2] & 0xFF) << 8) |
			(stream[pos + 3] & 0xFF);

	pos += 4;

	return pos;
}

int Nit::updateStream() {
	vector<TransportInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	char* dataStream;
	int pos = PrivateSection::updateStream();

	networkDescriptorsLength = 0;
	itDesc = descriptorsList.begin();
	while (itDesc != descriptorsList.end()) {
		networkDescriptorsLength += (*itDesc)->getStreamSize();
		++itDesc;
	}

	stream[pos] = 0xF0;
	stream[pos] = stream[pos] | ((networkDescriptorsLength >> 8) & 0x0F);
	pos++;
	stream[pos++] = networkDescriptorsLength & 0xFF;

	itDesc = descriptorsList.begin();
	while (itDesc != descriptorsList.end()) {
		streamLen = (*itDesc)->getStream(&dataStream);
		if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
			memcpy(stream + pos, dataStream, streamLen);
			pos += streamLen;
		} else {
			cout << "Nit::updateStream - Descriptor too long." << endl;
			break;
		}
		++itDesc;
	}

	transportStreamLoopLength = 0;
	it = transportList.begin();
	while (it != transportList.end()) {
		(*it)->transportDescriptorsLength = 0;
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			(*it)->transportDescriptorsLength += (*itDesc)->getStreamSize();
			itDesc++;
		}
		transportStreamLoopLength += (*it)->transportDescriptorsLength + 6;
		++it;
	}

	stream[pos] = 0xF0;
	stream[pos] = stream[pos] | ((transportStreamLoopLength >> 8) & 0x0F);
	pos++;
	stream[pos++] = transportStreamLoopLength & 0xFF;

	it = transportList.begin();
	while (it != transportList.end()) {
		stream[pos++] = ((*it)->transportStreamId >> 8) & 0xFF;
		stream[pos++] = (*it)->transportStreamId & 0xFF;
		stream[pos++] = ((*it)->originalNetworkId >> 8) & 0xFF;
		stream[pos++] = (*it)->originalNetworkId & 0xFF;
		(*it)->transportDescriptorsLength = 0;
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			(*it)->transportDescriptorsLength += (*itDesc)->getStreamSize();
			itDesc++;
		}
		stream[pos] = 0xF0;
		stream[pos] = stream[pos] | (((*it)->transportDescriptorsLength >> 8) & 0x0F);
		pos++;
		stream[pos++] = ((*it)->transportDescriptorsLength & 0xFF);
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			streamLen = (*itDesc)->getStream(&dataStream);
			if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
				memcpy(stream + pos, dataStream, streamLen);
				pos += streamLen;
			} else {
				cout << "Nit::updateStream - Descriptor too long." << endl;
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

int Nit::calculateSectionSize() {
	vector<TransportInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	unsigned int pos = PrivateSection::calculateSectionSize();

	pos = pos + 4;

	networkDescriptorsLength = 0;
	itDesc = descriptorsList.begin();
	while (itDesc != descriptorsList.end()) {
		networkDescriptorsLength += (*itDesc)->getStreamSize();
		++itDesc;
	}

	pos += networkDescriptorsLength;

	transportStreamLoopLength = 0;
	it = transportList.begin();
	while (it != transportList.end()) {
		(*it)->transportDescriptorsLength = 0;
		itDesc = (*it)->descriptorList.begin();
		while (itDesc != (*it)->descriptorList.end()) {
			(*it)->transportDescriptorsLength += (*itDesc)->getStreamSize();
			itDesc++;
		}
		pos += (*it)->transportDescriptorsLength + 6;
		transportStreamLoopLength += (*it)->transportDescriptorsLength + 6;
		++it;
	}

	return pos + 4;
}

vector<MpegDescriptor*>* Nit::getDescriptorsList() {
	return &descriptorsList;
}

void Nit::addDescriptor(MpegDescriptor* d) {
	descriptorsList.push_back(d);
}

void Nit::setTransportStreamId(unsigned short id) {
	transportStreamLoopLength = id;
}

unsigned short Nit::getTransportStreamId() {
	return transportStreamLoopLength;
}

void Nit::addTransportInformation(TransportInformation *ti) {
	if (ti) {
		transportList.push_back(ti);
	}
}

}
}
}
}


