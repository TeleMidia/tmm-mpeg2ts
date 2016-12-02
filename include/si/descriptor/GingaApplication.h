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
 * GingaApplication.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef GINGAAPPLICATION_H_
#define GINGAAPPLICATION_H_

#include "MpegDescriptor.h"
#include <cstring>
#include <string>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class GingaApplication : public MpegDescriptor {
	private:

	protected:
		vector<string> parameterList;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		GingaApplication(unsigned char descriptorTag);
		~GingaApplication();

		void addParameter(string param);
		vector<string>* getParameterList();

};

}
}
}
}


#endif /* GINGAAPPLICATION_H_ */
