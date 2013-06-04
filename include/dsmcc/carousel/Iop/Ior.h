/*
 * Ior.h
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#ifndef IOR_H_
#define IOR_H_

#include "dsmcc/carousel/Iop/BiopProfileBody.h"

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Ior {

		private:

		protected:
			unsigned int typeIdLength;
			char* typeIdByte;
			unsigned int taggedProfileCount;
			vector<TaggedProfile*>* taggedProfileList;

			char* stream;

			int updateStream();
			void clearTaggedProfileList();

		public:
			Ior();
			~Ior();

			unsigned int getTypeIdLength();
			int getTypeIdByte(char** dataStream);
			unsigned int getTaggedProfileCount();
			vector<TaggedProfile*>* getTaggedProfileList();

			void setTypeIdByte(const char* data, unsigned int length);
			void setTaggedProfileCount(unsigned int count);
			void setTaggedProfileList(vector<TaggedProfile*>* tp);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* IOR_H_ */
