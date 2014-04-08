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
