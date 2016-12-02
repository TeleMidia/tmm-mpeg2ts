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
 * ModuleInfo.h
 *
 *  Created on: Jun 29, 2009
 *      Author: Felippe Nagato
 */

#ifndef MODULEINFO_H_
#define MODULEINFO_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class ModuleInfo {
		private:

		protected:
			unsigned short moduleId;
			unsigned int moduleSize;
			unsigned char moduleVersion;
			unsigned char moduleInfoLength;
			char* moduleInfoByte;

			char* stream;

			int updateStream();

		public:
			ModuleInfo();
			~ModuleInfo();

			unsigned short getModuleId();
			unsigned int getModuleSize();
			unsigned char getModuleVersion();
			unsigned char getModuleInfoLength();
			int getModuleInfoByte(char** dataStream);

			void setModuleId(unsigned short id);
			void setModuleSize(unsigned int size);
			void setModuleVersion(unsigned char version);
			int setModuleInfoByte(char* data, int length);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);
	 };
}
}
}
}
}

#endif /* MODULEINFO_H_ */
