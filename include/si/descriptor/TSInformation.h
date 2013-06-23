/*
 * TSInformation.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef TSINFORMATION_H_
#define TSINFORMATION_H_

#include "MpegDescriptor.h"
#include <set>
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct TransmissionType {
	 unsigned char transmissionTypeInfo;
	 unsigned char numOfService;
	 set<unsigned short> serviceIdList;
};

class TSInformation : public MpegDescriptor {

	private:
		unsigned char remoteControlKeyId; // Virtual channel
		string tsName;
		set<TransmissionType*> transmissionTypeList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		TSInformation();
		~TSInformation();

		void setRemoteControlKeyId(unsigned char key);
		unsigned char getRemoteControlKeyId();
		unsigned char getTransmissionTypeCount();
		void setTsName(string name);
		string getTsName();
		void addTransmissionTypeList(TransmissionType* tt);
		set<TransmissionType*>* getTransmissionTypeList();
};

}
}
}
}


#endif /* TSINFORMATION_H_ */
