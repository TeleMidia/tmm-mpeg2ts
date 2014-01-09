/*
 * DeferredAssociationTags.h
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#ifndef DEFERREDASSOCIATIONTAGS_H_
#define DEFERREDASSOCIATIONTAGS_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class DeferredAssociationTags : public MpegDescriptor {
	private:
		vector<unsigned short> associationTagList;
		unsigned short transportStreamId;
		unsigned short programNumber;
		unsigned short originalNetworkId;
		int privateDataByteLength;
		char* privateDataByte;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		DeferredAssociationTags();
		~DeferredAssociationTags();

		void addAssociationTag(unsigned short at);
		void setTransportStreamId(unsigned short id);
		unsigned short getTransportStreamId();
		void setProgramNumber(unsigned short pn);
		unsigned short getProgramNumber();
		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();
		void setPrivateDataByte(char* stream, int length);
		int getPrivateDataByte(char** stream);
};

}
}
}
}


#endif /* DEFERREDASSOCIATIONTAGS_H_ */
