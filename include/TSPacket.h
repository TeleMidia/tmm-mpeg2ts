/******************************************************************************
Este arquivo eh parte da implementacao das ferramentas DTV do TeleMidia

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the TeleMidia DTV Tools

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
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
