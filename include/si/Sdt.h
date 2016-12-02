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
 * Sdt.h
 *
 *  Created on: 18/06/2013
 *      Author: Felippe Nagato
 */

#ifndef SDT_H_
#define SDT_H_

#include "PrivateSection.h"
#include "si/descriptor/Service.h"
#include "Crc32.h"

#include <vector>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct ServiceInformation {
	 unsigned short serviceId;
	 bool eitScheduleFlag;
	 bool eitPresentFollowingFlag;
	 unsigned char runningStatus;
	 bool freeCaMode;
	 unsigned char descriptorLoopLength;
	 vector<MpegDescriptor*> descriptorList;
};

class Sdt : public PrivateSection {

	#define RS_UNDEFINED				0
	#define RS_NOT_RUNNING				1
	#define RS_STARTS_IN_A_FEW_SECONDS	2
	#define RS_PAUSING					3
	#define RS_RUNNING					4

	private:

	protected:
		unsigned short originalNetworkId;
		vector<ServiceInformation*> serviceList;

		int processSectionPayload();
		int calculateSectionSize();

	public:
		Sdt();
		virtual ~Sdt();

		int updateStream();

		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();

		void addServiceInformation(ServiceInformation *si);
		void releaseAllServiceInformation();

};

}
}
}
}

#endif /* SDT_H_ */
