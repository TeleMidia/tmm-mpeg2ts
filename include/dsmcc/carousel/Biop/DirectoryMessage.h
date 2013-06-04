/*
 * DirectoryMessage.h
 *
 *  Created on: Jun 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef DIRECTORYMESSAGE_H_
#define DIRECTORYMESSAGE_H_

#include "dsmcc/carousel/Biop/BiopMessage.h"
#include "dsmcc/carousel/Binding.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class DirectoryMessage : public BiopMessage {

		private:
			void clearBindingsList();

		protected:
			char* objectInfoData; //max 65535 bytes
			unsigned short bindingsCount;
			vector<Binding*>* bindingsList;

			int64_t updateStream();
			unsigned int calculateMessageSize();
			unsigned int calculateMessageBodyLength();

		public:
			DirectoryMessage();
			~DirectoryMessage();

			int getObjectInfoData(char* data);
			unsigned char getBindingsCount();
			vector<Binding*>* getBindingsList();

			int setObjectInfoData(char* data, unsigned short length);
			void setBindingsList(vector<Binding*>* bl);

	 };
}
}
}
}
}

#endif /* DIRECTORYMESSAGE_H_ */
