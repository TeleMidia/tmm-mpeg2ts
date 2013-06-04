/*
 * TaggedProfile.h
 *
 *  Created on: Jun 17, 2009
 *      Author: Felippe Nagato
 */

#ifndef TAGGEDPROFILE_H_
#define TAGGEDPROFILE_H_

#include <iostream>
#include <string.h>

/* #include "carousel/../Biop/LiteComponent.h" */

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class TaggedProfile {

		private:

		protected:
			unsigned int profileIdTag;
			unsigned int profileDataLength;
			unsigned char profileDataByteOrder;
			unsigned char liteComponentCount;
			//LiteComponent* liteComponent;

			char* stream;

			virtual int updateStream();
			virtual unsigned int calculateStreamSize();

		public:
			TaggedProfile();
			virtual ~TaggedProfile();

			unsigned int getProfileIdTag();
			unsigned int getProfileDataLength();
			unsigned char getProfileDataByteOrder();
			unsigned char getLiteComponentCount();

			void setProfileDataByteOrder(unsigned char pdbo);
			void setLiteComponentCount(unsigned char count);

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* TAGGEDPROFILE_H_ */
