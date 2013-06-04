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

#ifndef STREAMEVENT_H_
#define STREAMEVENT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

class StreamEvent : public MpegDescriptor {
	private:
		unsigned char privateDataPayloadLength;

	protected:
		unsigned short eventId;
		uint64_t eventNPT;
		unsigned char privateDataLength;
		unsigned char commandTag;
		unsigned char sequenceNumber;
		unsigned char finalFlag;
		char* privateDataPayload;
		unsigned char fcs;

		int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		StreamEvent();
		StreamEvent(char* dataStream, int length);
		virtual ~StreamEvent();

		unsigned short getEventId();
		uint64_t getEventNPT();
		unsigned char getPrivateDataLength();
		unsigned char getCommandTag();
		unsigned char getSequenceNumber();
		unsigned char getFinalFlag();
		int getPrivateDataPayload(char** dataStream);
		unsigned char getFcs();

		void setEventId(unsigned short id);
		void setEventNPT(uint64_t npt);
		void setCommandTag(unsigned char tag);
		void setSequenceNumber(unsigned char seqNum);
		void setFinalFlag(unsigned char flag);
		int setPrivateDataPayload(char* data, unsigned char length);
		void setFcs(unsigned char fcs);

};

}
}
}
}
}


#endif /* STREAMEVENT_H_ */
