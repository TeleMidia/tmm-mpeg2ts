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
 * ObjectLocation.h
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#ifndef OBJECTLOCATION_H_
#define OBJECTLOCATION_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	struct InternalIor {
			 unsigned int carousel;
			 unsigned short moduleId;
			 unsigned int key;
	};

	class ObjectLocation {

		private:

		protected:
			unsigned int componentIdTag;
			unsigned char componentDataLength;
			unsigned char versionMajor;
			unsigned char versionMinor;
			unsigned char objectKeyLength;
			InternalIor* iior;

			char* stream;

			int updateStream();

		public:
			ObjectLocation();
			~ObjectLocation();

			unsigned int getComponentIdTag();
			unsigned char getComponentDataLength();
			unsigned int getCarouselId();
			unsigned short getModuleId();
			unsigned char getVersionMajor();
			unsigned char getVersionMinor();
			unsigned char getObjectKeyLength();
			unsigned int getObjectKeyData();
			InternalIor* getIior();

			void setVersionMajor(unsigned char version);
			void setVersionMinor(unsigned char version);
			void setIior(InternalIor* iior);

			int getStreamLength();

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* OBJECTLOCATION_H_ */
