/*
 * Binding.h
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#ifndef BINDING_H_
#define BINDING_H_

#include "dsmcc/carousel/Iop/Ior.h"
#include "dsmcc/carousel/Biop/Name.h"

#include <inttypes.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Binding {

		private:

		protected:

			unsigned char nameComponentsCount;
			Name* name;
			unsigned char bindingType;
			Ior* ior;
			unsigned short objectInfoLength;
			uint64_t contentSize;
			char* descriptorByte;

			char* stream;

			int updateStream();

		public:
			Binding();
			~Binding();

			unsigned char getNameComponentsCount();
			Name* getName();
			unsigned char getBindingType();
			Ior* getIor();
			unsigned short getObjectInfoLength();
			uint64_t getContentSize();
			int getDescriptorByte(char** dataStream);

			void setNameComponentsCount(unsigned char count);
			void setName(Name* name);
			void setBindingType(unsigned char bt);
			void setIor(Ior* ior);
			void setContentSize(uint64_t cs);
			void setDescriptorByte(char* data, int length);


			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* BINDING_H_ */
