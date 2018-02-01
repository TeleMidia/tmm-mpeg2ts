/*
 * DataComponent.h
 *
 *  Created on: 31/01/2018
 *      Author: Felippe Nagato
 */

#ifndef INCLUDE_SI_DESCRIPTOR_DATACOMPONENT_H_
#define INCLUDE_SI_DESCRIPTOR_DATACOMPONENT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class DataComponent : public MpegDescriptor {

	//ABNT NBR 15608-2
	#define	DC_GINGA_XML_BASE_MEDIA_CODING 	0x0007
	#define	DC_CLOSED_CAPTION				0x0012
	#define	DC_SBTVD_PROGRAM_INDEX_CODING	0x00A4
	#define	DC_GINGA_APP_EXECUTING_ENGINE	0x00A0
	#define	DC_GINGA_APP_INFORMATION_TABLE	0x00A3

	#define	TF_DATA_CAROUSEL_EVENT_MESSAGE	0x00
	#define	TF_DATA_CAROUSEL_STORAGE		0x01
	#define	TF_OBJECT_CAROUSEL				0x02

	private:
		unsigned short dataComponentId;

		unsigned short applicationType;
		unsigned char AITVersionNumber;

		unsigned char transmissionFormat;
		unsigned char applicationIdentifierFlag;
		unsigned char documentResolution;
		unsigned int organizationId;
		unsigned short applicationId;
		unsigned int carouselId;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		DataComponent();
		~DataComponent();

		void setDataComponentId(unsigned short id);
		unsigned short getDataComponentId();
		void setApplicationType(unsigned short type);
		unsigned short getApplicationType();
		void setAITVersionNumber(unsigned char ver);
		unsigned char getAITVersionNumber();
		void setTransmissionFormat(unsigned char format);
		unsigned char getTransmissionFormat();
		void setApplicationIdentifierFlag(unsigned char flag);
		unsigned char getApplicationIdentifierFlag();
		void setDocumentResolution(unsigned char dr);
		unsigned char getDocumentResolution();
		void setOrganizationId(unsigned int id);
		unsigned int getOrganizationId();
		void setApplicationId(unsigned short id);
		unsigned short getApplicationId();
		void setCarouselId(unsigned int id);
		unsigned int getCarouselId();
};

}
}
}
}

#endif /* INCLUDE_SI_DESCRIPTOR_DATACOMPONENT_H_ */
