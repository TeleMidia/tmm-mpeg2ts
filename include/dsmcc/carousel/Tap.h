/*
 * Tap.h
 *
 *  Created on: May 14, 2009
 *      Author: Felippe Nagato
 */

#ifndef TAP_H_
#define TAP_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Tap {
		public:
			static const unsigned short STR_NPT_USE = 0x000B;
			static const unsigned short STR_STATUS_AND_EVENT_USE = 0x000C;
			static const unsigned short STR_EVENT_USE = 0x000D;
			static const unsigned short STR_STATUS_USE = 0x000E;
			static const unsigned short BIOP_ES_USE = 0x0018;
			static const unsigned short BIOP_PROGRAM_USE = 0x0019;

		private:

		protected:
			unsigned short id;
			unsigned short use;
			unsigned short assocTag;
			unsigned char selectorLength;
			unsigned short selectorType;
			unsigned int transactionId;
			unsigned int timeout;

			char* stream;

			int updateStream();

		public:
			Tap();
			~Tap();

			unsigned short getId();
			unsigned short getUse();
			unsigned short getAssocTag();
			unsigned char getSelectorLength();
			unsigned short getSelectorType();
			unsigned int getTransactionId();
			unsigned int getTimeout();

			void setId(unsigned short id);
			void setUse(unsigned short use);
			void setAssocTag(unsigned short at);
			void setSelectorLength(unsigned short slen);
			void setSelectorType(unsigned short st);
			void setTransactionId(unsigned int tid);
			void setTimeout(unsigned int to);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);
	 };
}
}
}
}
}

#endif /* TAP_H_ */
