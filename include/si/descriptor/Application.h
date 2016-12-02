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
 * Application.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "MpegDescriptor.h"
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct AppProfile {
	 unsigned short applicationProfile;
	 unsigned char versionMajor;
	 unsigned char versionMinor;
	 unsigned char versionMicro;
};

class Application : public MpegDescriptor {

	private:

	protected:
		unsigned char applicationProfilesLength;
		bool serviceBoundFlag;
		unsigned char visibility;
		unsigned char applicationPriority;
		char* transportProtocolLabel;
		unsigned char transportProtocolLabelLength;
		vector<AppProfile*> appProfileList;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Application();
		~Application();

		void setApplicationProfilesLength(unsigned char len);
		unsigned char getApplicationProfilesLength();
		void setServiceBoundFlag(bool flag);
		bool getServiceBoundFlag();
		void setVisibility(unsigned char v);
		unsigned char getVisibility();
		void setApplicationPriority(unsigned char priority);
		unsigned char getApplicationPriority();
		void setTransportProtocolLabel(char* label, unsigned char len);
		unsigned char getTransportProtocolLabel(char** protocol);
		vector<AppProfile*>* getAppProfileList();
		void addAppProfile(unsigned short applicationProfile,
						   unsigned char versionMajor,
						   unsigned char versionMinor,
						   unsigned char versionMicro);

};

}
}
}
}


#endif /* APPLICATION_H_ */
