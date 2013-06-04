/*
 * Name.h
 *
 *  Created on: Jun 9, 2009
 *      Author: Felippe Nagato
 */

#ifndef NAME_H_
#define NAME_H_

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class Name {

		private:

		protected:
			unsigned char idLength;
			char* idData;
			unsigned char kindLength;
			char* kindData;
			char* stream;

			int updateStream();

		public:
			Name();
			~Name();

			unsigned char getIdLength();
			int getIdData(char** dataStream);
			unsigned char getKindLength();
			int getKindData(char** dataStream);

			void setIdData(const char* data, unsigned char length);
			void setKindData(const char* data, unsigned char length);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* NAME_H_ */
