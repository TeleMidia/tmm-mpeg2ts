/*
 * Eit.h
 *
 *  Created on: 24/07/2013
 *      Author: Felippe Nagato
 */

#ifndef EIT_H_
#define EIT_H_

#include "PrivateSection.h"
#include "si/Descriptor/ShortEvent.h"
#include "si/Descriptor/Component.h"
#include "si/Descriptor/ParentalRating.h"
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

	private:
		unsigned short transportStreamId;
		unsigned short originalNetworkId;
		unsigned char segmentLastSectionNumber;
		unsigned char lastTableId;

		map<unsigned short, EventInfo*> eventList;

	protected:
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

};

}
}
}
}


#endif /* EIT_H_ */
