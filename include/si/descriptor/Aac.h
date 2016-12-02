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
 * Aac.h
 *
 *  Created on: 23/10/2013
 *      Author: Felippe Nagato
 */

#ifndef AAC_H_
#define AAC_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Aac : public MpegDescriptor {
	private:
		unsigned char profileAndLevel;
		bool aacTypeFlag;
		unsigned char aacType;
		char* aditionalInfo;
		unsigned char aditionalInfoLength;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Aac();
		~Aac();

		void setProfileAndLevel(unsigned char pl);
		unsigned char getProfileAndLevel();
		void setAacTypeFlag(bool flag);
		bool getAacTypeFlag();
		void setAacType(unsigned char type);
		unsigned char getAacType();
		void setAditionalInfo(char* dataStream, unsigned char length);
		unsigned char getAditionalInfo(char** dataStream);
};

}
}
}
}


#endif /* AAC_H_ */
