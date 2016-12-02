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

#ifndef TSPACKET_H_
#define TSPACKET_H_

#include "TSAdaptationField.h"

#include <string.h>
#include <iostream>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class TSPacket {
	public:
		static const unsigned short TS_PACKET_SIZE_204 = 204;
		static const unsigned short TS_PACKET_SIZE = 188;
		static const unsigned short TS_PAYLOAD_SIZE = 184;
		static const unsigned short TS_PACKET_SYNC_BYTE = 0x47;

		/* 00 = ISO future use (0)
		 * 01 = no adaptation field (payload only) (1)
		 * 10 = adaptation field only (no payload) (2)
		 * 11 = adaptation field followed by payload (3)
		 */
		static const unsigned int FUTURE_USE = 0;
		static const unsigned int PAYLOAD_ONLY = 1;
		static const unsigned int NO_PAYLOAD = 2;
		static const unsigned int ADAPT_PAYLOAD = 3;

	private:

		int processHeader();
		int updateStream();

	protected:
		// ISO/IEC 13818-1 TS PACKET HEADER
		unsigned char syncByte;
		char transportErrorIndication;
		char payloadUnitStartIndicator;
		char transportPriority;
		unsigned short pid;

		/* 00 = Not scrambled
		 * 01 = User Defined
		 * 10 = User Defined
		 * 11 = User Defined
		 */
		unsigned char transportScramblingControl;

		unsigned char adaptationFieldControl;
		unsigned char continuityCounter;
		TSAdaptationField* tsaf;

		char* stream;
		char* payload;
		unsigned int payloadSize;
		char* payload2;
		unsigned int payloadSize2;
		char sectionType;

	public:
		TSPacket();
		TSPacket(char sectionType, char* payload, unsigned char payloadSize,
				TSAdaptationField* tsaf);
		TSPacket(char* packet);
		~TSPacket();

		int process();
		unsigned short getPid();
		void setPid(unsigned short pid);
		int getPayload(char** dataStream);
		unsigned int getPayloadSize();
		int getPayload2(char** dataStream);
		unsigned int getPayloadSize2();
		unsigned char getNumberOfPayloads();
		char getStartIndicator();
		void setStartIndicator(char flag);
		unsigned char getAdaptationFieldControl();
		void setAdaptationFieldControl(unsigned char afc);
		unsigned char getContinuityCounter();
		void setContinuityCounter(unsigned char counter);
		TSAdaptationField* getAdaptationField();
		void setAdaptationField(TSAdaptationField* tsaf);
		bool getSectionType();
		void setSectionType(char type);
		int getStream(char** stream); //returns payload length
		int getPayloadLengthAfterUpdate();

};
}
}
}
}


#endif /* TSPACKET_H_ */
