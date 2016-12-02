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
 * DownloadServerInitate.h
 *
 *  Created on: Jun 25, 2009
 *      Author: Felippe Nagato
 */

#ifndef DOWNLOADSERVERINITIATE_H_
#define DOWNLOADSERVERINITIATE_H_

#include "dsmcc/carousel/MessageHeader.h"

#include <iostream>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class DownloadServerInitiate : public MessageHeader {
		private:

		protected:
			char serverId[20];
			//compatibilityDescriptor()
			unsigned short privateDataLength;
			char* privateDataByte;

			int updateStream();
			unsigned int calculateStreamSize();

		public:
			DownloadServerInitiate();
			~DownloadServerInitiate();

			int getServerId(char** dataStream);
			unsigned short getPrivateDataLength();
			int getPrivateDataByte(char** dataStream);

			void setServerId(char* data, unsigned char length);
			int setPrivateDataByte(char* data, unsigned short length);
	 };
}
}
}
}
}

#endif /* DOWNLOADSERVERINITIATE_H_ */
