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

#ifndef PRIVATESECTION_H_
#define PRIVATESECTION_H_

#include <iostream>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class PrivateSection {
   public:
	   static const unsigned short MAX_SECTION_SIZE = 4096;

   protected:
	   unsigned char tableId;
	   unsigned char sectionSyntaxIndicator;
	   unsigned char privateIndicator;
	   unsigned short privateSectionLength;

		/*
		 * transport_stream_id for PAT & SDT
		 * network_id for NIT
		 * program_number for PMT
		 * table_id_extension for DSM_CC Section
		 * service_id for EIT
		 * application_type for AIT
		 */
	   unsigned short tableIdExtension;
	   unsigned char versionNumber;
	   unsigned char currentNextIndicator;
	   unsigned char sectionNumber;
	   unsigned char lastSectionNumber;
	   char* stream;
	   unsigned int crc32;

	   unsigned int currentSize;

	   virtual int processSectionPayload();
	   virtual int calculateSectionSize();

   public:
	   PrivateSection();
	   virtual ~PrivateSection();

	   int addData(char* data, unsigned int length);

	   unsigned char isConsolidated();

	   unsigned char getTableId();
	   void setTableId(unsigned char id);
	   unsigned char getSectionSyntaxIndicator();
	   void setSectionSyntaxIndicator(unsigned char syntax);
	   unsigned char getPrivateIndicator();
	   void setPrivateIndicator(unsigned char pi);
	   unsigned short getPrivateSectionLength();
	   unsigned short getTableIdExtension();
	   void setTableIdExtension(unsigned short id);
	   unsigned char getVersionNumber();
	   void setVersionNumber(unsigned char ver);
	   unsigned char getCurrentNextIndicator();
	   void setCurrentNextIndicator(unsigned char ni);
	   unsigned char getSectionNumber();
	   void setSectionNumber(unsigned char sec);
	   unsigned char getLastSectionNumber();
	   void setLastSectionNumber(unsigned char last);
	   unsigned int getCrc32();
	   void setCrc32(unsigned int crc);

	   int getSectionPayload(char** dataStream);
	   int getSectionPayloadSize();

	   virtual int updateStream();
	   int getStream(char** dataStream);
};

}
}
}
}

#endif /* PRIVATESECTION_H_ */
