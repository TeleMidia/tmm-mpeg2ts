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

#include "PESPacket.h"

#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

	PESPacket::PESPacket(char* dataStream, unsigned char length) {
		ptsDtsFlags = 0;
		startCodePrefix = 0;
		pts = 0;
		dts = 0;
		escrBase = 0;
		escrExtension = 0;
		esRate = 0;
		currentSize = 0;
		stream = NULL;
		process(dataStream, length);
	}

	PESPacket::PESPacket() {

	}

	PESPacket::~PESPacket() {
		releaseBufferList();
	}

	void PESPacket::releaseBufferList() {
		vector<Buffer*>::iterator it;

		it = bufferList.begin();
		while (it != bufferList.end()) {
			if (*it) {
				delete ((*it)->buffer);
				delete (*it);
			}
			++it;
		}
		bufferList.clear();
		if (stream) delete stream;
		stream = NULL;
		currentSize = 0;
	}

	void PESPacket::process(char* dataStream, unsigned char length) {
		Buffer* buf = new Buffer();
		buf->buffer = new char[length];
		buf->size = length;
		memcpy(buf->buffer, dataStream, length);

		bufferList.push_back(buf);

		if (currentSize == 0) {
			processHeader(dataStream, length);
		}

		currentSize += length;
	}

	void PESPacket::processHeader(char* dataStream, unsigned char length) {
		unsigned char value;
		unsigned char pos = 0;
		bool continueDecode = true;

		startCodePrefix = (((dataStream[pos] & 0xFF) << 16) |
					((dataStream[pos + 1] & 0xFF) << 8) |
					(dataStream[pos + 2] & 0xFF));

		if (startCodePrefix != 0x01) {
			return;
		}

		pos = pos + 3;
		streamId = (dataStream[pos] & 0xFF);

		if (streamId == 0xBC) {
			//cout << "PS Program Map Table" << endl;
			continueDecode = false;
		}
		if (streamId == 0xBE) {
			//cout << "padding_stream" << endl;
			continueDecode = false;
		}
		if (streamId == 0xBF) {
			//cout << "private_stream_2" << endl;
			continueDecode = false;
		}
		if (streamId == 0xF0) {
			//cout << "ECM private" << endl;
			continueDecode = false;
		}
		if (streamId == 0xF1) {
			//cout << "EMM private" << endl;
			continueDecode = false;
		}
		if (streamId == 0xF2) {
			//cout << "DSMCC_stream" << endl;
			continueDecode = false;
		}
		if (streamId == 0xF8) {
			//cout << "ITU-T Rec. H.222.1 type E stream" << endl;
			continueDecode = false;
		}

		packetLength = (((dataStream[pos + 1] & 0xFF) << 8) |
				(dataStream[pos + 2] & 0xFF));

		pos = pos + 3;

		if (!continueDecode) return;

		value = ((dataStream[pos] & 0xC0) >> 6); //pos = 6
		if (value != 2) {
			cout << "error: invalid PES!" << endl;
			return;
		}

		scramblingControl = ((dataStream[pos] & 0x30) >> 4); //pos = 6
		priority = ((dataStream[pos] & 0x08) >> 3);
		dataAlignmentIndicator = ((dataStream[pos] & 0x04) >> 2);
		copyright = ((dataStream[pos] & 0x02) >> 1);
		original = (dataStream[pos] & 0x01);
		ptsDtsFlags = ((dataStream[pos + 1] & 0xC0) >> 6); // pos = 7
		escrFlag = ((dataStream[pos] & 0x20) >> 5);
		esRateFlag = ((dataStream[pos] & 0x10) >> 4);
		dsmTrickModeFlag = ((dataStream[pos] & 0x08) >> 3);
		additionalCopyInfoFlag = ((dataStream[pos] & 0x04) >> 2);
		crcFlag = ((dataStream[pos] & 0x02) >> 1);
		extensionFlag = (dataStream[pos] & 0x01);
		headerDataLength = (dataStream[pos + 2] & 0xFF); // pos = 8

		pos = pos + 3;
		if (ptsDtsFlags == 2 || ptsDtsFlags == 3) {
			value = ((dataStream[pos] & 0xF0) >> 4); // pos = 9
			if (ptsDtsFlags == 2 && value != 2) {
				cout << "error: invalid PTS initiate stream!" << endl;
				return;
			}

			if (ptsDtsFlags == 3 && value != 3) {
				cout << "error: invalid PTS_DTS initiate stream!" << endl;
				return;
			}

			pts = (dataStream[pos] & 0x0E) << 15;
			pts = pts << 14;

			value = (dataStream[pos] & 0x01);
			if (value != 1) {
				cout << "error: invalid PTS value! (marker_bit 1)" << endl;
				return;
			}

			pts = pts | ((dataStream[pos + 1] & 0xFF) << 22);
			pts = pts | ((dataStream[pos + 2] & 0xFE) << 14);

			value = (dataStream[pos + 2] & 0x01);
			if (value != 1) {
				cout << "error: invalid PTS value! (marker_bit 2)" << endl;
				return;
			}

			pos = pos + 3;
			pts = pts | ((dataStream[pos    ] & 0xFF) << 7);
			pts = pts | ((dataStream[pos + 1] & 0xFE) >> 1);

			value = (dataStream[pos + 1] & 0x01); //pos = 13
			if (value != 1) {
				cout << "error: invalid PTS value! (marker_bit 3)" << endl;
				return;
			}

			pos = pos + 2;

			if (ptsDtsFlags == 3) {
				/* DTS */
				value = ((dataStream[pos] & 0xF0) >> 4); //pos = 14
				if (value != 1) {
					cout << "error: invalid PTS and DTS value!" << endl;
					return;
				}

				dts = (dataStream[pos] & 0x0E) << 15;
				dts = dts << 14;

				value = (dataStream[pos] & 0x01);
				if (value != 1) {
					cout << "error: invalid PTS and DTS value! (marker_bit 4)";
					cout << endl;
					return;
				}

				dts = dts | ((dataStream[pos + 1] & 0xFF) << 22);
				dts = dts | ((dataStream[pos + 2] & 0xFE) << 14);

				pos = pos + 2;
				value = (dataStream[pos] & 0x01);
				if (value != 1) {
					cout << "error: invalid PTS and DTS value! (marker_bit 5)";
					cout << endl;
					return;
				}

				dts = dts | ((dataStream[pos + 1] & 0xFF) << 7);
				dts = dts | ((dataStream[pos + 2] & 0xFE) >> 1);

				pos = pos + 2;
				value = (dataStream[pos] & 0x01); //pos = 18
				if (value != 1) {
					cout << "error: invalid PTS and DTS value! (marker_bit 6)";
					cout << endl;
					return;
				}
			}
		}

		if (escrFlag == 1) {
			pos++;
			escrBase = escrBase | ((dataStream[pos] & 0x38) << 27);

			value = (dataStream[pos] & 0x04);
			if (value != 4) {
				cout << "error: invalid ESCR value! (marker_bit 1)" << endl;
				return;
			}

			escrBase = escrBase | ((dataStream[pos] & 0x03) << 28);
			pos++;
			escrBase = escrBase | ((dataStream[pos] & 0xFF) << 20);
			pos++;
			escrBase = escrBase | ((dataStream[pos] & 0xF8) << 15);

			value = (dataStream[pos] & 0x04);
			if (value != 4) {
				cout << "error: invalid ESCR value! (marker_bit 2)" << endl;
				return;
			}

			escrBase = escrBase | ((dataStream[pos] & 0x03) << 13);
			pos++;
			escrBase = escrBase | ((dataStream[pos] & 0xFF) << 5);
			pos++;
			escrBase = escrBase | ((dataStream[pos] & 0xF8) >> 3);

			value = (dataStream[pos] & 0x04);
			if (value != 4) {
				cout << "error: invalid ESCR value! (marker_bit 3)" << endl;
				return;
			}

			escrExtension = escrExtension | ((dataStream[pos] & 0x03) << 7);
			pos++;
			escrExtension = escrExtension | ((dataStream[pos] & 0xFE) >> 1);

			value = (dataStream[pos] & 0x01);
			if (value != 1) {
				cout << "error: invalid ESCR value! (marker_bit 4)" << endl;
				return;
			}

			escrBase = ((escrBase << 31) >> 31);
			escrExtension = ((escrExtension << 55) >> 55);
		}

		if (esRateFlag == 1) {
			pos++;
			value = (dataStream[pos] & 0x80);
			if (value != 128) {
				cout << "error: invalid ES rate value! (marker_bit 1)" << endl;
				return;
			}

			esRate = esRate | ((dataStream[pos] & 0x7F) << 15);
			pos++;
			esRate = esRate | ((dataStream[pos] & 0xFF) << 7);
			pos++;
			esRate = esRate | ((dataStream[pos] & 0xFE) >> 1); //pos(max) = 27

			value = (dataStream[pos] & 0x01);
			if (value != 1) {
				cout << "error: invalid ES rate value! (marker_bit 2)" << endl;
				return;
			}

			esRate = ((esRate << 10) >> 10);
		}

		/* TODO: Complete header structure */
	}

	bool PESPacket::isConsolidated() {
		return ((packetLength + 6) == currentSize);
	}

	void PESPacket::mountStream() {
		vector<Buffer*>::iterator it;
		unsigned int pos = 0;

		if (stream) delete stream;
		stream = new char[currentSize];

		it = bufferList.begin();
		while (it != bufferList.end()) {
			if (*it) {
				memcpy(stream + pos, (*it)->buffer, (*it)->size);
				pos += (*it)->size;
			}
			++it;
		}
	}

	int PESPacket::getStream(char* dataStream) {
		mountStream();
		memcpy(dataStream, stream, currentSize);
		return currentSize;
	}

	int PESPacket::getStream(char** dataStream) {
		mountStream();
		*dataStream = stream;
		return currentSize;
	}

	unsigned int PESPacket::getStartCodePrefix() {
		return startCodePrefix;
	}

	unsigned int PESPacket::getStreamId() {
		return streamId;
	}

	unsigned short PESPacket::getPacketLength() {
		return packetLength;
	}

	unsigned short PESPacket::getScramblingControl() {
		return scramblingControl;
	}

	bool PESPacket::getPriority() {
		return (priority == 1);
	}

	bool PESPacket::getDataAlignmentIndicator() {
		return (dataAlignmentIndicator == 1);
	}

	bool PESPacket::getCopyright() {
		return (copyright == 1);
	}

	bool PESPacket::getOriginal() {
		return (original == 1);
	}

	unsigned char PESPacket::getPtsDtsFlags() {
		return ptsDtsFlags;
	}

	bool PESPacket::getEscrFlag() {
		return (escrFlag == 1);
	}

	bool PESPacket::getEsRateFlag() {
		return (esRateFlag == 1);
	}

	bool PESPacket::getDsmTrickModeFlag() {
		return (dsmTrickModeFlag == 1);
	}

	bool PESPacket::getAdditionalCopyInfoFlag() {
		return (additionalCopyInfoFlag == 1);
	}

	bool PESPacket::getCrcFlag() {
		return (crcFlag == 1);
	}

	bool PESPacket::getExtensionFlag() {
		return (extensionFlag == 1);
	}

	unsigned char PESPacket::getHeaderDataLength() {
		return headerDataLength;
	}

	uint64_t PESPacket::getPts() {
		return pts;
	}

	void PESPacket::setPts(uint64_t pts) {
		this->pts = pts;
	}

	uint64_t PESPacket::getDts() {
		return dts;
	}

	bool PESPacket::setPtsDirectStream(char* stream, uint64_t pts) {
		unsigned char ptsDtsFlags = ((stream[7] & 0xC0) >> 6);
		if ((ptsDtsFlags == 2) || (ptsDtsFlags == 3)) {
			if (ptsDtsFlags == 2) stream[9] = 0x21; else stream[9] = 0x31;
			stream[9] = stream[9] | (((pts >> 15) >> 14) & 0x0E);
			stream[10] = (pts >> 22) & 0xFF;
			stream[11] = 0x01;
			stream[11] = stream[11] | ((pts >> 14) & 0xFE);
			stream[12] = (pts >> 7) & 0xFF;
			stream[13] = 0x01;
			stream[13] = stream[13] | ((pts << 1) & 0xFE);
			return true;
		} else {
			return false;
		}
	}

	bool PESPacket::setDtsDirectStream(char* stream, uint64_t dts) {
		unsigned char ptsDtsFlags = ((stream[7] & 0xC0) >> 6);
		if (ptsDtsFlags == 3) {
			stream[14] = 0x11;
			stream[14] = stream[14] | (((dts >> 15) >> 14) & 0x0E);
			stream[15] = (dts >> 22) & 0xFF;
			stream[16] = 0x01;
			stream[16] = stream[16] | ((dts >> 14) & 0xFE);
			stream[17] = (dts >> 7) & 0xFF;
			stream[18] = 0x01;
			stream[18] = stream[18] | ((dts << 1) & 0xFE);
			return true;
		} else {
			return false;
		}
	}

	int64_t PESPacket::getPtsDirectStream(char* stream) {
		uint64_t pts;
		unsigned char value;
		int pos = 9;
		unsigned char ptsDtsFlags = ((stream[7] & 0xC0) >> 6);

		if ((ptsDtsFlags == 2) || (ptsDtsFlags == 3)) {
			pts = (stream[pos] & 0x0E) << 15;
			pts = pts << 14;

			value = (stream[pos] & 0x01);
			if (value != 1) {
				return -1;
			}

			pts = pts | ((stream[pos + 1] & 0xFF) << 22);
			pts = pts | ((stream[pos + 2] & 0xFE) << 14);

			value = (stream[pos + 2] & 0x01);
			if (value != 1) {
				return -2;
			}

			pos = pos + 3;
			pts = pts | ((stream[pos    ] & 0xFF) << 7);
			pts = pts | ((stream[pos + 1] & 0xFE) >> 1);
			return pts;
		}
		return -3;
	}

	int64_t PESPacket::getDtsDirectStream(char* stream) {
		uint64_t dts;
		unsigned char value;
		int pos = 14;
		unsigned char ptsDtsFlags = ((stream[7] & 0xC0) >> 6);

		if (ptsDtsFlags == 3) {
			dts = (stream[pos] & 0x0E) << 15;
			dts = dts << 14;

			value = (stream[pos] & 0x01);
			if (value != 1) {
				cout << endl;
				return -1;
			}

			dts = dts | ((stream[pos + 1] & 0xFF) << 22);
			dts = dts | ((stream[pos + 2] & 0xFE) << 14);

			pos = pos + 2;
			value = (stream[pos] & 0x01);
			if (value != 1) {
				cout << endl;
				return -2;
			}

			dts = dts | ((stream[pos + 1] & 0xFF) << 7);
			dts = dts | ((stream[pos + 2] & 0xFE) >> 1);
			return dts;
		}
		return -3;
	}

	bool PESPacket::setDataAlignmentIndicatorDirectStream(char* stream, bool ind) {
		stream[6] = (stream[6] & 0xFB);
		if (ind) {
			stream[6] = stream[6] | 0x04;
		}
		return true;
	}

	bool PESPacket::setStreamIdDirectStream(char* stream, unsigned char id) {
		stream[3] = id;
		return true;
	}

	uint64_t PESPacket::getEscrBase() {
		return escrBase;
	}

	uint64_t PESPacket::getEscrExtension() {
		return escrExtension;
	}

	unsigned int PESPacket::getEsRate() {
		return esRate * 50;
	}

	void PESPacket::print() {
		cout << "PESPacket::print" << endl;
		cout << "Stream ID: " << hex << (streamId & 0xFF);
		if ((streamId >= 0xC0) && (streamId <= 0xDF)) {
			cout << " (MPEG1 or MPEG2 Audio)";
		}
		if ((streamId >= 0xE0) && (streamId <= 0xEF)) {
			cout << " (MPEG1 or MPEG2 Video)";
		}
		cout << endl << "PTS DTS flag: " << (ptsDtsFlags & 0xFF) << endl;
		cout << "CRC flag: " << (crcFlag & 0xFF) << endl;
		if (ptsDtsFlags == 2 || ptsDtsFlags == 3) {
			cout << "PTS: " << dec << pts << endl;
		}
		if (ptsDtsFlags == 3) {
			cout << "DTS: " << dec << dts << endl;
		}
	}

}
}
}
}
