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
 * StreamEventMessage.h
 *
 *  Created on: May 18, 2009
 *      Author: Felippe Nagato
 */

#ifndef STREAMEVENTMESSAGE_H_
#define STREAMEVENTMESSAGE_H_

#include <iostream>

#include "dsmcc/carousel/Biop/StreamMessage.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class StreamEventMessage : public StreamMessage{

		typedef struct {
			int eventNameLength;
			unsigned short int eventId;
			char* eventNameData;
		} eventName;

		private:

		protected:
			vector<eventName*> eventNameList;

			int64_t updateStream();
			unsigned int calculateMessageSize();
			unsigned int calculateEventNameSize();

		public:
			StreamEventMessage();
			~StreamEventMessage();

			void setObjectInfo(StreamInfoT* oi);
			int addEvent(unsigned short id, char* name, unsigned char length);
			void clearEventNameList();

	 };
}
}
}
}
}

#endif /* STREAMEVENTMESSAGE_H_ */
