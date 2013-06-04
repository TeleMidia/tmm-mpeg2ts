/*
 * ConnBinder.h
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#ifndef CONNBINDER_H_
#define CONNBINDER_H_

#include "dsmcc/carousel/Tap.h"

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class ConnBinder {

		private:

		protected:
			unsigned int componentIdTag;
			unsigned char componentDataLength;
			unsigned char tapsCount;
			Tap* tap;
			char* addicionalTapByte;

			char* stream;

			int updateStream();

		public:
			ConnBinder();
			~ConnBinder();

			unsigned int getComponentIdTag();
			unsigned char getComponentDataLength();
			unsigned char getTapsCount();
			Tap* getTap();
			unsigned char getAddicionalTapByte(char** dataStream);

			void setTapsCount(unsigned char count);
			void setTap(Tap* tp);
			void setAddicionalTapByte(char* data, unsigned char length);

			int getStreamLength();

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* CONNBINDER_H_ */
