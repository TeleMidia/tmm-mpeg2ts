/*
 * ServiceList.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef SERVICELIST_H_
#define SERVICELIST_H_

#include "MpegDescriptor.h"
#include <map>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class ServiceList : public MpegDescriptor {

	private:
		map<unsigned short, unsigned char> serviceList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		ServiceList();
		~ServiceList();

		void addService(unsigned short id, unsigned char type);
		map<unsigned short, unsigned char>* getServiceList();
};

}
}
}
}


#endif /* SERVICELIST_H_ */
