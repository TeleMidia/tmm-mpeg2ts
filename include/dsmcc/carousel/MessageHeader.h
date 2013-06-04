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
