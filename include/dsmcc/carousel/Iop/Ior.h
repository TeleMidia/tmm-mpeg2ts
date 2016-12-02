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
 * Ior.h
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#ifndef IOR_H_
#define IOR_H_

#include "dsmcc/carousel/Iop/BiopProfileBody.h"

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Ior {

		private:

		protected:
			unsigned int typeIdLength;
			char* typeIdByte;
			unsigned int taggedProfileCount;
			vector<TaggedProfile*>* taggedProfileList;

			char* stream;

			int updateStream();
			void clearTaggedProfileList();

		public:
			Ior();
			~Ior();

			unsigned int getTypeIdLength();
			int getTypeIdByte(char** dataStream);
			unsigned int getTaggedProfileCount();
			vector<TaggedProfile*>* getTaggedProfileList();

			void setTypeIdByte(const char* data, unsigned int length);
			void setTaggedProfileCount(unsigned int count);
			void setTaggedProfileList(vector<TaggedProfile*>* tp);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* IOR_H_ */
