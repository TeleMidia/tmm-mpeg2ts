/*
 * Module.h
 *
 *  Created on: Nov 3, 2009
 *      Author: Felippe Nagato
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "dsmcc/carousel/Biop/FileMessage.h"
#include "util/functions.h"
using namespace br::pucrio::telemidia::mpeg2::util;

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	class Module {
		private:

		protected:
			unsigned short moduleId;
			vector<BiopMessage*>* biopList;
			unsigned int currentSize;
			bool sizeChanged;

			unsigned int getBiopSize(BiopMessage* biop);
			void clearBiopList();

		public:
			Module();
			~Module();

			bool addBiop(BiopMessage* biop);
			bool canAddBiop(BiopMessage* biop);
			unsigned int getSize();
			unsigned int getRemainingSize();
			unsigned int getNumberOfBiops();
			bool saveModuleToFile(string filename);
			unsigned short getModuleId();
			void setModuleId(unsigned short id);
			vector<BiopMessage*>* getBiopList();

	 };
}
}
}
}
}

#endif /* MODULE_H_ */
