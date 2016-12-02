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
 * ConnBinder.h
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#ifndef CONNBINDER_H_
#define CONNBINDER_H_

#include "dsmcc/carousel/Tap.h"

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class ConnBinder {

		private:

		protected:
			unsigned int componentIdTag;
			unsigned char componentDataLength;
			unsigned char tapsCount;
			Tap* tap;
			char* addicionalTapByte;

			char* stream;

			int updateStream();

		public:
			ConnBinder();
			~ConnBinder();

			unsigned int getComponentIdTag();
			unsigned char getComponentDataLength();
			unsigned char getTapsCount();
			Tap* getTap();
			unsigned char getAddicionalTapByte(char** dataStream);

			void setTapsCount(unsigned char count);
			void setTap(Tap* tp);
			void setAddicionalTapByte(char* data, unsigned char length);

			int getStreamLength();

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* CONNBINDER_H_ */
