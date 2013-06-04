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
		 * transport_stream_id for PAT
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
