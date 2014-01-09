/*
 * HierarchicalTransmission.h
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#ifndef HIERARCHICALTRANSMISSION_H_
#define HIERARCHICALTRANSMISSION_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class HierarchicalTransmission : public MpegDescriptor {
	private:
		unsigned char qualityLevel;
		unsigned short referencePID;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		HierarchicalTransmission();
		~HierarchicalTransmission();

		void setQualityLevel(unsigned char ql);
		unsigned char getQualityLevel();
		void setReferencePID(unsigned short pid);
		unsigned short getReferencePID();
};

}
}
}
}


#endif /* HIERARCHICALTRANSMISSION_H_ */
