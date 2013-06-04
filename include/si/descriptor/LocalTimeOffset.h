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
