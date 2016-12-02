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
 * Name.h
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#ifndef NAME_H_
#define NAME_H_

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Name {

		private:

		protected:
			unsigned char idLength;
			char* idData;
			unsigned char kindLength;
			char* kindData;
			char* stream;

			int updateStream();

		public:
			Name();
			~Name();

			unsigned char getIdLength();
			int getIdData(char** dataStream);
			unsigned char getKindLength();
			int getKindData(char** dataStream);

			void setIdData(const char* data, unsigned char length);
			void setKindData(const char* data, unsigned char length);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* NAME_H_ */
