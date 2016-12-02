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
 * NPTEndpoint.h
 *
 *  Created on: Oct 22, 2012
 *      Author: Felippe Nagato
 */

#ifndef NPTENDPOINT_H_
#define NPTENDPOINT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

class NPTEndpoint : public MpegDescriptor {
	private:

	protected:
		uint64_t startNPT;
		uint64_t stopNPT;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		NPTEndpoint();
		~NPTEndpoint();

		uint64_t getStartNPT();
		uint64_t getStopNPT();
		void setStartNPT(uint64_t npt);
		void setStopNPT(uint64_t npt);

};

}
}
}
}
}

#endif /* NPTENDPOINT_H_ */
