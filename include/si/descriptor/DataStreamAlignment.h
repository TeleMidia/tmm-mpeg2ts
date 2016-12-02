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
 * DataStreamAlignment.h
 *
 *  Created on: 08/01/2014
 *      Author: Felippe Nagato
 */

#ifndef DATASTREAMALIGNMENT_H_
#define DATASTREAMALIGNMENT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class DataStreamAlignment : public MpegDescriptor {
	private:
		unsigned char alignmentType;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		DataStreamAlignment();
		~DataStreamAlignment();

		void setAlignmentType(unsigned char at);
		unsigned char getAlignmentType();
};

}
}
}
}

#endif /* DATASTREAMALIGNMENT_H_ */
