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
