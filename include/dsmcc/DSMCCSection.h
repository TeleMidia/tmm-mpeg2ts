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

#ifndef DSMCCSECTION_H_
#define DSMCCSECTION_H_

#include <map>
#include "PrivateSection.h"
#include "Crc32.h"
#include "MpegDescriptor.h"
#include "dsmcc/descriptor/NPTReference.h"
#include "dsmcc/descriptor/NPTEndpoint.h"
#include "dsmcc/descriptor/StreamMode.h"
#include "dsmcc/descriptor/StreamEvent.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

class DSMCCSection : public PrivateSection {
	protected:
		unsigned short privateDataLength;
		char* privateDataByte;

		vector<MpegDescriptor*>* dsmccDescritorList;
		unsigned int checksum;

		int processSectionPayload();
		int calculateSectionSize();

		void clearDsmccDescritor();
		void deleteDescriptor(MpegDescriptor* desc);

		/*
		 * if (tableId == 3D) {
		 *     tableIdExtension = dataEventId & eventMsgGroupId
		 * }
		 */
	public:
		DSMCCSection();
		virtual ~DSMCCSection();

		int updateStream();

		vector<MpegDescriptor*>* getDsmccDescritorList();
		unsigned int getChecksum();
		void setChecksum(unsigned int cs);
		int getPrivateDataByte(char** dataStream);
		int setPrivateDataByte(char* data, unsigned short length);

		void addDsmccDescriptor(MpegDescriptor* d);
		void removeDsmccDescriptor(unsigned char descriptorTag);
};

}
}
}
}
}

#endif /* DSMCCSECTION_H_ */
