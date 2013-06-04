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
			vector<eventName*>* eventNameList;

			int64_t updateStream();
			unsigned int calculateMessageSize();

		public:
			StreamEventMessage();
			~StreamEventMessage();

			int addEvent(unsigned short id, char* name, unsigned char length);
			void clearEventNameList();

	 };
}
}
}
}
}

#endif /* STREAMEVENTMESSAGE_H_ */
