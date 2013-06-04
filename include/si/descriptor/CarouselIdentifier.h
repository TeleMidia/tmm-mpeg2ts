/*
 * CarouselIdentifier.h
 *
 *  Created on: 03/06/2013
 *      Author: Felippe Nagato
 */

#ifndef CAROUSELIDENTIFIER_H_
#define CAROUSELIDENTIFIER_H_

#include "MpegDescriptor.h"
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class CarouselIdentifier : public MpegDescriptor {

	private:

	protected:
		unsigned int carouselId;
		char* privateDataByte;
		unsigned char privateDataByteLength;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		CarouselIdentifier();
		~CarouselIdentifier();

		unsigned int getCarouselId();
		void setCarouselId(unsigned int id);

};

}
}
}
}


#endif /* CAROUSELIDENTIFIER_H_ */
