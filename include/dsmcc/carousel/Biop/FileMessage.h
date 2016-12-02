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
 * FileMessage.h
 *
 *  Created on: Jun 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef FILEMESSAGE_H_
#define FILEMESSAGE_H_

#include "dsmcc/carousel/Biop/BiopMessage.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class FileMessage : public BiopMessage{

		private:

		protected:
			uint64_t contentSize;
			unsigned int contentLength;
			string filename;

			int64_t updateStream();
			unsigned int calculateMessageSize();

		public:
			FileMessage();
			~FileMessage();

			uint64_t getContentSize();
			unsigned int getContentLength();
			string getFilename();
			bool setFilename(const string& filename);

			int64_t getSecondaryStreamLength();
			int64_t writeSecondaryStream(int fd);
	 };
}
}
}
}
}

#endif /* FILEMESSAGE_H_ */
