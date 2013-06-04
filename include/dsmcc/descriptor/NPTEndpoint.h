/*
 * NPTEndpoint.h
 *
 *  Created on: Oct 22, 2012
 *      Author: Felippe Nagato
 */

#ifndef NPTENDPOINT_H_
#define NPTENDPOINT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

class NPTEndpoint : public MpegDescriptor {
	private:

	protected:
		uint64_t startNPT;
		uint64_t stopNPT;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		NPTEndpoint();
		~NPTEndpoint();

		uint64_t getStartNPT();
		uint64_t getStopNPT();
		void setStartNPT(uint64_t npt);
		void setStopNPT(uint64_t npt);

};

}
}
}
}
}

#endif /* NPTENDPOINT_H_ */
