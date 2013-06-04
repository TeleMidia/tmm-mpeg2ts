/*
 * Application.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "MpegDescriptor.h"
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct AppProfile {
	 unsigned short applicationProfile;
	 unsigned char versionMajor;
	 unsigned char versionMinor;
	 unsigned char versionMicro;
};

class Application : public MpegDescriptor {

	private:

	protected:
		unsigned char applicationProfilesLength;
		bool serviceBoundFlag;
		unsigned char visibility;
		unsigned char applicationPriority;
		char* transportProtocolLabel;
		unsigned char transportProtocolLabelLength;
		vector<AppProfile*> appProfileList;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Application();
		~Application();

		void setApplicationProfilesLength(unsigned char len);
		unsigned char getApplicationProfilesLength();
		void setServiceBoundFlag(bool flag);
		bool getServiceBoundFlag();
		void setVisibility(unsigned char v);
		unsigned char getVisibility();
		void setApplicationPriority(unsigned char priority);
		unsigned char getApplicationPriority();
		void setTransportProtocolLabel(char* label, unsigned char len);
		unsigned char getTransportProtocolLabel(char** protocol);
		vector<AppProfile*>* getAppProfileList();
		void addAppProfile(unsigned short applicationProfile,
						   unsigned char versionMajor,
						   unsigned char versionMinor,
						   unsigned char versionMicro);

};

}
}
}
}


#endif /* APPLICATION_H_ */
