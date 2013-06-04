/*
 * GingaApplication.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef GINGAAPPLICATION_H_
#define GINGAAPPLICATION_H_

#include "MpegDescriptor.h"
#include <cstring>
#include <string>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class GingaApplication : public MpegDescriptor {
	private:

	protected:
		vector<string> parameterList;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		GingaApplication(unsigned char descriptorTag);
		~GingaApplication();

		void addParameter(string param);
		vector<string>* getParameterList();

};

}
}
}
}


#endif /* GINGAAPPLICATION_H_ */
