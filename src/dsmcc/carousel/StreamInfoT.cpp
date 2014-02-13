/*
 * StreamInfoT.cpp
 *
 *  Created on: May 8, 2009
 *      Author: Felippe Nagato
 */

#include "dsmcc/carousel/StreamInfoT.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

	StreamInfoT::StreamInfoT() {
		descriptionLength = 0;
		descriptionBytes = NULL;
		stream = NULL;
		audio = 0;
		video = 0;
		data = 0;
		duration.aSeconds = 0;
		duration.aMicroSeconds = 0;
	}

	StreamInfoT::StreamInfoT(char* dataStream, unsigned char length) {
		descriptionBytes = NULL;
		try {
			stream = new char[length];
		} catch (...) {
			cout << "Error: StreamInfoT" << endl;
		}
		memcpy(stream, dataStream, length);
		process();
	}

	StreamInfoT::~StreamInfoT() {
		if (descriptionBytes != NULL) {
			delete (descriptionBytes);
		}
		if (stream != NULL) {
			delete (stream);
		}
	}

	void StreamInfoT::process() {
		//TODO: Decode stream
	}

	int StreamInfoT::updateStream() {
		int pos = 0;
		if (stream != NULL) {
			delete (stream);
		}
		try {
			stream = new char[descriptionLength + 12];
		} catch(...) {
			return -1;
		}
		stream[pos++] = descriptionLength;
		memcpy(stream + pos, descriptionBytes, descriptionLength);
		pos = pos + descriptionLength;
		stream[pos++] = (duration.aSeconds >> 24) & 0xFF;
		stream[pos++] = (duration.aSeconds >> 16) & 0xFF;
		stream[pos++] = (duration.aSeconds >> 8) & 0xFF;
		stream[pos++] = duration.aSeconds & 0xFF;
		stream[pos++] = (duration.aMicroSeconds >> 24) & 0xFF;
		stream[pos++] = (duration.aMicroSeconds >> 16) & 0xFF;
		stream[pos++] = (duration.aMicroSeconds >> 8) & 0xFF;
		stream[pos++] = duration.aMicroSeconds & 0xFF;
		stream[pos++] = audio & 0xFF;
		stream[pos++] = video & 0xFF;
		stream[pos++] = data & 0xFF;
		return pos;
	}

	int StreamInfoT::getStreamLength() {
		return descriptionLength + 12;
	}

	unsigned char StreamInfoT::getDescriptionLength() {
		return descriptionLength;
	}

	int StreamInfoT::getDescriptionBytes(char* data) {
		memcpy(data, descriptionBytes, descriptionLength);
		return descriptionLength;
	}

	int StreamInfoT::getDurationSeconds() {
		return duration.aSeconds;
	}

	int StreamInfoT::getDurationMicroSeconds() {
		return duration.aMicroSeconds;
	}

	unsigned char StreamInfoT::getAudio() {
		return audio;
	}

	unsigned char StreamInfoT::getVideo() {
		return video;
	}

	unsigned char StreamInfoT::getData() {
		return data;
	}

	int StreamInfoT::setDescriptionBytes(char* data, unsigned char length) {
		if (descriptionBytes != NULL) {
			delete (descriptionBytes);
		}
		try {
			descriptionBytes = new char[length];
		} catch(...) {
			return -1;
		}
		memcpy(descriptionBytes, data, length);
		descriptionLength = length;
		return descriptionLength;
	}

	void StreamInfoT::setDurationSeconds(int sec) {
		duration.aSeconds = sec;
	}

	void StreamInfoT::setDurationMicroSeconds(int microSec) {
		duration.aMicroSeconds = microSec;
	}

	void StreamInfoT::setAudio(unsigned char a) {
		audio = a;
	}

	void StreamInfoT::setVideo(unsigned char v) {
		video = v;
	}

	void StreamInfoT::setData(unsigned char d) {
		data = d;
	}

	int StreamInfoT::getStream(char* dataStream) {
		if (updateStream() >= 0) {
			memcpy(dataStream, stream, descriptionLength + 12);
			return descriptionLength + 12;
		} else {
			return -1;
		}
	}

	int StreamInfoT::getStream(char** dataStream) {
		if (updateStream() >= 0) {
			*dataStream = stream;
			return descriptionLength + 12;
		} else {
			return -1;
		}
	}

}
}
}
}
}
