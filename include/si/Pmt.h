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

#ifndef PMT_H_
#define PMT_H_

#include "PrivateSection.h"
#include "Crc32.h"

#include <map>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Pmt : public PrivateSection {
	protected:
		//programNumber = tableIdExtension
		unsigned short pcrPid;
		unsigned short programInfoLength;
		map<char*, unsigned short>* programInfoList;
		map<unsigned short, unsigned char>* esList;
		map<unsigned short, map<char*, unsigned short>*>* esDescriptorList;

		int calculateSectionSize();
		void clearEsDescriptorList();

	public:
		static const unsigned char STREAM_TYPE_ISO_RESERVED = 0x00;
		static const unsigned char STREAM_TYPE_VIDEO_MPEG1 = 0x01;
		static const unsigned char STREAM_TYPE_VIDEO_MPEG2 = 0x02;
		static const unsigned char STREAM_TYPE_AUDIO_MPEG1 = 0x03;
		static const unsigned char STREAM_TYPE_AUDIO_MPEG2 = 0x04;
		static const unsigned char STREAM_TYPE_PRIVATE_SECTION = 0x05;
		static const unsigned char STREAM_TYPE_PRIVATE_DATA = 0x06;
		static const unsigned char STREAM_TYPE_MHEG = 0x07;
		static const unsigned char STREAM_TYPE_MPEG2_DSMCC = 0x08;
		static const unsigned char STREAM_TYPE_H222_1 = 0x09;
		static const unsigned char STREAM_TYPE_DSMCC_TYPE_A = 0x0A;
		static const unsigned char STREAM_TYPE_DSMCC_TYPE_B = 0x0B;
		static const unsigned char STREAM_TYPE_DSMCC_TYPE_C = 0x0C;
		static const unsigned char STREAM_TYPE_DSMCC_TYPE_D = 0x0D;
		static const unsigned char STREAM_TYPE_MPEG2_AUX = 0x0E;
		static const unsigned char STREAM_TYPE_AUDIO_AAC = 0x0F;
		static const unsigned char STREAM_TYPE_VIDEO_MPEG4 = 0x10;
		static const unsigned char STREAM_TYPE_AUDIO_MPEG4 = 0x11;
		static const unsigned char STREAM_TYPE_SL_PES = 0x12;
		static const unsigned char STREAM_TYPE_SL_SECTION = 0x13;
		static const unsigned char STREAM_TYPE_DSMCC_SYNCH = 0x14;
		static const unsigned char STREAM_TYPE_META_PES = 0x15;
		static const unsigned char STREAM_TYPE_META_SEC = 0x16;
		static const unsigned char STREAM_TYPE_META_DATA_CAROUSEL = 0x17;
		static const unsigned char STREAM_TYPE_META_OBJECT_CAROUSEL = 0x18;
		static const unsigned char STREAM_TYPE_META_DSMCC_SYNCH = 0x19;
		static const unsigned char STREAM_TYPE_MPEG2_IPMP_STREAM = 0x1A;
		static const unsigned char STREAM_TYPE_VIDEO_H264 = 0x1B;
		static const unsigned char STREAM_TYPE_IPMP_STREAM = 0x7F;
		static const unsigned char STREAM_TYPE_AUDIO_AC3 = 0x81;
		static const unsigned char STREAM_TYPE_AUDIO_DTS = 0x8A;

		Pmt();
		virtual ~Pmt();

		int updateStream();

		static string getStreamTypeName(unsigned char streamType);

		int processSectionPayload();

		unsigned short getProgramNumber();
		void setProgramNumber(unsigned short pnum);

		unsigned short getPCRPid();
		void setPCRPid(unsigned short pid);

		map<char*, unsigned short>* getProgramInfoList();
		void addInfo(char* data, unsigned short length);
		void removeInfoList();

		map<unsigned short, unsigned char>* getEsList();
		map<char*, unsigned short>* getEsDescriptorList(unsigned short pid);

		void addEs(unsigned char streamType, unsigned short pid);
		void removeEs(unsigned short pid);
		int addEsDescriptor(
				unsigned short pid, char* data, unsigned char length);
		void removeEsDescriptorList(unsigned short pid);
};

}
}
}
}

#endif /* PMT_H_ */
