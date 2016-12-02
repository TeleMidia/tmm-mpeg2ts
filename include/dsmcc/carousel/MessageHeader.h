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
 * MessageHeader.h
 *
 *  Created on: Jun 26, 2009
 *      Author: Felippe Nagato
 */

#ifndef MESSAGEHEADER_H_
#define MESSAGEHEADER_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class MessageHeader {
		private:

		protected:
			unsigned char protocolDiscriminator;
			unsigned char dsmccType;
			unsigned short messageId;

			/*
			 Message Header = transactionId
			 Download Message Header = downloadId
			*/
			unsigned int extensionId;
			unsigned char adaptationLength;
			unsigned short messageLength;

			char* stream;

			virtual int updateStream();
			virtual unsigned int calculateStreamSize();

		public:
			MessageHeader();
			virtual ~MessageHeader();

			unsigned char getProtocolDiscriminator();
			unsigned char getDsmccType();
			unsigned short getMessageId();
			unsigned int getExtensionId();
			unsigned char getAdaptationLength();
			unsigned short getMessageLength();

			void setProtocolDiscriminator(unsigned char pd);
			void setDsmccType(unsigned char type);
			void setMessageId(unsigned short id);
			void setExtensionId(unsigned int id);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);
	 };
}
}
}
}
}

#endif /* MESSAGEHEADER_H_ */
