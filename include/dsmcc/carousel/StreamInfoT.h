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
