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
 * LocalTimeOffset.h
 *
 *  Created on: 03/05/2013
 *      Author: Felippe Nagato
 */

#ifndef LOCALTIMEOFFSET_H_
#define LOCALTIMEOFFSET_H_

#include "MpegDescriptor.h"
#include "si/Tot.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct LocalTimeData {
	 string countryCode;
	 unsigned char countryRegionId;
	 unsigned char localTimeOffsetPolarity;
	 time_t localTimeOffset;
	 time_t timeOfChange;
	 time_t nextTimeOffset;
};

class LocalTimeOffset : public MpegDescriptor {
	private:
		vector<LocalTimeData*> localTimeDataList;

	protected:
		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		LocalTimeOffset();
		~LocalTimeOffset();

		void addLocalTimeData(LocalTimeData* ltd);

		vector<LocalTimeData*>* getLocalTimeDataList();

};

}
}
}
}


#endif /* LOCALTIMEOFFSET_H_ */
