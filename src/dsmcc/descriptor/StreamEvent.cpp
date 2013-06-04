/******************************************************************************
Este arquivo eh parte da implementacao das ferramentas DTV do TeleMidia

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the TeleMidia DTV Tools

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "dsmcc/descriptor/StreamEvent.h"

#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

StreamEvent::StreamEvent() : MpegDescriptor(0x04) {
	privateDataPayloadLength = 0;
	privateDataLength = 0;
	privateDataPayload = NULL;
	eventId = 0;
	commandTag = 0;
	sequenceNumber = 0;
	finalFlag = 0;
	fcs = 0;
	descriptorLength = 1;
}

StreamEvent::~StreamEvent() {
	if (privateDataPayload != NULL) {
		delete (privateDataPayload);
	}
}

int StreamEvent::process() {
	int pos = MpegDescriptor::process();

	eventId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
	pos = pos + 5;

	eventNPT = 0;
	eventNPT = (stream[pos++] & 0x01);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);

	privateDataLength = (stream[pos++] & 0xFF);
	commandTag = (stream[pos++] & 0xFF);
	sequenceNumber = (stream[pos] & 0x7F) >> 1;
	finalFlag = (stream[pos++] & 0x01);

	if (privateDataPayload != NULL) {
		delete (privateDataPayload);
	}
	try {
		privateDataPayloadLength = privateDataLength - 3;
		privateDataPayload = new char[privateDataPayloadLength];
		memcpy(privateDataPayload, stream + pos, privateDataPayloadLength);
	} catch(...) {
		return -1;
	}

	pos = pos + privateDataPayloadLength;

	fcs = (stream[pos++] & 0xFF);
	return 0;
}

int StreamEvent::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = ((eventId >> 8) & 0xFF);
	stream[pos++] = (eventId & 0xFF);
	stream[pos++] = 0xFF;
	stream[pos++] = 0xFF;
	stream[pos++] = 0xFF;
	stream[pos]   = 0xFE;

	pos = pos + 5;
	stream[pos + 1] = stream[pos + 1] | ((eventNPT >> 32) & 0x01);

	pos++;
	stream[pos++] = (eventNPT >> 24) & 0xFF;
	stream[pos++] = (eventNPT >> 16) & 0xFF;
	stream[pos++] = (eventNPT >> 8) & 0xFF;
	stream[pos++] = eventNPT & 0xFF;

	privateDataLength = calculateDescriptorSize() - 13;

	stream[pos++] = privateDataLength & 0xFF;
	stream[pos++] = commandTag & 0xFF;
	stream[pos] = (sequenceNumber << 1) & 0xFE;

	pos++;
	stream[pos] = stream[pos] | (finalFlag & 0x01);

	memcpy(stream + pos, privateDataPayload, privateDataPayloadLength);
	pos = pos + privateDataPayloadLength;

	stream[pos++] = (fcs & 0xFF);

	return pos;
}

unsigned int StreamEvent::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();
	return pos + 14 + privateDataPayloadLength;
}

unsigned short StreamEvent::getEventId() {
	return eventId;
}

uint64_t StreamEvent::getEventNPT() {
	return eventNPT;
}

unsigned char StreamEvent::getPrivateDataLength() {
	return privateDataLength;
}

unsigned char StreamEvent::getCommandTag() {
	return commandTag;
}

unsigned char StreamEvent::getSequenceNumber() {
	return sequenceNumber;
}

unsigned char StreamEvent::getFinalFlag() {
	return finalFlag;
}

int StreamEvent::getPrivateDataPayload(char** dataStream) {
	*dataStream = privateDataPayload;
	return privateDataPayloadLength;
}

unsigned char StreamEvent::getFcs() {
	return fcs;
}

void StreamEvent::setEventId(unsigned short id) {
	eventId = id;
}

void StreamEvent::setEventNPT(uint64_t npt) {
	eventNPT = npt;
}

void StreamEvent::setCommandTag(unsigned char tag) {
	commandTag = tag;
}

void StreamEvent::setSequenceNumber(unsigned char seqNum) {
	sequenceNumber = (seqNum & 0x7F);
}

void StreamEvent::setFinalFlag(unsigned char flag) {
	finalFlag = (flag & 0x01);
}

int StreamEvent::setPrivateDataPayload(char* data, unsigned char length) {
	if (privateDataPayload != NULL) {
		delete (privateDataPayload);
	}
	try {
		privateDataPayload = new char[length];
	} catch(...) {
		return -1;
	}
	memcpy(privateDataPayload, data, length);
	privateDataPayloadLength = length;
	privateDataLength = privateDataPayloadLength + 3;
	descriptorLength = privateDataLength + 13;
	return privateDataPayloadLength;
}

void StreamEvent::setFcs(unsigned char fcs) {
	this->fcs = fcs;
}

}
}
}
}
}
