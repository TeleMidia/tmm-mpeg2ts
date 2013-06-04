/*
 * ApplicationName.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef APPLICATIONNAME_H_
#define APPLICATIONNAME_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct AppName {
	 string ISO639LanguageCode;
	 unsigned char appNameLength;
	 string name;
};

class ApplicationName : public MpegDescriptor {
	private:

	protected:
		vector<AppName*> appNameList;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		ApplicationName();
		~ApplicationName();

		void setAppName(string ISO639LanguageCode, string name);
		vector<AppName*>* getAppNameList();

};

}
}
}
}


#endif /* APPLICATIONNAME_H_ */
