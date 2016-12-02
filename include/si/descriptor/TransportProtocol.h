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
 * TransportProtocol.h
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#ifndef TRANSPORTPROTOCOL_H_
#define TRANSPORTPROTOCOL_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct ObjectCarouselProtocol {
	 bool remoteConnection;
	 unsigned short originalNetworkId;
	 unsigned short transportStreamId;
	 unsigned short serviceId;
	 unsigned char componentTag;
};

class TransportProtocol : public MpegDescriptor {

	#define	P_OBJECT_CAROUSEL 	0x01
	#define	P_IP_DVB 			0x02
	#define	P_DATA_PIPING 		0x03
	#define	P_DATA_CAROUSEL		0x04

	private:

	protected:
		unsigned short protocolId;
		unsigned char transportProtocolLabel;
		ObjectCarouselProtocol* ocProtocol;

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		TransportProtocol();
		~TransportProtocol();

		void setProtocolId(unsigned short id);
		unsigned short getProtocolId();
		void setTransportProtocolLabel(unsigned char pl);
		unsigned char getTransportProtocolLabel();
		void setOcProtocol(ObjectCarouselProtocol* ocp);
		ObjectCarouselProtocol* getOcProtocol();

};

}
}
}
}


#endif /* TRANSPORTPROTOCOL_H_ */
