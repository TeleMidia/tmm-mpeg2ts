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

#include "TSPacket.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

	TSPacket::TSPacket(char* packet) {
		stream = new char[TS_PACKET_SIZE_204];
		payload = new char[TS_PAYLOAD_SIZE];
		payload2 = new char[TS_PAYLOAD_SIZE];
		transportErrorIndication = 0;
		transportScramblingControl = 0;
		sectionType = 1;
		payloadSize = 0;
		tsaf = NULL;
		memcpy(stream, packet, TS_PACKET_SIZE);
		processHeader();
	}

	TSPacket::TSPacket() {
		stream = new char[TS_PACKET_SIZE_204];
		payload = new char[TS_PAYLOAD_SIZE];
		payload2 = new char[TS_PAYLOAD_SIZE];
		tsaf = NULL;
		sectionType = 1;
		transportErrorIndication = 0;
		transportScramblingControl = 0;
	}

	/*
	 * function: TSPacket()
	 * You can specify a fixed payload size, even if it is not used completely
	 * by the packet. However you should use the GetStream() function to know
	 * the payload size actually used.
	 */
	TSPacket::TSPacket(char sectionType, char* payload,
			unsigned char payloadSize,
				TSAdaptationField* tsaf) {

		stream = new char[TS_PACKET_SIZE_204];
		this->payload = new char[TS_PAYLOAD_SIZE];
		payload2 = new char[TS_PAYLOAD_SIZE];

		stream[0] = 0;

		if (tsaf != NULL) {
			if (payloadSize) {
				adaptationFieldControl = 3;
			} else {
				adaptationFieldControl = 2;
			}
		} else if (payloadSize) {
			adaptationFieldControl = 1;
		} else {
			adaptationFieldControl = 0;
		}
		transportErrorIndication = 0;
		transportPriority = 0;
		transportScramblingControl = 0;
		this->tsaf = tsaf;
		this->sectionType = sectionType;
		this->payloadSize = payloadSize;
		memcpy(this->payload, payload, payloadSize);
	}

	TSPacket::~TSPacket() {
		if (tsaf != NULL) {
			delete tsaf;
		}
		if (stream != NULL) {
			delete stream;
		}
		if (payload != NULL) {
			delete payload;
		}
		if (payload2 != NULL) {
			delete payload2;
		}
	}

	bool TSPacket::getSectionType() {
		return sectionType;
	}

	void TSPacket::setSectionType(char type) {
		sectionType = type;
	}

	int TSPacket::processHeader() {
		syncByte = (stream[0] & 0xFF);
		if (syncByte != TS_PACKET_SYNC_BYTE) {
			cout << "TSPacket::process warning! syncByte != 0x47" << endl;
			return -1;
		}

		transportErrorIndication = (stream[1] & 0x80) >> 7;
		payloadUnitStartIndicator = (stream[1] & 0x40) >> 6;
		transportPriority = (stream[1] & 0x20) >> 5;
		pid = (((stream[1] & 0x1F) << 8) | (stream[2] & 0xFF));

		transportScramblingControl = (stream[3] & 0xC0) >> 6;
		adaptationFieldControl = (stream[3] & 0x30) >> 4;
		continuityCounter = (stream[3] & 0x0F);
		return 0;
	}

	int TSPacket::process() {
		unsigned char pointerField, pointerFieldPos, payloadOffset = 4;
		unsigned int pesStartCode;

		syncByte = (stream[0] & 0xFF);
		if (syncByte != TS_PACKET_SYNC_BYTE) {
			cout << "TSPacket::process warning! syncByte != 0x47" << endl;
			return -1;
		}

		if (adaptationFieldControl == ADAPT_PAYLOAD) {
			unsigned char aflen = stream[4] & 0xFF;
			payloadOffset += (aflen + 1);
		}

		pesStartCode = (((stream[payloadOffset] & 0xFF) << 16) |
						((stream[payloadOffset + 1] & 0xFF) << 8) |
						(stream[payloadOffset + 2] & 0xFF));

		if (pid != 0x1FF0) {
			sectionType = !((pesStartCode == 0x01) && (pid != 0x00));
		} else sectionType = false;

		if (adaptationFieldControl == PAYLOAD_ONLY) {
			if (sectionType) {
				if (payloadUnitStartIndicator) {
					pointerField = (stream[4] & 0xFF);
					payloadSize = TS_PAYLOAD_SIZE - pointerField - 1;
					memcpy(payload, stream + pointerField + 5, payloadSize);
					payloadSize2 = TS_PAYLOAD_SIZE - payloadSize - 1;
					memcpy(payload2, stream + 5, payloadSize2);
					if ((payload[0] & 0xFF) == 0xFF) {
						payloadSize = 0;
					}
				} else {
					payloadSize = TS_PAYLOAD_SIZE;
					memcpy(payload, stream + 4, payloadSize);
					payloadSize2 = 0;
				}
			} else {
				payloadSize = TS_PAYLOAD_SIZE;
				memcpy(payload, stream + 4, payloadSize);
				payloadSize2 = 0;
			}

		} else if (adaptationFieldControl == ADAPT_PAYLOAD) {
			if (tsaf != NULL) {
				delete tsaf;
			}

			tsaf = new TSAdaptationField(stream + 4);

			if (sectionType) {
				if (payloadUnitStartIndicator) {
					pointerFieldPos = tsaf->getAdaptationFieldLength() + 5;
					pointerField = stream[pointerFieldPos] & 0xFF;
					payloadSize = TS_PACKET_SIZE - pointerField -
						pointerFieldPos - 1;
					if (payloadSize > 184) payloadSize = 184 - //Check this!!!
							tsaf->getAdaptationFieldLength() - 1;

					memcpy(payload, stream + pointerField + pointerFieldPos + 1,
						payloadSize);

					payloadSize2 = pointerField;
					memcpy(payload2, stream + pointerFieldPos + 1,
							payloadSize2);

					if ((payload[0] & 0xFF) == 0xFF) {
						payloadSize = 0;
					}
				} else {
					payloadSize = TS_PACKET_SIZE -
						(tsaf->getAdaptationFieldLength() + 5);
					memcpy(payload,
						stream + tsaf->getAdaptationFieldLength() + 5,
						payloadSize);
					payloadSize2 = 0;
				}
			} else {
				payloadSize = TS_PACKET_SIZE -
					(tsaf->getAdaptationFieldLength() + 5);
				memcpy(payload,
					stream + tsaf->getAdaptationFieldLength() + 5,
					payloadSize);
				payloadSize2 = 0;
			}

		} else if (adaptationFieldControl == NO_PAYLOAD) {
			payloadSize = 0;
			payloadSize2 = 0;
			if (tsaf != NULL) {
				delete tsaf;
			}
			tsaf = new TSAdaptationField(stream + 4);
		}
		return 0;
	}

	unsigned short TSPacket::getPid() {
		return pid;
	}

	void TSPacket::setPid(unsigned short pid) {
		this->pid = pid;
	}

	int TSPacket::getPayload(char** dataStream) {
		*dataStream = payload;
		return payloadSize;
	}

	unsigned int TSPacket::getPayloadSize() {
		return payloadSize;
	}

	int TSPacket::getPayload2(char** dataStream) {
		*dataStream = payload2;
		return payloadSize2;
	}

	unsigned int TSPacket::getPayloadSize2() {
		return payloadSize2;
	}

	unsigned char TSPacket::getNumberOfPayloads() {
		if (((payloadSize) && (!payloadSize2)) ||
				((!payloadSize) && (payloadSize2))) {
			return 1;
		} else if ((payloadSize) && (payloadSize2)) {
			return 2;
		} else {
			return 0;
		}
	}

	char TSPacket::getStartIndicator() {
		return payloadUnitStartIndicator;
	}

	void TSPacket::setStartIndicator(char flag) {
		payloadUnitStartIndicator = flag;
	}

	unsigned char TSPacket::getAdaptationFieldControl() {
		return adaptationFieldControl;
	}

	void TSPacket::setAdaptationFieldControl(unsigned char afc) {
		adaptationFieldControl = afc;
	}

	unsigned char TSPacket::getContinuityCounter() {
		return continuityCounter;
	}

	void TSPacket::setContinuityCounter(unsigned char counter) {
		continuityCounter = counter;
	}

	/*
	 * function: updateStream()
	 * Returns the payload length used. Because the payload could
	 * exceed the TS packet capacity when this class is created manually.
	 */
	int TSPacket::updateStream() { //encode stream
		char value;
		char afbuffer[MAX_ADAPTATION_FIELD_SIZE];
		unsigned int len, plen;

		if (adaptationFieldControl == 0) {
			return -1;
		}

		memset(stream, 0xFF, 188);

		stream[0] = 0x47;
		value = transportErrorIndication;
		stream[1] = stream[1] & 0x7F;
		stream[1] = stream[1] | ((value << 7) & 0x80);
		stream[1] = stream[1] & 0xBF;
		value = payloadUnitStartIndicator;
		stream[1] = stream[1] | ((value << 6) & 0x40);
		stream[1] = stream[1] & 0xDF;
		value = transportPriority;
		stream[1] = stream[1] | ((value << 5) & 0x20);
		stream[1] = stream[1] & 0xE0;
		stream[1] = stream[1] | ((pid & 0x1F00) >> 8);
		stream[2] = pid & 0x00FF;
		stream[3] = stream[3] & 0x3F;
		stream[3] = stream[3] | ((transportScramblingControl << 6) & 0xC0);
		stream[3] = stream[3] & 0xCF;
		stream[3] = stream[3] | ((adaptationFieldControl << 4) & 0x30);
		stream[3] = stream[3] & 0xF0;
		stream[3] = stream[3] | (continuityCounter & 0x0F);

		if (sectionType && payloadUnitStartIndicator) {
			if (adaptationFieldControl == 1) {
				//payload only
				len = payloadSize;
				if (payloadSize > 183) len = 183;
				stream[4] = 0;
				memcpy(stream + 5, payload, len);
				return len;
			} else if (adaptationFieldControl == 2) {
				//adaptation field only
				if (tsaf != NULL) {
					tsaf->setAdaptationFieldLength(183);
					len = tsaf->getStream(afbuffer);
					memcpy(stream + 4, afbuffer, len);
					return 0;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			} else if (adaptationFieldControl == 3) {
				//adaptation field followed by payload
				if (tsaf != NULL) {
					plen = TS_PAYLOAD_SIZE -
						(tsaf->getAdaptationFieldLength() + 2); //available
					len = tsaf->getStream(afbuffer);
					memcpy(stream + 4, afbuffer, len);
					stream[len + 4] = 0;
					if (plen > payloadSize) {
						plen = payloadSize;
					}
					memcpy(stream + len + 5, payload, plen);
					return plen;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			}
		} else if ((!sectionType) ||
				(sectionType && (!payloadUnitStartIndicator))) {
			if (adaptationFieldControl == 1) {
				//payload only
				memcpy(stream + 4, payload, payloadSize);
				return payloadSize;
			} else if (adaptationFieldControl == 2) {
				//adaptation field only
				if (tsaf != NULL) {
					tsaf->setAdaptationFieldLength(183);
					len = tsaf->getStream(afbuffer);
					memcpy(stream + 4, afbuffer, len);
					return 0;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			} else if (adaptationFieldControl == 3) {
				//adaptation field followed by payload
				if (tsaf != NULL) {
					plen = TS_PAYLOAD_SIZE -
						(tsaf->getAdaptationFieldLength() + 1);

					len = tsaf->getStream(afbuffer);
					memcpy(stream + 4, afbuffer, len);
					if (plen > payloadSize) {
						plen = payloadSize;
					}
					memcpy(stream + len + 4, payload, plen);
					return plen;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			}
		}
		return -1;
	}

	/* getStream() returns payload length used */
	int TSPacket::getStream(char** dataStream) {
		int len;
		len = updateStream();
		*dataStream = stream;
		return len;
	}

	int TSPacket::getPayloadLengthAfterUpdate() {
		unsigned int plen;
		if (sectionType && payloadUnitStartIndicator) {
			if (adaptationFieldControl == 1) {
				//payload only
				plen = payloadSize;
				if (payloadSize > 183) plen = 183;
				return plen;
			} else if (adaptationFieldControl == 2) {
				//adaptation field only
				if (tsaf == NULL) {
					cout << "Error: No adaptation field." << endl;
				}
			} else if (adaptationFieldControl == 3) {
				//adaptation field followed by payload
				if (tsaf != NULL) {
					plen = TS_PAYLOAD_SIZE -
						(tsaf->getAdaptationFieldLength() + 2); //available
					if (plen > payloadSize) {
						plen = payloadSize;
					}
					return plen;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			}
		} else if ((!sectionType) ||
				(sectionType && (!payloadUnitStartIndicator))) {
			if (adaptationFieldControl == 1) {
				//payload only
				return payloadSize;
			} else if (adaptationFieldControl == 2) {
				//adaptation field only
				if (tsaf != NULL) {
					return 0;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			} else if (adaptationFieldControl == 3) {
				//adaptation field followed by payload
				if (tsaf != NULL) {
					plen = TS_PAYLOAD_SIZE -
						(tsaf->getAdaptationFieldLength() + 1);
					if (plen > payloadSize) {
						plen = payloadSize;
					}
					return plen;
				} else {
					cout << "Error: No adaptation field." << endl;
				}
			}
		}
		return -1;
	}

	TSAdaptationField* TSPacket::getAdaptationField() {
		return tsaf;
	}

	void TSPacket::setAdaptationField(TSAdaptationField* tsaf) {
		this->tsaf = tsaf;
	}
}
}
}
}
