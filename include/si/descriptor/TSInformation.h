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
 * TSInformation.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef TSINFORMATION_H_
#define TSINFORMATION_H_

#include "MpegDescriptor.h"
#include <set>
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct TransmissionType {
	 unsigned char transmissionTypeInfo;
	 unsigned char numOfService;
	 set<unsigned short> serviceIdList;
};

class TSInformation : public MpegDescriptor {

	private:
		unsigned char remoteControlKeyId; // Virtual channel
		string tsName;
		set<TransmissionType*> transmissionTypeList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		TSInformation();
		~TSInformation();

		void setRemoteControlKeyId(unsigned char key);
		unsigned char getRemoteControlKeyId();
		unsigned char getTransmissionTypeCount();
		void setTsName(string name);
		string getTsName();
		void addTransmissionTypeList(TransmissionType* tt);
		set<TransmissionType*>* getTransmissionTypeList();
};

}
}
}
}


#endif /* TSINFORMATION_H_ */
