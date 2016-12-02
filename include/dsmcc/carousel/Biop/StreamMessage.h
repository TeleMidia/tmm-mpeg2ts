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
 * StreamMessage.h
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef STREAMMESSAGE_H_
#define STREAMMESSAGE_H_

#include <iostream>

#include "dsmcc/carousel/Biop/BiopMessage.h"
#include "dsmcc/carousel/StreamInfoT.h"
#include "dsmcc/carousel/Tap.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class StreamMessage : public BiopMessage {

		private:
			void clearTapsList();

		protected:
			StreamInfoT* objectInfo;
			unsigned short objectInfoByteLength;
			char* objectInfoByte;
			vector<Tap*> tapsList;

			virtual int64_t updateStream();
			virtual unsigned int calculateMessageSize();

		public:
			StreamMessage();
			~StreamMessage();

			StreamInfoT* getObjectInfo();
			int getObjectInfoByte(char* data);
			unsigned char getTapsCount();
			vector<Tap*>* getTapsList();

			void setObjectInfo(StreamInfoT* oi);
			int setObjectInfoByte(char* data, unsigned short length);
			void addTap(Tap* tap);

	 };
}
}
}
}
}

#endif /* STREAMMESSAGE_H_ */
