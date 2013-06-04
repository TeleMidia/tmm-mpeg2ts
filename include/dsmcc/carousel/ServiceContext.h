/*
 * ServiceContext.h
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef SERVICECONTEXT_H_
#define SERVICECONTEXT_H_

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class ServiceContext {
		private:
			void process();
			int updateStream();

		protected:
			unsigned int contextId;
			unsigned short contextDataLength;
			char* contextDataByte;
			char* stream;

		public:
			ServiceContext();
			ServiceContext(
					unsigned int id, char* dataByte, unsigned short length);
			~ServiceContext();

			unsigned int getContextId();
			unsigned short getContextDataLength();
			unsigned short getContextDataByte(char* dataByte);
			void setContextId(unsigned int id);
			int setContextDataByte(char* dataByte, unsigned short length);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);
	 };
}
}
}
}
}

#endif /* SERVICECONTEXT_H_ */
