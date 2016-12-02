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
 * ServiceContext.h
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef SERVICECONTEXT_H_
#define SERVICECONTEXT_H_

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class ServiceContext {
		private:
			void process();
			int updateStream();

		protected:
			unsigned int contextId;
			unsigned short contextDataLength;
			char* contextDataByte;
			char* stream;

		public:
			ServiceContext();
			ServiceContext(
					unsigned int id, char* dataByte, unsigned short length);
			~ServiceContext();

			unsigned int getContextId();
			unsigned short getContextDataLength();
			unsigned short getContextDataByte(char* dataByte);
			void setContextId(unsigned int id);
			int setContextDataByte(char* dataByte, unsigned short length);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);
	 };
}
}
}
}
}

#endif /* SERVICECONTEXT_H_ */
