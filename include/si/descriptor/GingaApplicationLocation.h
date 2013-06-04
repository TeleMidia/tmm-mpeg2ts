/*
 * GingaApplicationLocation.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef GINGAAPPLICATIONLOCATION_H_
#define GINGAAPPLICATIONLOCATION_H_

#include "MpegDescriptor.h"
#include <cstring>
#include <string>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class GingaApplicationLocation : public MpegDescriptor {
	private:

	protected:
		string baseDirectory;
		string classExtension;
		string initialClass;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		GingaApplicationLocation(unsigned char descriptorTag);
		~GingaApplicationLocation();

		void setBaseDirectory(string dir);
		string getBaseDirectory();
		void setClassExtension(string ext);
		string getClassExtension();
		void setInitialClass(string init);
		string getInitialClass();

};

}
}
}
}


#endif /* GINGAAPPLICATIONLOCATION_H_ */
