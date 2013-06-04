/*
 * Ait.cpp
 *
 *  Created on: 18/03/2013
 *      Author: Felippe Nagato
 */

#include "si/Ait.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Ait::Ait() : PrivateSection() {
	tableId = 0x74;
}

Ait::~Ait() {
	vector<AppInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;

	it = appInformationList.begin();
	while (it != appInformationList.end()) {
		if ((*it)->appDescriptorList) {
			itDesc = (*it)->appDescriptorList->begin();
			while (itDesc != (*it)->appDescriptorList->end()) {
				delete (*itDesc);
				++itDesc;
			}
			delete (*it)->appDescriptorList;
		}
		delete (*it);
		++it;
	}
	itDesc = descriptorList.begin();
	while (itDesc != descriptorList.end()) {
		delete (*itDesc);
		++itDesc;
	}
}

int Ait::createAndAddDescriptor(unsigned char descriptorTag,
				                unsigned short descriptorSize, char* buffer,
				                vector<MpegDescriptor*>* descList) {
	MpegDescriptor *d;

	switch (descriptorTag) {
	case 0x00: // Application descriptor
		d = new Application();
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	case 0x01: // Application name
		d = new ApplicationName();
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	case 0x02: // Transport protocol
		d = new TransportProtocol();
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	case 0x03: // Ginga-J application
		d = new GingaApplication(descriptorTag);
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	case 0x04: // Ginga-J application location
		d = new GingaApplicationLocation(descriptorTag);
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	case 0x06: // Ginga-NCL application
		d = new GingaApplication(descriptorTag);
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	case 0x07: // Ginga-NCL application location
		d = new GingaApplicationLocation(descriptorTag);
		d->addData(buffer, descriptorSize);
		descList->push_back(d);
		break;

	default:
		cout << "Descriptor unrecognized. " << (descriptorTag && 0xFF) << endl;
		break;
	}
	return 0;
}

int Ait::processSectionPayload() {
	int pos = PrivateSection::processSectionPayload();
	unsigned short cdlength = 0, cdlength2 = 0;
	unsigned char descriptorTag;
	unsigned short descriptorSize;

	if (tableId != 0x74) {
		return -1;
	}

	commonDescriptorLength = (((stream[pos] & 0x0F) << 8) |
							   (stream[pos+1] & 0xFF));

	pos += 2;

	while (cdlength < commonDescriptorLength) {
		descriptorTag = stream[pos] & 0xFF;
		descriptorSize = (stream[pos + 1] & 0xFF) + 2;
		cdlength += descriptorSize;
		createAndAddDescriptor(descriptorTag, descriptorSize, stream + pos,
							   &descriptorList);
		pos = pos + descriptorSize;
	}

	applicationLoopLength = (((stream[pos] & 0x0F) << 8) |
			   	   	   	      (stream[pos+1] & 0xFF));

	pos += 2;

	cdlength = 0;

	while (cdlength < applicationLoopLength) {
		AppInformation *appinfo = new AppInformation();
		appinfo->organizationId = (
				((stream[pos] & 0xFF) << 24) | ((stream[pos+1] & 0xFF) << 16) |
				((stream[pos+2] & 0xFF) << 8) | (stream[pos+3] & 0xFF));
		pos += 4;
		appinfo->applicationId = (((stream[pos] & 0xFF) << 8) |
 	   	   	                       (stream[pos+1] & 0xFF));
		pos += 2;
		appinfo->applicationControlCode = stream[pos++] & 0xFF;
		appinfo->descriptorLoopLength = (((stream[pos] & 0x0F) << 8) |
 	   	   	                              (stream[pos+1] & 0xFF));
		pos += 2;
		cdlength += 9;
		appinfo->appDescriptorList = new vector<MpegDescriptor*>;
		while (cdlength2 < appinfo->descriptorLoopLength) {
			descriptorTag = stream[pos] & 0xFF;
			descriptorSize = (stream[pos + 1] & 0xFF) + 2;
			cdlength += descriptorSize;
			cdlength2 += descriptorSize;
			createAndAddDescriptor(descriptorTag, descriptorSize, stream + pos,
					               appinfo->appDescriptorList);
			pos = pos + descriptorSize;
		}
		appInformationList.push_back(appinfo);
	}

	if (sectionSyntaxIndicator) {
		crc32 = ((stream[pos] & 0xFF) << 24) |
					((stream[pos + 1] & 0xFF) << 16) |
					((stream[pos + 2] & 0xFF) << 8) |
					(stream[pos + 3] & 0xFF);
		pos += 4;
	}

	return pos;
}

int Ait::updateStream() {
	vector<AppInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	char* dataStream;
	int pos = PrivateSection::updateStream();

	commonDescriptorLength = 0;
	itDesc = descriptorList.begin();
	while (itDesc != descriptorList.end()) {
		commonDescriptorLength += (*itDesc)->getStreamSize();
		++itDesc;
	}

	stream[pos] = 0xF0;
	stream[pos] = stream[pos] | ((commonDescriptorLength >> 8) & 0xFF);
	pos++;
	stream[pos++] = commonDescriptorLength & 0xFF;

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

	applicationLoopLength = 0;
	it = appInformationList.begin();
	while (it != appInformationList.end()) {
		applicationLoopLength += 9;
		applicationLoopLength += (*it)->descriptorLoopLength;
		++it;
	}

	stream[pos] = 0xF0;
	stream[pos] = stream[pos] | ((applicationLoopLength >> 8) & 0xFF);
	pos++;
	stream[pos++] = applicationLoopLength & 0xFF;

	it = appInformationList.begin();
	while (it != appInformationList.end()) {
		stream[pos++] = ((*it)->organizationId >> 24) & 0xFF;
		stream[pos++] = ((*it)->organizationId >> 16) & 0xFF;
		stream[pos++] = ((*it)->organizationId >> 8) & 0xFF;
		stream[pos++] = (*it)->organizationId & 0xFF;
		stream[pos++] = ((*it)->applicationId >> 8) & 0xFF;
		stream[pos++] = (*it)->applicationId & 0xFF;
		stream[pos++] = (*it)->applicationControlCode & 0xFF;
		stream[pos] = 0xF0;
		stream[pos] = stream[pos] | (((*it)->descriptorLoopLength >> 8) & 0xFF);
		pos++;
		stream[pos++] = (*it)->descriptorLoopLength & 0xFF;
		if ((*it)->appDescriptorList) {
			itDesc = (*it)->appDescriptorList->begin();
			while (itDesc != (*it)->appDescriptorList->end()) {
				streamLen = (*itDesc)->getStream(&dataStream);
				if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
					memcpy(stream + pos, dataStream, streamLen);
					pos += streamLen;
				} else {
					break;
				}
				++itDesc;
			}
		}
		++it;
	}

	if (sectionSyntaxIndicator) {
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

int Ait::calculateSectionSize() {
	vector<AppInformation*>::iterator it;
	vector<MpegDescriptor*>::iterator itDesc;
	int streamLen;
	unsigned int pos = PrivateSection::calculateSectionSize();

	pos += 4;

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

	it = appInformationList.begin();
	while (it != appInformationList.end()) {
		pos += 9;
		if ((*it)->appDescriptorList) {
			itDesc = (*it)->appDescriptorList->begin();
			while (itDesc != (*it)->appDescriptorList->end()) {
				streamLen = (*itDesc)->getStreamSize();
				if ((pos + streamLen + 4) <= MAX_SECTION_SIZE) {
					pos += streamLen;
				} else {
					break;
				}
				++itDesc;
		}
		}
		++it;
	}

	return pos + 4;
}

unsigned short Ait::getCommonDescriptorLength() {
	return commonDescriptorLength;
}

vector<MpegDescriptor*>* Ait::getDescriptorList() {
	return &descriptorList;
}

unsigned short Ait::getApplicationLoopLength() {
	return applicationLoopLength;
}

vector<AppInformation*>* Ait::getAppInformationList() {
	return &appInformationList;
}

void Ait::addCommonDescriptor(MpegDescriptor *d) {
	descriptorList.push_back(d);
}

void Ait::addApplicationInfo(unsigned int organizationId,
						 	unsigned short applicationId,
							unsigned char applicationControlCode,
							vector<MpegDescriptor*>* appDescriptorList) {
	vector<MpegDescriptor*>::iterator itDesc;
	AppInformation *appinfo = new AppInformation();
	appinfo->organizationId = organizationId;
	appinfo->applicationId = applicationId;
	appinfo->applicationControlCode = applicationControlCode;
	appinfo->appDescriptorList = appDescriptorList;
	appinfo->descriptorLoopLength = 0;
	if (appDescriptorList) {
		itDesc = appinfo->appDescriptorList->begin();
		while (itDesc != appinfo->appDescriptorList->end()) {
			appinfo->descriptorLoopLength += (*itDesc)->getStreamSize();
			++itDesc;
		}
	}
	appInformationList.push_back(appinfo);
}


}
}
}
}
