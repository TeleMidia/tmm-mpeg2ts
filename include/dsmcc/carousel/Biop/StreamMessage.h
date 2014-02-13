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
