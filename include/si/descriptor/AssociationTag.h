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
 * AssociationTag.h
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#ifndef ASSOCIATIONTAG_H_
#define ASSOCIATIONTAG_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class AssociationTag : public MpegDescriptor {
	private:
		unsigned short associationTag;
		unsigned short use;
		unsigned char selectorLength;
		unsigned int transactionId;
		unsigned int timeout;
		char* selectorByte;
		int privateDataByteLength;
		char* privateDataByte;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		AssociationTag();
		~AssociationTag();

		void setAssociationTag(unsigned short at);
		unsigned short getAssociationTag();
		void setUse(unsigned short use);
		unsigned short getUse();
		unsigned char getSelectorLength();
		void setTransactionId(unsigned int id);
		unsigned int getTransactionId();
		void setTimeout(unsigned int t);
		unsigned int getTimeout();
		void setSelectorByte(char* stream, int length);
		int getSelectorByte(char** stream);
		void setPrivateDataByte(char* stream, int length);
		int getPrivateDataByte(char** stream);
};

}
}
}
}


#endif /* ASSOCIATIONTAG_H_ */
