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
 * DeferredAssociationTags.h
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#ifndef DEFERREDASSOCIATIONTAGS_H_
#define DEFERREDASSOCIATIONTAGS_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class DeferredAssociationTags : public MpegDescriptor {
	private:
		vector<unsigned short> associationTagList;
		unsigned short transportStreamId;
		unsigned short programNumber;
		unsigned short originalNetworkId;
		int privateDataByteLength;
		char* privateDataByte;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		DeferredAssociationTags();
		~DeferredAssociationTags();

		void addAssociationTag(unsigned short at);
		void setTransportStreamId(unsigned short id);
		unsigned short getTransportStreamId();
		void setProgramNumber(unsigned short pn);
		unsigned short getProgramNumber();
		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();
		void setPrivateDataByte(char* stream, int length);
		int getPrivateDataByte(char** stream);
};

}
}
}
}


#endif /* DEFERREDASSOCIATIONTAGS_H_ */
