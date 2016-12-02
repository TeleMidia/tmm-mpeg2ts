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
 * Binding.h
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#ifndef BINDING_H_
#define BINDING_H_

#include "dsmcc/carousel/Iop/Ior.h"
#include "dsmcc/carousel/Biop/Name.h"

#include <inttypes.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Binding {

		private:

		protected:

			unsigned char nameComponentsCount;
			Name* name;
			unsigned char bindingType;
			Ior* ior;
			unsigned short objectInfoLength;
			uint64_t contentSize;
			char* descriptorByte;

			char* stream;

			int updateStream();

		public:
			Binding();
			~Binding();

			unsigned char getNameComponentsCount();
			Name* getName();
			unsigned char getBindingType();
			Ior* getIor();
			unsigned short getObjectInfoLength();
			uint64_t getContentSize();
			int getDescriptorByte(char** dataStream);

			void setNameComponentsCount(unsigned char count);
			void setName(Name* name);
			void setBindingType(unsigned char bt);
			void setIor(Ior* ior);
			void setContentSize(uint64_t cs);
			void setDescriptorByte(char* data, int length);


			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* BINDING_H_ */
