/*
 * DataStreamAlignment.h
 *
 *  Created on: 08/01/2014
 *      Author: Felippe Nagato
 */

#ifndef DATASTREAMALIGNMENT_H_
#define DATASTREAMALIGNMENT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class DataStreamAlignment : public MpegDescriptor {
	private:
		unsigned char alignmentType;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		DataStreamAlignment();
		~DataStreamAlignment();

		void setAlignmentType(unsigned char at);
		unsigned char getAlignmentType();
};

}
}
}
}

#endif /* DATASTREAMALIGNMENT_H_ */
