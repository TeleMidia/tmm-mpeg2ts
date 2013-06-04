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

#ifndef PESPACKET_H_
#define PESPACKET_H_

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct Buffer {
	 char* buffer;
	 unsigned int size;
};

class PESPacket {
	public:
		static const unsigned int START_CODE_PREFIX = 0x00000001;

	private:

	protected:
		vector<Buffer*> bufferList;
		char* stream;
		int currentSize;

		/* ISO/IEC 13818-1 PES PACKET HEADER */
		unsigned int startCodePrefix; /* 24 bits */
		unsigned char streamId; /* 8 bits */
		unsigned short packetLength; /* 16 bits */
		unsigned char scramblingControl; /* 2 bits */
		unsigned char priority; /* 1 bits */
		unsigned char dataAlignmentIndicator; /* 1 bits */
		unsigned char copyright; /* 1 bits */
		unsigned char original; /* 1 bits */
		unsigned char ptsDtsFlags; /* 2 bits */
		unsigned char escrFlag; /* 1 bits */
		unsigned char esRateFlag; /* 1 bits */
		unsigned char dsmTrickModeFlag; /* 1 bits */
		unsigned char additionalCopyInfoFlag; /* 1 bits */
		unsigned char crcFlag; /* 1 bits */
		unsigned char extensionFlag; /* 1 bits */
		unsigned char headerDataLength; /* 8 bits */
		uint64_t pts; /* 33 bits */
		uint64_t dts; /* 33 bits */
		uint64_t escrBase;
		uint64_t escrExtension;
		unsigned int esRate;

		/*
		unsigned char trickModeControl;
		unsigned char fieldId;
		unsigned char intraSliceRefresh;
		unsigned char frequencyTruncation;
		unsigned char repCntrl;
		unsigned char additionalCopyInfo;

		previous_PES_packet_CRC
		PES_private_data_flag
		pack_header_field_flag
		program_packet_sequence_counter_flag
		P-STD_buffer_flag
		PES_extension_flag_2
		PES_private_data
		pack_field_length
		pack_header()
		program_packet_sequence_counter
		MPEG1_MPEG2_identifier
		original_stuff_length
		P-STD_buffer_scale
		P-STD_buffer_size
		PES_extension_field_length
		stream_id_extension_flag
		stream_id_extension
		...
		*/

		void processHeader(char* dataStream, unsigned char length);
		void releaseBufferList();
		void mountStream();

	public:
		PESPacket();
		PESPacket(char* dataStream, unsigned char length);
		~PESPacket();

		void process(char* dataStream, unsigned char length);

		unsigned int getStartCodePrefix();
		unsigned int getStreamId();
		unsigned short getPacketLength();
		unsigned short getScramblingControl();
		bool getPriority();
		bool getDataAlignmentIndicator();
		bool getCopyright();
		bool getOriginal();
		unsigned char getPtsDtsFlags();
		bool getEscrFlag();
		bool getEsRateFlag();
		bool getDsmTrickModeFlag();
		bool getAdditionalCopyInfoFlag();
		bool getCrcFlag();
		bool getExtensionFlag();
		unsigned char getHeaderDataLength();
		uint64_t getPts();
		void setPts(uint64_t pts);
		uint64_t getDts();

		/* Must have an encoded stream with PTS or DTS to work */
		static bool setPtsDirectStream(char* stream, uint64_t pts);
		static bool setDtsDirectStream(char* stream, uint64_t dts);
		static int64_t getPtsDirectStream(char* stream);
		static int64_t getDtsDirectStream(char* stream);

		uint64_t getEscrBase();
		uint64_t getEscrExtension();
		unsigned int getEsRate();
		int getStream(char* dataStream);
		int getStream(char** dataStream);

		bool isConsolidated();

		void print();
};

}
}
}
}

#endif /* PESPACKET_H_ */
