/******************************************************************************
TeleMídia Multiplexer MPEG-2 Transport Stream library
Copyright (C) 2016 TeleMídia Lab/PUC-Rio
https://github.com/TeleMidia/tmm-mpeg2ts/graphs/contributors

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
for more details. You should have received a copy of the GNU Affero General
Public License along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

/*
 * Service.h
 *
 *  Created on: 18/06/2013
 *      Author: Felippe Nagato
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Service : public MpegDescriptor {

	#define DIGITAL_TELEVISION_SERVICE 0x01
	#define DIGITAL_AUDIO_SERVICE 0x02
	#define TELETEXT_SERVICE 0x03
	#define NVOD_REFERENCE_SERVICE 0x04
	#define NVOD_TIME_SHIFTED_SERVICE 0x05
	#define MOSAIC_SERVICE 0x06
	#define ADVANCED_CODING_FOR_DIGITAL_RADIO_SERVICE 0x0A
	#define ADVANCED_CODING_FOR_MOSAIC_SERVICE 0x0B
	#define DATA_BROADCASTING_SERVICE 0x0C
	#define RCS_MAP 0x0E
	#define RCS_FLS 0x0F
	#define DIGITAL_MPEG2_HD_TELEVISION_SERVICE 0x11
	#define ADVANCED_CODING_FOR_DIGITAL_SD_TELEVISION_SERVICE 0x16
	#define ADVANCED_CODING_FOR_DIGITAL_NVOD_SD_TIME_SHIFTED_TELEVISION_SERVICE 0x17
	#define ADVANCED_CODING_FOR_DIGITAL_NVOD_SD_REFERENCE_TELEVISION_SERVICE 0x18
	#define ADVANCED_CODING_FOR_DIGITAL_NVOD_HD_TELEVISION_SERVICE 0x19
	#define ADVANCED_CODING_FOR_DIGITAL_NVOD_HD_TIME_SHIFTED_TELEVISION_SERVICE 0x1A
	#define ADVANCED_CODING_FOR_DIGITAL_NVOD_HD_REFERENCE_TELEVISION_SERVICE 0x1B
	#define SPECIAL_VIDEO_SERVICE 0xA1
	#define SPECIAL_AUDIO_SERVICE 0xA2
	#define SPECIAL_DATA_SERVICE 0xA3
	#define ENGINEERING_SERVICE 0xA4
	#define PROMOTIONAL_VIDEO_SERVICE 0xA5
	#define PROMOTIONAL_AUDIO_SERVICE 0xA6
	#define PROMOTIONAL_DATA_SERVICE 0xA7
	#define DATA_SERVICE_FOR_ANTICIPATED_STORAGE 0xA8
	#define EXCLUSIVE_DATA_SERVICE_FOR_STORAGE 0xA9
	#define BOOKMARK_SERVICE_LIST 0xAA
	#define SIMULTANEOUS_SERVER_TYPE_SERVICE 0xAB
	#define INDEPENDENT_FILE_SERVICE 0xAC
	#define DATA_SERVICE 0xC0

	private:
		unsigned char serviceType;
		string providerName;
		string serviceName;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Service();
		~Service();

		void setServiceType(unsigned char type);
		unsigned char getServiceType();
		void setProviderName(string name);
		string getProviderName();
		void setServiceName(string name);
		string getServiceName();
};

}
}
}
}

#endif /* SERVICE_H_ */
