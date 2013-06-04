/*
 * Module.cpp
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/Module.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	Module::Module() {
		biopList = new vector<BiopMessage*>;
		currentSize = 0;
		moduleId = 0;
		sizeChanged = false;
	}

	Module::~Module() {
		if (biopList != NULL) {
			clearBiopList();
			delete biopList;
		}
	}

	bool Module::addBiop(BiopMessage* biop) {
		if (canAddBiop(biop)) {
			biopList->push_back(biop);
			sizeChanged = true;
			return true;
		}
		return false;
	}

	bool Module::canAddBiop(BiopMessage* biop) {
		if (biop != NULL) {
			return (getBiopSize(biop) <= getRemainingSize());
		}
		return false;
	}

	unsigned int Module::getSize() {
		vector<BiopMessage*>::iterator i;
		BiopMessage* biop = NULL;

		if (sizeChanged) {
			sizeChanged = false;
			currentSize = 0;

			i = biopList->begin();
			while (i != biopList->end()) {
				biop = *i;
				currentSize = currentSize + getBiopSize(biop);
				++i;
			}
		}

		return currentSize;
	}

	unsigned int Module::getRemainingSize() {
		vector<BiopMessage*>::iterator i;
		BiopMessage* biop = NULL;

		getSize();

		if (biopList->size() == 0) {
			return 0xFFFFFFFF;
		} else if (biopList->size() == 1) {
			i = biopList->begin();
			biop = *i;
			if (getBiopSize(biop) >= 65535) {
				return 0;
			}
		}
		return (65535 - currentSize);
	}

	unsigned int Module::getBiopSize(BiopMessage* biop) {
		char* kind;
		string kindStr;
		FileMessage* biopFile;

		biop->getObjectKindData(&kind);
		kindStr.assign(kind, 3);
		if (kindStr == "fil") {
			biopFile = (FileMessage*) biop;
			return (biopFile->getPrimaryStreamLength() +
				biopFile->getSecondaryStreamLength());
		}
		return biop->getPrimaryStreamLength();
	}

	unsigned int Module::getNumberOfBiops() {
		return biopList->size();
	}

	bool Module::saveModuleToFile(string filename) {
		int fd;
		char* kind;
		string kindStr;
		BiopMessage* biop;
		FileMessage* biopFile;
		vector<BiopMessage*>::iterator i;

		fd = getWriteFD(filename);
		if (fd < 0) {
			return false;
		}

		i = biopList->begin();
		while (i != biopList->end()) {
			biop = *i;
			//write to file
			biop->writePrimaryStream(fd);
			biop->getObjectKindData(&kind);
			kindStr.assign(kind, 3);
			if (kindStr == "fil") {
				biopFile = (FileMessage*) biop;
				biopFile->writeSecondaryStream(fd);
			}
			++i;
		}

		close(fd);
		return true;
	}

	unsigned short Module::getModuleId() {
		return moduleId;
	}

	void Module::setModuleId(unsigned short id) {
		moduleId = id;
	}

	void Module::clearBiopList() {
		vector<BiopMessage*>::iterator i;

		if (biopList == NULL) {
			return;
		}

		i = biopList->begin();
		while (i != biopList->end()) {
			delete *i;
			++i;
		}
	}

	vector<BiopMessage*>* Module::getBiopList() {
		return biopList;
	}

}
}
}
}
}
