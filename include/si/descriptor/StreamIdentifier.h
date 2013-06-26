/*
 * StreamIdentifier.h
 *
 *  Created on: 25/06/2013
 *      Author: Felippe Nagato
 */

#ifndef STREAMIDENTIFIER_H_
#define STREAMIDENTIFIER_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class StreamIdentifier : public MpegDescriptor {

	private:
		unsigned char componentTag;
	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		StreamIdentifier();
		~StreamIdentifier();

		unsigned char getComponentTag();
		void setComponentTag(unsigned char ct);
};

}
}
}
}


#endif /* STREAMIDENTIFIER_H_ */
