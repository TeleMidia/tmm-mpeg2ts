/*
 * ApplicationSignalling.h
 *
 *  Created on: 01/02/2018
 *      Author: Felippe Nagato
 */

#ifndef INCLUDE_SI_DESCRIPTOR_APPLICATIONSIGNALLING_H_
#define INCLUDE_SI_DESCRIPTOR_APPLICATIONSIGNALLING_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class ApplicationSignalling : public MpegDescriptor {

	private:
		unsigned short applicationType;
		unsigned char AITVersionNumber;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		ApplicationSignalling();
		~ApplicationSignalling();

		void setApplicationType(unsigned short type);
		unsigned short getApplicationType();
		void setAITVersionNumber(unsigned char ver);
		unsigned char getAITVersionNumber();
};

}
}
}
}

#endif /* INCLUDE_SI_DESCRIPTOR_APPLICATIONSIGNALLING_H_ */
