/*
 * SystemManagement.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef SYSTEMMANAGEMENT_H_
#define SYSTEMMANAGEMENT_H_

#include "MpegDescriptor.h"
#include <map>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class SystemManagement : public MpegDescriptor {

	#define SM_BROADCASTING 0x00;
	#define SM_NON_BROADCASTING1 0x01;
	#define SM_NON_BROADCASTING2 0x02;
	#define SM_UNDEFINED 0x03;

	#define ST_UNDEFINED 0x00;
	#define ST_ISDB_SYSTEM 0x03;

	private:
		unsigned char broadcastingFlag;
		unsigned char broadcastingIdentifier;
		unsigned char additionalBroadcastingId;

		unsigned char additionalIdentificationInfoLength;
		char* additionalIdentificationInfo;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		SystemManagement();
		~SystemManagement();

		void setBroadcastingFlag(unsigned char bf);
		unsigned char getBroadcastingFlag();
		void setBroadcastingIdentifier(unsigned char bi);
		unsigned char getBroadcastingIdentifier();
		void setAdditionalBroadcastingId(unsigned char abi);
		unsigned char getAdditionalBroadcastingId();

		void setAdditionalIdentificationInfo(char* stream, unsigned char len);
		unsigned char getAdditionalIdentificationInfo(char** stream);
};

}
}
}
}


#endif /* SYSTEMMANAGEMENT_H_ */
