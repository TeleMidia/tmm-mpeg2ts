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

#ifndef MPEGDefs_H_
#define MPEGDefs_H_

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

	// specified packet data
	static const unsigned char TS_PACKET_SIZE      = 188;
	static const unsigned char TS_PAYLOAD_SIZE     = 184;
	static const unsigned char TS_PACKET_SYNC_BYTE = 0x47;

	// specified packet ids
	static const short PAT_PID  = 0x0000;
	static const short CAT_PID  = 0x0001;
	static const short TSDT_PID = 0x0001; //TS Description Table
	static const short NIT_PID  = 0x0010;
	static const short SDT_PID  = 0x0011;
	static const short EIT_PID  = 0x0012;
	static const short CDT_PID  = 0x0029;

	// specified section ids
	static const unsigned char PAT_TID  = 0x00;
	static const unsigned char CAT_TID  = 0x01;
	static const unsigned char PMT_TID  = 0x02;
	static const unsigned char TSDT_TID = 0x03; //TS Description Section
	static const unsigned char OCI_TID  = 0x3B; //Object Carousel Info
	static const unsigned char OCD_TID  = 0x3C; //Object Carousel Data
	static const unsigned char DDE_TID  = 0x3D; //DSM-CC Descriptors
	static const unsigned char NIT_TID  = 0x40;
	static const unsigned char SDT_TID  = 0x42;
	static const unsigned char EIT_TID  = 0x4E;
	static const unsigned char CDT_TID  = 0xC8;
	static const unsigned char AIT_TID  = 0x74;
	static const unsigned char TOT_TID  = 0x73;

	// specified stream types
	static const unsigned char STREAM_TYPE_VIDEO_MPEG1     = 0x01;
	static const unsigned char STREAM_TYPE_VIDEO_MPEG2     = 0x02;
	static const unsigned char STREAM_TYPE_AUDIO_MPEG1     = 0x03;
	static const unsigned char STREAM_TYPE_AUDIO_MPEG2     = 0x04;
	static const unsigned char STREAM_TYPE_PRIVATE_SECTION = 0x05;
	static const unsigned char STREAM_TYPE_PRIVATE_DATA    = 0x06;
	static const unsigned char STREAM_TYPE_MHEG            = 0x07;
	static const unsigned char STREAM_TYPE_DSMCC_TYPE_A    = 0x0A;
	static const unsigned char STREAM_TYPE_DSMCC_TYPE_B    = 0x0B;
	static const unsigned char STREAM_TYPE_DSMCC_TYPE_C    = 0x0C;
	static const unsigned char STREAM_TYPE_DSMCC_TYPE_D    = 0x0D;
	static const unsigned char STREAM_TYPE_AUDIO_AAC       = 0x0F;
	static const unsigned char STREAM_TYPE_VIDEO_MPEG4     = 0x10;
	static const unsigned char STREAM_TYPE_AUDIO_MPEG4     = 0x11;
	static const unsigned char STREAM_TYPE_VIDEO_H264      = 0x1B;
	static const unsigned char STREAM_TYPE_AUDIO_AC3       = 0x81;
	static const unsigned char STREAM_TYPE_AUDIO_DTS       = 0x8A;

  class MPEGDefs {
	public:
		static unsigned char getTableIdFromStreamType(unsigned char streamType) {
			switch (streamType) {
				case STREAM_TYPE_DSMCC_TYPE_B:
					return OCI_TID;

				case STREAM_TYPE_DSMCC_TYPE_C:
					return OCD_TID;

				case STREAM_TYPE_DSMCC_TYPE_D:
					return DDE_TID;

				case STREAM_TYPE_PRIVATE_SECTION:
					return AIT_TID;

				default:
					return -1;
			}
		};

		static unsigned char getStreamTypeFromTableId(unsigned char tid) {
			switch (tid) {
				case OCI_TID:
					return STREAM_TYPE_DSMCC_TYPE_B;

				case OCD_TID:
					return STREAM_TYPE_DSMCC_TYPE_C;

				case DDE_TID:
					return STREAM_TYPE_DSMCC_TYPE_D;

				case AIT_TID:
					return STREAM_TYPE_PRIVATE_SECTION;

				default:
					return -1;
			}
		};
	 };
}
}
}
}

#endif /*MPEGDefs_H_*/
