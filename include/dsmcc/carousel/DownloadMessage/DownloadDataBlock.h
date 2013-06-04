/*
 * DownloadDataBlock.h
 *
 *  Created on: Jun 25, 2009
 *      Author: Felippe Nagato
 */

#ifndef DOWNLOADDATABLOCK_H_
#define DOWNLOADDATABLOCK_H_

#include "dsmcc/carousel/MessageHeader.h"

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class DownloadDataBlock : public MessageHeader {
		private:
			unsigned int blockDataByteLength;

		protected:
			unsigned short moduleId;
			unsigned char moduleVersion;
			unsigned short blockNumber;
			char* blockDataByte;

			int updateStream();
			unsigned int calculateStreamSize();

		public:
			DownloadDataBlock();
			~DownloadDataBlock();

			unsigned short getModuleId();
			unsigned char getModuleVersion();
			unsigned short getBlockNumber();
			unsigned int getBlockDataByte(char** dataStream);
			unsigned int getBlockDataByteLength();

			void setModuleId(unsigned short id);
			void setModuleVersion(unsigned char version);
			void setBlockNumber(unsigned short number);
			int setBlockDataByte(char* data, int length);

	 };
}
}
}
}
}

#endif /* DOWNLOADDATABLOCK_H_ */
