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

#ifndef NPTREFERENCE_H_
#define NPTREFERENCE_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

class NPTReference : public MpegDescriptor {
	private:

	protected:
		char postDiscontinuityIndicator;
		unsigned char contentId;
		uint64_t stcRef;
		uint64_t nptRef;
		short scaleNumerator;
		unsigned short scaleDenominator;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		NPTReference();
		~NPTReference();

		char getPostDiscontinuityIndicator();
		unsigned char getContentId();
		uint64_t getStcRef();
		uint64_t getNptRef();
		short getScaleNumerator();
		unsigned short getScaleDenominator();

		void setPostDiscontinuityIndicator(char indicator);
		void setContentId(unsigned char id);
		void setStcRef(uint64_t stc);
		void setNptRef(uint64_t npt);
		void setScaleNumerator(short numerator);
		void setScaleDenominator(unsigned short denominator);

};

}
}
}
}
}

#endif /* NPTREFERENCE_H_ */
