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
 * Nit.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef NIT_H_
#define NIT_H_

#include "PrivateSection.h"
#include "si/descriptor/NetworkName.h"
#include "si/descriptor/SystemManagement.h"
#include "si/descriptor/ServiceList.h"
#include "si/descriptor/TerrestrialDeliverySystem.h"
#include "si/descriptor/TSInformation.h"
#include "si/descriptor/PartialReception.h"
#include "Crc32.h"

#include <vector>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct TransportInformation {
	 unsigned short transportStreamId;
	 unsigned short originalNetworkId;
	 unsigned short transportDescriptorsLength;
	 vector<MpegDescriptor*> descriptorList;
};

class Nit : public PrivateSection {

	private:

	protected:
		unsigned short networkDescriptorsLength;
		vector<MpegDescriptor*> descriptorsList;
		unsigned short transportStreamLoopLength;
		vector<TransportInformation*> transportList;

		int processSectionPayload();
		int calculateSectionSize();

	public:
		Nit();
		virtual ~Nit();

		int updateStream();

		vector<MpegDescriptor*>* getDescriptorsList();
		vector<TransportInformation*>* getTransportList();
		void addDescriptor(MpegDescriptor* d);
		void addTransportInformation(TransportInformation *ti);
		void releaseAllDescriptors();
		void releaseAllTransportInformation();

};

}
}
}
}


#endif /* NIT_H_ */
