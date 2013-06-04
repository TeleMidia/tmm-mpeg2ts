/*
 * ModuleInfo.h
 *
 *  Created on: Jun 29, 2009
 *      Author: Felippe Nagato
 */

#ifndef MODULEINFO_H_
#define MODULEINFO_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class ModuleInfo {
		private:

		protected:
			unsigned short moduleId;
			unsigned int moduleSize;
			unsigned char moduleVersion;
			unsigned char moduleInfoLength;
			char* moduleInfoByte;

			char* stream;

			int updateStream();

		public:
			ModuleInfo();
			~ModuleInfo();

			unsigned short getModuleId();
			unsigned int getModuleSize();
			unsigned char getModuleVersion();
			unsigned char getModuleInfoLength();
			int getModuleInfoByte(char** dataStream);

			void setModuleId(unsigned short id);
			void setModuleSize(unsigned int size);
			void setModuleVersion(unsigned char version);
			int setModuleInfoByte(char* data, int length);

			int getStreamLength();
			int getStream(char* dataStream);
			int getStream(char** dataStream);
	 };
}
}
}
}
}

#endif /* MODULEINFO_H_ */
