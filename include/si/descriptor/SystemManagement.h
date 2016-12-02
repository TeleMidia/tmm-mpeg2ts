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
 * SystemManagement.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef SYSTEMMANAGEMENT_H_
#define SYSTEMMANAGEMENT_H_

#include "MpegDescriptor.h"
#include <map>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class SystemManagement : public MpegDescriptor {

	#define SM_BROADCASTING 0x00
	#define SM_NON_BROADCASTING1 0x01
	#define SM_NON_BROADCASTING2 0x02
	#define SM_UNDEFINED 0x03

	#define ST_UNDEFINED 0x00
	#define ST_ISDB_SYSTEM 0x03

	private:
		unsigned char broadcastingFlag;
		unsigned char broadcastingIdentifier;
		unsigned char additionalBroadcastingId;

		unsigned char additionalIdentificationInfoLength;
		char* additionalIdentificationInfo;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		SystemManagement();
		~SystemManagement();

		void setBroadcastingFlag(unsigned char bf);
		unsigned char getBroadcastingFlag();
		void setBroadcastingIdentifier(unsigned char bi);
		unsigned char getBroadcastingIdentifier();
		void setAdditionalBroadcastingId(unsigned char abi);
		unsigned char getAdditionalBroadcastingId();

		void setAdditionalIdentificationInfo(char* stream, unsigned char len);
		unsigned char getAdditionalIdentificationInfo(char** stream);
};

}
}
}
}


#endif /* SYSTEMMANAGEMENT_H_ */
