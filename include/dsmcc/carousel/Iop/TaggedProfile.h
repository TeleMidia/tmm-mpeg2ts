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
 * TaggedProfile.h
 *
 *  Created on: Jun 17, 2009
 *      Author: Felippe Nagato
 */

#ifndef TAGGEDPROFILE_H_
#define TAGGEDPROFILE_H_

#include <iostream>
#include <string.h>

/* #include "carousel/../Biop/LiteComponent.h" */

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class TaggedProfile {

		private:

		protected:
			unsigned int profileIdTag;
			unsigned int profileDataLength;
			unsigned char profileDataByteOrder;
			unsigned char liteComponentCount;
			//LiteComponent* liteComponent;

			char* stream;

			virtual int updateStream();
			virtual unsigned int calculateStreamSize();

		public:
			TaggedProfile();
			virtual ~TaggedProfile();

			unsigned int getProfileIdTag();
			unsigned int getProfileDataLength();
			unsigned char getProfileDataByteOrder();
			unsigned char getLiteComponentCount();

			void setProfileDataByteOrder(unsigned char pdbo);
			void setLiteComponentCount(unsigned char count);

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* TAGGEDPROFILE_H_ */
