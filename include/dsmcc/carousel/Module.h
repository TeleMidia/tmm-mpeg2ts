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
			bool saveModuleToFile(const string& filename);
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
