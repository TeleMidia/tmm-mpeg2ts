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
