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

#ifndef PESPACKET_H_
#define PESPACKET_H_

#include <iostream>
#include <cstring>
#include <vector>
#include <inttypes.h>
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
		static bool setDataAlignmentIndicatorDirectStream(char* stream, bool ind);
		static bool setStreamIdDirectStream(char* stream, unsigned char id);

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
