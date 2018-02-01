/*
 * DataComponent.cpp
 *
 *  Created on: 31/01/2018
 *      Author: Felippe Nagato
 */

#include "si/descriptor/DataComponent.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

DataComponent::DataComponent() : MpegDescriptor(0xFD) {
	transmissionFormat = TF_OBJECT_CAROUSEL;
	applicationIdentifierFlag = 1;
}

DataComponent::~DataComponent() {

}

int DataComponent::process() {
	int pos = MpegDescriptor::process();

	dataComponentId = ((stream[pos] & 0xFF) << 8) |
 	 	 	 	 	 	 (stream[pos + 1] & 0xFF);

	pos = pos + 2;

	switch (dataComponentId) {
	case DC_GINGA_APP_EXECUTING_ENGINE:
		transmissionFormat = (stream[pos] & 0xC0) >> 6;
		applicationIdentifierFlag = (stream[pos] & 0x20) >> 5;
		documentResolution = (stream[pos] & 0x1E) >> 1;
		pos++;
		if (applicationIdentifierFlag) {
			organizationId = ((stream[pos] & 0xFF) << 24) |
							((stream[pos + 1] & 0xFF) << 16) |
							((stream[pos + 2] & 0xFF) << 8) |
							(stream[pos + 3] & 0xFF);
			pos = pos + 4;
			applicationId = ((stream[pos] & 0xFF) << 8) |
							(stream[pos + 1] & 0xFF);
			pos = pos + 2;
		}
		switch (transmissionFormat) {
		case 2:
			carouselId = ((stream[pos] & 0xFF) << 24) |
						((stream[pos + 1] & 0xFF) << 16) |
						((stream[pos + 2] & 0xFF) << 8) |
						(stream[pos + 3] & 0xFF);
			pos = pos + 5; //reserved field included
			break;
		}
		break;
	case DC_GINGA_APP_INFORMATION_TABLE:
		applicationType = ((stream[pos] & 0xFF) << 8) |
		 	 	 	 	 (stream[pos + 1] & 0xFF);
		pos = pos + 2;
		AITVersionNumber = stream[pos++] & 0x1F;
		break;
	default:
		break;
	}

	return pos;
}

int DataComponent::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = (dataComponentId >> 8) & 0xFF;
	stream[pos++] = dataComponentId & 0xFF;

	switch (dataComponentId) {
	case DC_GINGA_APP_EXECUTING_ENGINE:
		stream[pos] = (transmissionFormat << 6) & 0xC0;
		stream[pos] = stream[pos] | ((applicationIdentifierFlag << 5) & 0x20);
		stream[pos] = stream[pos] | ((documentResolution << 1) & 0x1E);
		pos++;
		if (applicationIdentifierFlag) {
			stream[pos++] = (organizationId >> 24) & 0xFF;
			stream[pos++] = (organizationId >> 16) & 0xFF;
			stream[pos++] = (organizationId >> 8) & 0xFF;
			stream[pos++] = organizationId & 0xFF;
			stream[pos++] = (applicationId >> 8) & 0xFF;
			stream[pos++] = applicationId & 0xFF;
		}
		switch (transmissionFormat) {
		case 1:
			stream[pos++] = 0xFF;
			break;
		case 2:
			stream[pos++] = (carouselId >> 24) & 0xFF;
			stream[pos++] = (carouselId >> 16) & 0xFF;
			stream[pos++] = (carouselId >> 8) & 0xFF;
			stream[pos++] = carouselId & 0xFF;
			stream[pos++] = 0x1F;
			break;
		}
		break;
	case DC_GINGA_APP_INFORMATION_TABLE:
		stream[pos++] = (applicationType >> 8) & 0xFF;
		stream[pos++] = applicationType & 0xFF;
		stream[pos] = 0xE0;
		stream[pos] = stream[pos] | (AITVersionNumber & 0x1F);
		pos++;
		break;
	default:
		break;
	}

	return pos;
}

unsigned int DataComponent::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 2;

	switch (dataComponentId) {
	case DC_GINGA_APP_EXECUTING_ENGINE:
		pos++;
		if (applicationIdentifierFlag) {
			pos += 6;
		}
		switch (transmissionFormat) {
		case 1:
			pos++;
			break;
		case 2:
			pos += 5;
			break;
		}
		break;
	case DC_GINGA_APP_INFORMATION_TABLE:
		pos += 3;
		break;
	default:
		break;
	}

	return pos;
}

void DataComponent::setDataComponentId(unsigned short id) {
	dataComponentId = id;
}

unsigned short DataComponent::getDataComponentId() {
	return dataComponentId;
}

void DataComponent::setApplicationType(unsigned short type) {
	applicationType = type;
}

unsigned short DataComponent::getApplicationType() {
	return applicationType;
}

void DataComponent::setAITVersionNumber(unsigned char ver) {
	AITVersionNumber = ver;
}

unsigned char DataComponent::getAITVersionNumber() {
	return AITVersionNumber;
}

void DataComponent::setTransmissionFormat(unsigned char format) {
	transmissionFormat = format;
}

unsigned char DataComponent::getTransmissionFormat() {
	return transmissionFormat;
}

void DataComponent::setApplicationIdentifierFlag(unsigned char flag) {
	applicationIdentifierFlag = flag;
}

unsigned char DataComponent::getApplicationIdentifierFlag() {
	return applicationIdentifierFlag;
}

void DataComponent::setDocumentResolution(unsigned char dr) {
	documentResolution = dr;
}

unsigned char DataComponent::getDocumentResolution() {
	return documentResolution;
}

void DataComponent::setOrganizationId(unsigned int id) {
	organizationId = id;
}

unsigned int DataComponent::getOrganizationId() {
	return organizationId;
}

void DataComponent::setApplicationId(unsigned short id) {
	applicationId = id;
}

unsigned short DataComponent::getApplicationId() {
	return applicationId;
}

void DataComponent::setCarouselId(unsigned int id) {
	carouselId = id;
}

unsigned int DataComponent::getCarouselId() {
	return carouselId;
}

}
}
}
}
