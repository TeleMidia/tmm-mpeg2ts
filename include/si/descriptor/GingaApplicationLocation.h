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
 * GingaApplicationLocation.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef GINGAAPPLICATIONLOCATION_H_
#define GINGAAPPLICATIONLOCATION_H_

#include "MpegDescriptor.h"
#include <cstring>
#include <string>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class GingaApplicationLocation : public MpegDescriptor {
	private:

	protected:
		string baseDirectory;
		string classExtension;
		string initialClass;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		GingaApplicationLocation(unsigned char descriptorTag);
		~GingaApplicationLocation();

		void setBaseDirectory(string dir);
		string getBaseDirectory();
		void setClassExtension(string ext);
		string getClassExtension();
		void setInitialClass(string init);
		string getInitialClass();

};

}
}
}
}


#endif /* GINGAAPPLICATIONLOCATION_H_ */
