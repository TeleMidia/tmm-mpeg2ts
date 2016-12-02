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
 * TransportProtocol.cpp
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/TransportProtocol.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

TransportProtocol::TransportProtocol() : MpegDescriptor(0x02) {
	ocProtocol = NULL;
}

TransportProtocol::~TransportProtocol() {
	if (ocProtocol) delete ocProtocol;
}

int TransportProtocol::process() {
	int pos = MpegDescriptor::process();

	protocolId = (((stream[pos] & 0x0F) << 8) |
			       (stream[pos+1] & 0xFF));
	pos += 2;

	transportProtocolLabel = stream[pos++] & 0xFF;

	switch (protocolId) {
	case P_OBJECT_CAROUSEL:
		ocProtocol = new ObjectCarouselProtocol();
		ocProtocol->remoteConnection = (stream[pos++] & 0x80) > 0;
		if (ocProtocol->remoteConnection) {
			ocProtocol->originalNetworkId = (((stream[pos] & 0x0F) << 8) |
					   	   	   	   	   	   	  (stream[pos+1] & 0xFF));
			pos += 2;
			ocProtocol->transportStreamId = (((stream[pos] & 0x0F) << 8) |
											  (stream[pos+1] & 0xFF));
			pos += 2;
			ocProtocol->serviceId = (((stream[pos] & 0x0F) << 8) |
									  (stream[pos+1] & 0xFF));
			pos += 2;
		}
		ocProtocol->componentTag = stream[pos++] & 0xFF;
		break;
	default:
		cout << "Unrecognized protocol." << endl;
		break;
	}

	return pos;
}

int TransportProtocol::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = (protocolId >> 8) & 0xFF;
	stream[pos++] = protocolId & 0xFF;
	stream[pos++] = transportProtocolLabel & 0xFF;

	switch (protocolId) {
	case P_OBJECT_CAROUSEL:
		if (ocProtocol) {
			if (ocProtocol->remoteConnection) {
				stream[pos++] = 0xFF;
				stream[pos] = (ocProtocol->originalNetworkId >> 8) & 0xFF;
				pos++;
				stream[pos++] = ocProtocol->originalNetworkId & 0xFF;
				stream[pos] = (ocProtocol->transportStreamId >> 8) & 0xFF;
				pos++;
				stream[pos++] = ocProtocol->transportStreamId & 0xFF;
				stream[pos] = (ocProtocol->serviceId >> 8) & 0xFF;
				pos++;
				stream[pos++] = ocProtocol->serviceId & 0xFF;
			} else {
				stream[pos++] = 0x7F;
			}
			stream[pos++] = ocProtocol->componentTag & 0xFF;

		} else {
			cout << "Object carousel protocol not initialized." << endl;
			return -1;
		}
		break;
	default:
		cout << "Unrecognized protocol." << endl;
		break;
	}

	return pos;
}

unsigned int TransportProtocol::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 3;
	switch (protocolId) {
	case P_OBJECT_CAROUSEL:
		if (ocProtocol) {
			if (ocProtocol->remoteConnection) pos += 6;
			pos += 2;
		} else {
			cout << "Object carousel protocol not initialized." << endl;
			return -1;
		}
		break;
	default:
		cout << "Unrecognized protocol." << endl;
		break;
	}

	return pos;
}

void TransportProtocol::setProtocolId(unsigned short id) {
	protocolId = id;
}

unsigned short TransportProtocol::getProtocolId() {
	return protocolId;
}

void TransportProtocol::setTransportProtocolLabel(unsigned char pl) {
	transportProtocolLabel = pl;
}

unsigned char TransportProtocol::getTransportProtocolLabel() {
	return transportProtocolLabel;
}

void TransportProtocol::setOcProtocol(ObjectCarouselProtocol* ocp) {
	if (ocProtocol) delete ocProtocol;
	ocProtocol = ocp;
}

ObjectCarouselProtocol* TransportProtocol::getOcProtocol() {
	return ocProtocol;
}

}
}
}
}


