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

#include "dsmcc/descriptor/StreamMode.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

StreamMode::StreamMode() : MpegDescriptor(0x19) {
	streamMode = 0;
	descriptorLength = 2;
}

StreamMode::~StreamMode() {

}

int StreamMode::process() {
	int pos = MpegDescriptor::process();
	streamMode = stream[pos++] & 0xFF;
	return 0;
}

int StreamMode::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = streamMode & 0xFF;
	stream[pos++] = 0xFF;

	return pos;
}

unsigned int StreamMode::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();
	return pos + 2;
}

unsigned char StreamMode::getStreamMode() {
	return streamMode;
}

void StreamMode::setStreamMode(unsigned char mode) {
	streamMode = mode;
}

}
}
}
}
}
