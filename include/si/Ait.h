/*
 * Ait.h
 *
 *  Created on: 18/03/2013
 *      Author: Felippe Nagato
 */

#ifndef AIT_H_
#define AIT_H_

#include "PrivateSection.h"
#include "si/descriptor/Application.h"
#include "si/descriptor/ApplicationName.h"
#include "si/descriptor/TransportProtocol.h"
#include "si/descriptor/GingaApplication.h"
#include "si/descriptor/GingaApplicationLocation.h"
#include "Crc32.h"

#include <vector>
#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct AppInformation {
	 unsigned int organizationId;
	 unsigned short applicationId;
	 unsigned char applicationControlCode;
	 unsigned short descriptorLoopLength;
	 vector<MpegDescriptor*>* appDescriptorList;
};

class Ait : public PrivateSection {

	#define	CC_AUTO_START 	0x01
	#define	CC_PRESENT 		0x02
	#define	CC_DESTROY 		0x03
	#define	CC_KILL 		0x04
	#define	CC_REMOTE 		0x06
	#define	CC_UNBOUND 		0x07

	#define AT_GINGA_J		0x0001
	#define AT_DVB_HTML		0x0001
	#define AT_ACAP_J		0x0006
	#define AT_ARIB_BML		0x0007
	#define AT_GINGA_NCL	0x0009

	private:
		unsigned short commonDescriptorLength;
		vector<MpegDescriptor*> descriptorList;
		unsigned short applicationLoopLength;
		vector<AppInformation*> appInformationList;

	protected:
		int processSectionPayload();
		int calculateSectionSize();
		int createAndAddDescriptor(unsigned char descriptorTag,
				                   unsigned short descriptorSize, char* buffer,
				                   vector<MpegDescriptor*>* descList);

	public:
		Ait();
		virtual ~Ait();

		int updateStream();

		unsigned short getCommonDescriptorLength();
		vector<MpegDescriptor*>* getDescriptorList();
		unsigned short getApplicationLoopLength();
		vector<AppInformation*>* getAppInformationList();
		void addCommonDescriptor(MpegDescriptor *d);
		void addApplicationInfo(unsigned int organizationId,
								unsigned short applicationId,
								unsigned char applicationControlCode,
								vector<MpegDescriptor*>* appDescriptorList);

};

}
}
}
}


#endif /* AIT_H_ */
