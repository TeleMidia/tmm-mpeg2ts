/*
 * NetworkName.h
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#ifndef NETWORKNAME_H_
#define NETWORKNAME_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class NetworkName : public MpegDescriptor {

	private:
		string networkName;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		NetworkName();
		~NetworkName();

		void setNetworkName(string name);
		string getNetworkName();
};

}
}
}
}


#endif /* NETWORKNAME_H_ */
