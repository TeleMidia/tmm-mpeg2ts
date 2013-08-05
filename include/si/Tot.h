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
		time_t dateTime;
		vector<MpegDescriptor*> descriptorList;
		unsigned short descriptorsLength;

	protected:
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
