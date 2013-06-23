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
		unsigned short networkDescriptorsLength;
		vector<MpegDescriptor*> descriptorsList;
		unsigned short transportStreamLoopLength;
		vector<TransportInformation*> transportList;

	protected:
		int processSectionPayload();
		int calculateSectionSize();

	public:
		Nit();
		virtual ~Nit();

		int updateStream();

		void setTransportStreamId(unsigned short id);
		unsigned short getTransportStreamId();

		vector<MpegDescriptor*>* getDescriptorsList();
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
