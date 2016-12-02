/******************************************************************************
TeleMídia Multiplexer MPEG-2 Transport Stream library
Copyright (C) 2016 TeleMídia Lab/PUC-Rio
https://github.com/TeleMidia/tmm-mpeg2ts/graphs/contributors

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
for more details. You should have received a copy of the GNU Affero General
Public License along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

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
