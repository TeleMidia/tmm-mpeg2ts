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
