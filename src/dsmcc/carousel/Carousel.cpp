/*
 * Carousel.cpp
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Carousel.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	Carousel::Carousel() {
		moduleManager = new ModuleManager();
		blockSize = 4066;
		serviceDomain = 0x0A; // downloadId
		transactionId = 0x80090002;
		sectionEncapsulationMode = false;
		continuityCounter = 0;
		pid = 0x384;
		dsi = NULL;
		dii = NULL;
	}

	Carousel::~Carousel() {
		delete moduleManager;
		if (dsi != NULL) {
			delete dsi;
		}
		if (dii != NULL) {
			delete dii;
		}
	}

	void Carousel::setPid(unsigned short pid) {
		this->pid = pid;
	}

	unsigned short Carousel::getPid() {
		return pid;
	}

	void Carousel::setBitrate(int bitrate) {
		this->bitrate = bitrate;
	}

	int Carousel::getBitrate() {
		return bitrate;
	}

	int Carousel::makeDSI() {
		char buffer[20];
		char* srgIorStream;
		int len;

		if (dsi != NULL) {
			delete dsi;
		}

		memset(buffer, 0xFF, 20);
		dsi = new DownloadServerInitiate();
		dsi->setExtensionId(transactionId);
		dsi->setServerId(buffer, 20);

		len = moduleManager->getSrgIorStream(&srgIorStream);
		dsi->setPrivateDataByte(srgIorStream, len);
		delete srgIorStream;

		return 0;
	}

	int Carousel::makeDII() {
		map<unsigned short, Module*>* ml = NULL;
		map<unsigned short, Module*>::iterator i;
		vector<ModuleInfo*>* mil = NULL;
		ModuleInfo* mi = NULL;
		unsigned short maxbs;
		char buffer[21];

		memset(buffer, 0xFF, 8);
		memset(buffer + 8, 0x00, 4);
		buffer[12] = 0x01;
		buffer[13] = 0x00;
		buffer[14] = 0x00;
		buffer[15] = 0x00;
		buffer[16] = 0x17;
		buffer[17] = 0x00;
		buffer[18] = 0x40;
		buffer[19] = 0x00;
		buffer[20] = 0x00;

		if (dii != NULL) {
			delete dii;
		}

		ml = moduleManager->getModuleList();
		mil = new vector<ModuleInfo*>;
		dii = new DownloadInfoIndication();
		dii->setExtensionId(transactionId);
		dii->setDownloadId(serviceDomain);
		dii->setNumberOfModules(ml->size());

		i = ml->begin();
		while (i != ml->end()) {
			mi = new ModuleInfo();
			mi->setModuleId(i->first);
			mi->setModuleSize(i->second->getSize());
			mi->setModuleInfoByte(buffer, 21);
			mil->push_back(mi);
			++i;
		}
		dii->setModuleInfoList(mil);

		checkMinBlockSize();

		maxbs = 4066 - dii->getAdaptationLength();
		if (blockSize > maxbs) {
			blockSize = maxbs;
			cout << "Resizing BlockSize to " << blockSize <<
				". (Maximum value for this scenario)" << endl;
		}
		dii->setBlockSize(blockSize);

		return 0;
	}

	int Carousel::createCarousel(string outputFile, string tempFolder) {
		int result;
		string outputFolder = getPath(outputFile);

		if ((outputFolder[outputFolder.size() - 1] == '\\') ||
				(outputFolder[outputFolder.size() - 1] == '/')) {
			outputFolder.erase(outputFolder.size() - 1, 1);
		}
		if (isDir(outputFolder) != 1) {
			makeDir(outputFolder.c_str(), 0777);
			if (isDir(outputFolder) != 1) {
				cout << "Carousel::createCarousel: Invalid output folder." << endl;
				return -1;
			}
		}

		FileManager::removeFiles(tempFolder);
		remove(outputFile.c_str());

		if (!moduleManager->setTempFolder(tempFolder)) {
			cout << "Carousel::createCarousel: Invalid temporary folder." << endl;
			return -2;
		}
		if (!moduleManager->readServiceGateway(serviceGatewayFolder)) {
			cout << "Invalid service gateway folder." << endl;
			return -3;
		}

		moduleManager->setTransactionId(transactionId);

		result = moduleManager->makeModules(serviceDomain);
		if (result >= 0) {
			makeDSI();
			makeDII();
			if (encapsulateCarousel(outputFile, tempFolder) < 0) {
				cout << "Unable to encapsulate carousel." << endl;
				return -5;
			}
			return 0;
		}

		switch (result) {
		case -3:
			cout << "Cannot create FileMessage." << endl;
			break;
		case -4:
			cout << "Cannot add FileMessage into module." << endl;
			break;
		case -5:
			cout << "Cannot create DirectoryMessage." << endl;
			break;
		case -6:
			cout << "Cannot add DirectoryMessage into module." << endl;
			break;
		case -7:
			cout << "Cannot save modules into files." << endl;
			break;
		default:
			cout << "Weird error." << endl;
			break;
		}

		return -4;
	}

	int Carousel::encapsulateCarousel(string outputFile, string tempFolder) {
		char* buffer;
		char* blockBuffer;
		unsigned short blockNumber;
		unsigned int sn, lsn, streamLength, length, pos, fd, fdr, blockRd, t;
		DSMCCSection* section = NULL;
		vector<DSMCCSection*> sectionList;
		vector<DSMCCSection*> dsiDiiSectionList;
		map<unsigned short, Module*>* ml = NULL;
		map<unsigned short, Module*>::iterator i;
		DownloadDataBlock* ddb = NULL;
		MessageHeader* msg;
		string modFileName;
		long modSize;

		fd = getWriteFD(outputFile);
		if (fd < 0) {
			return -1;
		}

		continuityCounter = 0;

		for (t = 0; t < 2; t++) {
			if (t == 0) {
				msg = dsi;
			} else {
				msg = dii;
			}
			streamLength = msg->getStream(&buffer);
			pos = 0;
			sn = 0;
			lsn = streamLength / 4084;
			if (lsn > 0) {
				if ((streamLength % 4084) > 0) {
					lsn += 1;
				}
				lsn--;
			}
			while (pos < streamLength) {
				section = new DSMCCSection();
				section->setTableId(0x3B);
				section->setSectionNumber(sn);
				section->setLastSectionNumber(lsn);
				section->setCurrentNextIndicator(1);
				section->setTableIdExtension((msg->getExtensionId() & 0xFFFF));
				sn++;
				if ((pos + 4084) > streamLength) {
					length = streamLength - pos;
				} else {
					length = 4084;
				}
				section->setPrivateDataByte(buffer + pos, length);
				pos += length;
				dsiDiiSectionList.push_back(section);
			}

			delete msg;
		}

		dsi = NULL;
		dii = NULL;

		blockBuffer = new char[blockSize];
		ml = moduleManager->getModuleList();
		i = ml->begin();
		while (i != ml->end()) {
			if (writeSections(fd, &dsiDiiSectionList) < 0) {
				close(fd);
				delete blockBuffer;
				return -2;
			}
			modFileName = tempFolder+"/mod-"+intToStrHexa(i->first)+".bin";
			if (isDir(modFileName) != 0) {
				delete blockBuffer;
				return -3;
			}
			fdr = getReadFD(modFileName);
			if (fdr <= 0) {
				close(fd);
				delete blockBuffer;
				return -4;
			}
			modSize = fileSize(modFileName);
			sn = 0;
			lsn = modSize / blockSize;
			if (lsn > 0) {
				if ((modSize % blockSize) > 0) {
					lsn += 1;
				}
				lsn--;
			}
			if (lsn > 0xFF) {
				lsn = 0xFF;
				cout << "Warning: carousel may not be compatible with all decoders." << endl;
				cout << "Tip: Avoid inserting files larger than 1016 KB." << endl;
			}
			blockNumber = 0;
			blockRd = read(fdr, blockBuffer, blockSize);
			while (blockRd > 0) {

				ddb = new DownloadDataBlock();
				ddb->setExtensionId(serviceDomain);
				ddb->setModuleId(i->first);
				ddb->setModuleVersion(0x01);
				ddb->setBlockDataByte(blockBuffer, blockRd);
				ddb->setBlockNumber(blockNumber++);
				streamLength = ddb->getStream(&buffer);
				section = new DSMCCSection();
				section->setTableId(0x3C);
				section->setSectionNumber(sn);
				section->setLastSectionNumber(lsn);
				section->setCurrentNextIndicator(1);
				section->setTableIdExtension(i->first);
				sn++;
				section->setPrivateDataByte(buffer, streamLength);
				sectionList.push_back(section);

				delete ddb;

				if (writeSections(fd, &sectionList) < 0) {
					close(fdr);
					close(fd);
					delete blockBuffer;
					return -5;
				}

				clearSectionList(&sectionList);

				blockRd = read(fdr, blockBuffer, blockSize);
			}

			close(fdr);
			++i;
		}

		close(fd);
		delete blockBuffer;

		return 0;
	}

	int Carousel::writeSections(int fd, vector<DSMCCSection*>* sectionList) {
		TSPacket* packet = NULL;
		vector<DSMCCSection*>::iterator i;
		unsigned int sectionSize, rw, pos, wb = 0;
		unsigned char payloadLength;
		char* buffer;
		char* pktBuffer;

		i = sectionList->begin();
		while (i != sectionList->end()) {
			(*i)->updateStream();
			sectionSize = (*i)->getStream(&buffer);
			if (sectionEncapsulationMode) {
				rw = write(fd, buffer, sectionSize);
				if (rw != sectionSize) {
					return -1;
				}
				wb += rw;
			} else {
				if (sectionSize == 0) {
					continue; //buffer is empty
				}
				pos = 0;
				while (pos < sectionSize) {
					if (sectionSize - pos > 184) {
						payloadLength = 184;
					} else {
						payloadLength = sectionSize - pos;
					}
					packet = new TSPacket(1, buffer + pos, payloadLength, NULL);
					if (pos == 0) {
						packet->setStartIndicator(1);
					} else {
						packet->setStartIndicator(0);
					}
					packet->setPid(pid);
					packet->setContinuityCounter(continuityCounter++);
					payloadLength = packet->getStream(&pktBuffer);
					pos += payloadLength;
					rw = write(fd, pktBuffer, 188);
					if (rw != 188) {
						return -1;
					}
					wb += rw;
					delete packet;
				}
			}
			++i;
		}

		return wb;
	}

	void Carousel::checkMinBlockSize() {
		map<unsigned short, Module*>* ml = NULL;
		map<unsigned short, Module*>::iterator i;
		unsigned int max = 0;
		unsigned int minbs;

		ml = moduleManager->getModuleList();
		i = ml->begin();
		while (i != ml->end()) {
			if (i->second->getSize() > max){
				max = i->second->getSize();
			}
			++i;
		}

		minbs = max / 65536;
		if ((max % 65536) > 0) {
			minbs++;
		}

		if (blockSize < minbs) {
			blockSize = minbs;
			cout << "Resizing BlockSize to " << blockSize <<
				". (Minimum value for this scenario)" << endl;
		}
	}

	void Carousel::clearSectionList(vector<DSMCCSection*>* sectionList) {
		vector<DSMCCSection*>::iterator i;

		i = sectionList->begin();
		while (i != sectionList->end()) {
			delete *i;
			++i;
		}
		sectionList->clear();
	}

	map<string,InternalIor*>* Carousel::getFilesIor() {
		return moduleManager->getFileManager()->getFileList();
	}

	unsigned short Carousel::getBlockSize() {
		return blockSize;
	}

	unsigned int Carousel::getServiceDomain() {
		return serviceDomain;
	}

	unsigned int Carousel::getTransactionId() {
		return transactionId;
	}

	string Carousel::getServiceGatewayFolder() {
		return serviceGatewayFolder;
	}

	void Carousel::setBlockSize(unsigned short size) {
		blockSize = size;
	}

	void Carousel::setServiceDomain(unsigned int sd) {
		serviceDomain = sd;
	}

	void Carousel::setTransactionId(unsigned int id) {
		id = id & 0x0000FFFF;
		transactionId = 0x80090000;
		transactionId = transactionId | id;
	}

	bool Carousel::getSectionEncapsulationMode() {
		return sectionEncapsulationMode;
	}

	void Carousel::setSectionEncapsulationMode(bool active) {
		sectionEncapsulationMode = active;
	}

	void Carousel::setServiceGatewayFolder(string path) {
		serviceGatewayFolder = path;
	}

}
}
}
}
}
