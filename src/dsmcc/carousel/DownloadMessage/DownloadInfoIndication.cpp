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
 * DownloadInfoIndication.cpp
 *
 *  Created on: Jun 25, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/DownloadMessage/DownloadInfoIndication.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	DownloadInfoIndication::DownloadInfoIndication() : MessageHeader() {
		protocolDiscriminator = 0x11;
		dsmccType = 0x03;
		messageId = 0x1002;
		extensionId = 0x80000002;
		moduleInfoList = NULL;
		privateDataByte = NULL;
		privateDataLength = 0;
		blockSize = 0;
		windowSize = 0;
		ackPeriod = 0;
		tCDownloadWindow = 0;
		tCDownloadScenario = 0;
		numberOfModules = 0;
		downloadId = 0;
	}

	DownloadInfoIndication::~DownloadInfoIndication() {
		if (moduleInfoList != NULL) {
			clearModuleInfoList();
			delete (moduleInfoList);
		}
		if (privateDataByte != NULL) {
			delete (privateDataByte);
		}
	}

	int DownloadInfoIndication::updateStream() {
		int pos = MessageHeader::updateStream();
		char* tempStr;
		int tempStrLen;

		stream[pos++] = (downloadId >> 24) & 0xFF;
		stream[pos++] = (downloadId >> 16) & 0xFF;
		stream[pos++] = (downloadId >> 8) & 0xFF;
		stream[pos++] = downloadId & 0xFF;
		stream[pos++] = (blockSize >> 8) & 0xFF;
		stream[pos++] = blockSize & 0xFF;
		stream[pos++] = windowSize & 0xFF;
		stream[pos++] = ackPeriod & 0xFF;
		stream[pos++] = (tCDownloadWindow >> 24) & 0xFF;
		stream[pos++] = (tCDownloadWindow >> 16) & 0xFF;
		stream[pos++] = (tCDownloadWindow >> 8) & 0xFF;
		stream[pos++] = tCDownloadWindow & 0xFF;
		stream[pos++] = (tCDownloadScenario >> 24) & 0xFF;
		stream[pos++] = (tCDownloadScenario >> 16) & 0xFF;
		stream[pos++] = (tCDownloadScenario >> 8) & 0xFF;
		stream[pos++] = tCDownloadScenario & 0xFF;

		//compatibilityDescriptorLength
		stream[pos++] = 0x00;
		stream[pos++] = 0x00;

		stream[pos++] = (numberOfModules >> 8) & 0xFF;
		stream[pos++] = numberOfModules & 0xFF;

		ModuleInfo* mi;
		vector<ModuleInfo*>::iterator miIt;
		if ((moduleInfoList != NULL) && (!moduleInfoList->empty())) {
			miIt = moduleInfoList->begin();
			while (miIt != moduleInfoList->end()) {
				mi = *miIt;
				tempStrLen = mi->getStream(&tempStr);
				memcpy(stream + pos, tempStr, tempStrLen);
				pos = pos + tempStrLen;
				++miIt;
			}
		}

		stream[pos++] = (privateDataLength >> 8) & 0xFF;
		stream[pos++] = privateDataLength & 0xFF;

		if (privateDataByte != NULL) {
			memcpy(stream + pos, privateDataByte, privateDataLength);
			pos = pos + privateDataLength;
		}

		return pos;
	}

	unsigned int DownloadInfoIndication::calculateStreamSize() {
		unsigned int pos = MessageHeader::calculateStreamSize();
		ModuleInfo* mi;
		vector<ModuleInfo*>::iterator miIt;
		if ((moduleInfoList != NULL) && (!moduleInfoList->empty())) {
			miIt = moduleInfoList->begin();
			while (miIt != moduleInfoList->end()) {
				mi = *miIt;
				pos += mi->getStreamLength();
				++miIt;
			}
		}
		pos += privateDataLength;
		return pos + 22;
	}

	unsigned int DownloadInfoIndication::getDownloadId() {
		return downloadId;
	}

	unsigned short DownloadInfoIndication::getBlockSize() {
		return blockSize;
	}

	unsigned char DownloadInfoIndication::getWindowSize() {
		return windowSize;
	}

	unsigned char DownloadInfoIndication::getAckPeriod() {
		return ackPeriod;
	}

	unsigned int DownloadInfoIndication::getTCDownloadWindow() {
		return tCDownloadWindow;
	}

	unsigned int DownloadInfoIndication::getTCDownloadScenario() {
		return tCDownloadScenario;
	}

	unsigned short DownloadInfoIndication::getNumberOfModules() {
		return numberOfModules;
	}

	vector<ModuleInfo*>* DownloadInfoIndication::getModuleInfoList() {
		if (moduleInfoList != NULL) {
			return moduleInfoList;
		}
		return NULL;
	}

	unsigned short DownloadInfoIndication::getPrivateDataLength() {
		return privateDataLength;
	}

	int DownloadInfoIndication::getPrivateDataByte(char** dataStream) {
		if (privateDataByte != NULL) {
			*dataStream = privateDataByte;
			return privateDataLength;
		} else {
			return 0;
		}
	}

	void DownloadInfoIndication::setDownloadId(unsigned int id) {
		downloadId = id;
	}

	void DownloadInfoIndication::setBlockSize(unsigned short size) {
		blockSize = size;
	}

	void DownloadInfoIndication::setWindowSize(unsigned char size) {
		windowSize = size;
	}

	void DownloadInfoIndication::setAckPeriod(unsigned char period) {
		ackPeriod = period;
	}

	void DownloadInfoIndication::setTCDownloadWindow(unsigned int dw) {
		tCDownloadWindow = dw;
	}

	void DownloadInfoIndication::setTCDownloadScenario(unsigned int dc) {
		tCDownloadScenario = dc;
	}

	void DownloadInfoIndication::setNumberOfModules(unsigned short num) {
		numberOfModules = num;
	}

	void DownloadInfoIndication::setModuleInfoList(vector<ModuleInfo*>* mil) {
		if (moduleInfoList != NULL) {
			delete (moduleInfoList);
		}
		moduleInfoList = mil;
		numberOfModules = moduleInfoList->size();
		messageLength = calculateStreamSize() - 12;
	}

	int DownloadInfoIndication::setPrivateDataByte(char* data, int length) {
		if (privateDataByte != NULL) {
			delete (privateDataByte);
		}
		privateDataLength = length;
		messageLength = calculateStreamSize() - 12;
		try {
			privateDataByte = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(privateDataByte, data, length);
		return 0;
	}

	void DownloadInfoIndication::clearModuleInfoList() {
		ModuleInfo* mi;
		vector<ModuleInfo*>::iterator i;
		if ((moduleInfoList != NULL) && (!moduleInfoList->empty())) {
			i = moduleInfoList->begin();
			while (i != moduleInfoList->end()) {
				mi = *i;
				delete (mi);
				++i;
			}
			moduleInfoList->clear();
		}
	}

}
}
}
}
}
