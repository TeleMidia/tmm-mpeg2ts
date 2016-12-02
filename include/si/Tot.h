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
 * Tot.h
 *
 *  Created on: 03/05/2013
 *      Author: Felippe Nagato
 */

#ifndef TOT_H_
#define TOT_H_

#include "PrivateSection.h"
#include "si/descriptor/LocalTimeOffset.h"
#include "Crc32.h"

#include <ctime>
#include <vector>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Tot : public PrivateSection {

	#define MJD_1970_01_01 40587

	private:

	protected:
		time_t dateTime;
		vector<MpegDescriptor*> descriptorList;
		unsigned short descriptorsLength;

		int processSectionPayload();
		int calculateSectionSize();

	public:
		Tot();
		virtual ~Tot();

		int updateStream();

		void setDateTime(time_t rawTime);
		time_t getDateTime();
		vector<MpegDescriptor*>* getDescriptorList();
		void addDescriptor(MpegDescriptor *d);
		void releaseAllDescriptors();

		static unsigned int dateToMJD(time_t rawTime);
		static time_t MJDtoDateTime(unsigned int mjd);
		static unsigned char decToBCD(unsigned char val);
		static unsigned char BCDtoDec(unsigned char val);
		static unsigned int timeToBCD(time_t rawTime, char utc);
		static time_t BCDtoTime(unsigned int bcd);
		static short localTimezone(); //in minutes
		static time_t makeUtcTime(unsigned char hh, unsigned char mm,
								  unsigned char ss);

};

}
}
}
}


#endif /* TOT_H_ */
