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
 * ApplicationName.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef APPLICATIONNAME_H_
#define APPLICATIONNAME_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct AppName {
	 string ISO639LanguageCode;
	 unsigned char appNameLength;
	 string name;
};

class ApplicationName : public MpegDescriptor {
	private:

	protected:
		vector<AppName*> appNameList;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		ApplicationName();
		~ApplicationName();

		void setAppName(string ISO639LanguageCode, string name);
		vector<AppName*>* getAppNameList();

};

}
}
}
}


#endif /* APPLICATIONNAME_H_ */
