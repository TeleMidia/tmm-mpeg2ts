/*
 * TerrestrialDeliverySystem.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef TERRESTRIALDELIVERYSYSTEM_H_
#define TERRESTRIALDELIVERYSYSTEM_H_

#include "MpegDescriptor.h"
#include <set>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class TerrestrialDeliverySystem : public MpegDescriptor {
	#define GUARD_INTERVAL_1_32 0x00
	#define GUARD_INTERVAL_1_16 0x01
	#define GUARD_INTERVAL_1_8  0x02
	#define GUARD_INTERVAL_1_4  0x03

	#define TRANSMISSION_MODE_1 0x00
	#define TRANSMISSION_MODE_2 0x01
	#define TRANSMISSION_MODE_3 0x02
	#define TRANSMISSION_MODE_UNDEFINED 0x03

	private:
		unsigned short areaCode;
		unsigned char guardInterval;
		unsigned char transmissionMode;
		set<unsigned short> frequencyList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		TerrestrialDeliverySystem();
		~TerrestrialDeliverySystem();

		void setAreaCode(unsigned short ac);
		unsigned short getAreaCode();
		void setGuardInterval(unsigned char gi);
		unsigned char getGuardInterval();
		void setTransmissionMode(unsigned char tm);
		unsigned char getTransmissionMode();

		void addFrequency(unsigned short freq);
		set<unsigned short>* getFrequencyList();
};

}
}
}
}


#endif /* TERRESTRIALDELIVERYSYSTEM_H_ */
