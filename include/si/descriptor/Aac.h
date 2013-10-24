/*
 * Aac.h
 *
 *  Created on: 23/10/2013
 *      Author: Felippe Nagato
 */

#ifndef AAC_H_
#define AAC_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Aac : public MpegDescriptor {
	private:
		unsigned char profileAndLevel;
		bool aacTypeFlag;
		unsigned char aacType;
		char* aditionalInfo;
		unsigned char aditionalInfoLength;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Aac();
		~Aac();

		void setProfileAndLevel(unsigned char pl);
		unsigned char getProfileAndLevel();
		void setAacTypeFlag(bool flag);
		bool getAacTypeFlag();
		void setAacType(unsigned char type);
		unsigned char getAacType();
		void setAditionalInfo(char* dataStream, unsigned char length);
		unsigned char getAditionalInfo(char** dataStream);
};

}
}
}
}


#endif /* AAC_H_ */
