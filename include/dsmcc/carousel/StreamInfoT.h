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

/*
 * StreamInfoT.h
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#ifndef STREAMINFOT_H_
#define STREAMINFOT_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	class StreamInfoT {

		typedef struct {
			int aSeconds;
			int aMicroSeconds;
		} Duration;

		private:
			void process();
			int updateStream();

		protected:
			unsigned char descriptionLength;
			char* descriptionBytes;
			Duration duration;
			unsigned char audio;
			unsigned char video;
			unsigned char data;
			char* stream;
		public:
			StreamInfoT();
			StreamInfoT(char* dataStream, unsigned char length);
			~StreamInfoT();

			int getStreamLength();
			unsigned char getDescriptionLength();
			int getDescriptionBytes(char* data);
			int getDurationSeconds();
			int getDurationMicroSeconds();
			unsigned char getAudio();
			unsigned char getVideo();
			unsigned char getData();

			int setDescriptionBytes(char* data, unsigned char length);
			void setDurationSeconds(int sec);
			void setDurationMicroSeconds(int microSec);
			void setAudio(unsigned char a);
			void setVideo(unsigned char v);
			void setData(unsigned char d);

			int getStream(char* dataStream);
			int getStream(char** dataStream);

	 };
}
}
}
}
}

#endif /* STREAMINFOT_H_ */
