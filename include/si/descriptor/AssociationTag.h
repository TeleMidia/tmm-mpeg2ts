/*
 * AssociationTag.h
 *
 *  Created on: 09/01/2014
 *      Author: Felippe Nagato
 */

#ifndef ASSOCIATIONTAG_H_
#define ASSOCIATIONTAG_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class AssociationTag : public MpegDescriptor {
	private:
		unsigned short associationTag;
		unsigned short use;
		unsigned char selectorLength;
		unsigned int transactionId;
		unsigned int timeout;
		char* selectorByte;
		int privateDataByteLength;
		char* privateDataByte;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		AssociationTag();
		~AssociationTag();

		void setAssociationTag(unsigned short at);
		unsigned short getAssociationTag();
		void setUse(unsigned short use);
		unsigned short getUse();
		unsigned char getSelectorLength();
		void setTransactionId(unsigned int id);
		unsigned int getTransactionId();
		void setTimeout(unsigned int t);
		unsigned int getTimeout();
		void setSelectorByte(char* stream, int length);
		int getSelectorByte(char** stream);
		void setPrivateDataByte(char* stream, int length);
		int getPrivateDataByte(char** stream);
};

}
}
}
}


#endif /* ASSOCIATIONTAG_H_ */
