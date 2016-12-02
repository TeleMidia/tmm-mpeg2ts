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

/*
 * AssociationTag.cpp
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#include "si/descriptor/AssociationTag.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

AssociationTag::AssociationTag() : MpegDescriptor(0x14) {
	selectorLength = 0x08;
	selectorByte = NULL;
	privateDataByte = NULL;
	privateDataByteLength = 0;
	use = 0x0000;
}

AssociationTag::~AssociationTag() {
	if (selectorByte) delete selectorByte;
	if (privateDataByte) delete privateDataByte;
}

int AssociationTag::process() {
	int pos = MpegDescriptor::process();

	associationTag = ((stream[pos] & 0xFF) << 8) |
	 	 	 	 	 (stream[pos + 1] & 0xFF);

	pos += 2;

	use = ((stream[pos] & 0xFF) << 8) |
			(stream[pos + 1] & 0xFF);

	pos += 2;

	if (use == 0x0000) {
		selectorLength = stream[pos++] & 0xFF;

		transactionId = ((stream[pos] & 0xFF) << 24) |
						 ((stream[pos + 1] & 0xFF) << 16) |
						 ((stream[pos + 2] & 0xFF) << 8) |
						 (stream[pos + 3] & 0xFF);

		timeout = ((stream[pos] & 0xFF) << 24) |
				 ((stream[pos + 1] & 0xFF) << 16) |
				 ((stream[pos + 2] & 0xFF) << 8) |
				 (stream[pos + 3] & 0xFF);

	} else if (use == 0x0001) {
		selectorLength = stream[pos++] & 0xFF;
	} else {
		selectorLength = stream[pos++] & 0xFF;

		if (selectorByte) delete selectorByte;
		selectorByte = new char[selectorLength];
		for (int i = 0; i < selectorLength; i++) {
			selectorByte[i] = stream[pos + i];
		}

		pos += selectorLength;
	}

	privateDataByteLength = 4;
	if (use == 0x0000) privateDataByteLength += 9;
		else if (use == 0x0001) privateDataByteLength += 1;
			else privateDataByteLength += (selectorLength + 1);
	privateDataByteLength = descriptorLength - privateDataByteLength;

	if (privateDataByte) delete privateDataByte;
	privateDataByte = new char[privateDataByteLength];
	for (int i = 0; i < privateDataByteLength; i++) {
		privateDataByte[i] = stream[pos + i];
	}

	pos += privateDataByteLength;

	return pos;
}

int AssociationTag::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = (associationTag >> 8) & 0xFF;
	stream[pos++] = associationTag & 0xFF;

	stream[pos++] = (use >> 8) & 0xFF;
	stream[pos++] = use & 0xFF;

	if (use == 0x0000) {
		stream[pos++] = selectorLength & 0xFF;

		stream[pos++] = (transactionId >> 24) & 0xFF;
		stream[pos++] = (transactionId >> 16) & 0xFF;
		stream[pos++] = (transactionId >> 8) & 0xFF;
		stream[pos++] = transactionId & 0xFF;

		stream[pos++] = (timeout >> 24) & 0xFF;
		stream[pos++] = (timeout >> 16) & 0xFF;
		stream[pos++] = (timeout >> 8) & 0xFF;
		stream[pos++] = timeout & 0xFF;

	} else if (use == 0x0001) {
		stream[pos++] = selectorLength & 0xFF;

	} else {
		stream[pos++] = selectorLength & 0xFF;

		for (int i = 0; i < selectorLength; i++) {
			stream[pos + i] = selectorByte[i];
		}

		pos += selectorLength;
	}

	for (int i = 0; i < privateDataByteLength; i++) {
		stream[pos + i] = privateDataByte[i];
	}

	pos += privateDataByteLength;

	return pos;
}

unsigned int AssociationTag::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 4;
	if (use == 0x0000) pos += 9;
		else if (use == 0x0001) pos += 1;
			else pos += (selectorLength + 1);
	pos += privateDataByteLength;

	return pos;
}

void AssociationTag::setAssociationTag(unsigned short at) {
	associationTag = at;
}

unsigned short AssociationTag::getAssociationTag() {
	return associationTag;
}

void AssociationTag::setUse(unsigned short use) {
	this->use = use;
}

unsigned short AssociationTag::getUse() {
	return use;
}

unsigned char AssociationTag::getSelectorLength() {
	return selectorLength;
}

void AssociationTag::setTransactionId(unsigned int id) {
	transactionId = id;
}

unsigned int AssociationTag::getTransactionId() {
	return transactionId;
}

void AssociationTag::setTimeout(unsigned int t) {
	timeout = t;
}

unsigned int AssociationTag::getTimeout() {
	return timeout;
}

void AssociationTag::setSelectorByte(char* stream, int length) {
	if (stream) {
		if (selectorByte) delete selectorByte;
		selectorByte = new char[length];
		memcpy(selectorByte, stream, length);
		selectorLength = length;
	}
}

int AssociationTag::getSelectorByte(char** stream) {
	if (selectorByte) {
		*stream = selectorByte;
		return selectorLength;
	}
	return 0;
}

void AssociationTag::setPrivateDataByte(char* stream, int length) {
	if (stream) {
		if (privateDataByte) delete privateDataByte;
		privateDataByte = new char[length];
		memcpy(privateDataByte, stream, length);
		privateDataByteLength = length;
	}
}

int AssociationTag::getPrivateDataByte(char** stream) {
	if (privateDataByte) {
		*stream = privateDataByte;
		return privateDataByteLength;
	}
	return 0;
}

}
}
}
}
