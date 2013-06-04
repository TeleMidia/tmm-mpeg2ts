/*
 * ObjectLocation.h
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#ifndef OBJECTLOCATION_H_
#define OBJECTLOCATION_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	struct InternalIor {
			 unsigned int carousel;
			 unsigned short moduleId;
			 unsigned int key;
	};

	class ObjectLocation {

		private:

		protected:
			unsigned int componentIdTag;
			unsigned char componentDataLength;
			unsigned char versionMajor;
			unsigned char versionMinor;
			unsigned char objectKeyLength;
			InternalIor* iior;

			char* stream;

			int updateStream();

		public:
			ObjectLocation();
			~ObjectLocation();

			unsigned int getComponentIdTag();
			unsigned char getComponentDataLength();
			unsigned int getCarouselId();
			unsigned short getModuleId();
			unsigned char getVersionMajor();
			unsigned char getVersionMinor();
			unsigned char getObjectKeyLength();
			unsigned int getObjectKeyData();
			InternalIor* getIior();

			void setVersionMajor(unsigned char version);
			void setVersionMinor(unsigned char version);
			void setIior(InternalIor* iior);

			int getStreamLength();

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* OBJECTLOCATION_H_ */
