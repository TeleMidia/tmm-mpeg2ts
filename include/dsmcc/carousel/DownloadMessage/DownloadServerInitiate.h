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
