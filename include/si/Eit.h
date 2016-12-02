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
 * Eit.h
 *
 *  Created on: 24/07/2013
 *      Author: Felippe Nagato
 */

#ifndef EIT_H_
#define EIT_H_

#include "PrivateSection.h"
#include "si/descriptor/ShortEvent.h"
#include "si/descriptor/Component.h"
#include "si/descriptor/ParentalRating.h"
#include "si/Tot.h"
#include "Crc32.h"

#include <map>
#include <ctime>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct EventInfo {
	unsigned short eventId;
	time_t startTime;
	time_t duration;
	unsigned char runningStatus;
	bool freeCaMode;
	unsigned short descriptorsLoopLength;
	vector<MpegDescriptor*> descriptorsList;
};

class Eit : public PrivateSection {

	#define RS_UNDEFINED				0
	#define RS_NOT_RUNNING				1
	#define RS_STARTS_IN_A_FEW_SECONDS	2
	#define RS_PAUSING					3
	#define RS_RUNNING					4

	private:

	protected:
		unsigned short transportStreamId;
		unsigned short originalNetworkId;
		unsigned char segmentLastSectionNumber;
		unsigned char lastTableId;
		bool destroyEvents;

		map<unsigned short, EventInfo*> eventList;

		int processSectionPayload();
		int calculateSectionSize();

	public:
		Eit();
		virtual ~Eit();

		int updateStream();

		void setTransportStreamId(unsigned short id);
		unsigned short getTransportStreamId();
		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();
		void setSegmentLastSectionNumber(unsigned char slsn);
		unsigned char getSegmentLastSectionNumber();
		void setLastTableId(unsigned char ltid);
		unsigned char getLastTableId();

		map<unsigned short, EventInfo*>* getEventList();
		bool hasEvent(unsigned short eventId);
		bool addEventInfo(EventInfo *event);
		void releaseAllEvents();
		void setDestroyEvents(bool destroy);

};

}
}
}
}


#endif /* EIT_H_ */
