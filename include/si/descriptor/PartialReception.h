/*
 * PartialReception.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef PARTIALRECEPTION_H_
#define PARTIALRECEPTION_H_

#include "MpegDescriptor.h"
#include <set>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class PartialReception : public MpegDescriptor {

	private:
		set<unsigned short> serviceIdList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		PartialReception();
		~PartialReception();

		void addServiceId(unsigned short id);
		set<unsigned short>* getServiceIdList();
};

}
}
}
}


#endif /* PARTIALRECEPTION_H_ */
