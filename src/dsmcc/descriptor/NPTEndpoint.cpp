/*
 * NPTEndpoint.cpp
 *
 *  Created on: Oct 22, 2012
 *      Author: Felippe Nagato
 */

#include "dsmcc/descriptor/NPTEndpoint.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

NPTEndpoint::NPTEndpoint() : MpegDescriptor(0x02) {
	descriptorLength = 14;
	startNPT = 0;
	stopNPT = 0;
}

NPTEndpoint::~NPTEndpoint() {

}

int NPTEndpoint::process() {
	int pos = MpegDescriptor::process();

	pos = pos + 1;

	startNPT = 0;
	startNPT = (stream[pos++] & 0x01);
	startNPT = startNPT << 8;
	startNPT = startNPT | (stream[pos++] & 0xFF);
	startNPT = startNPT << 8;
	startNPT = startNPT | (stream[pos++] & 0xFF);
	startNPT = startNPT << 8;
	startNPT = startNPT | (stream[pos++] & 0xFF);
	startNPT = startNPT << 8;
	startNPT = startNPT | (stream[pos++] & 0xFF);

	pos = pos + 3;

	stopNPT = 0;
	stopNPT = (stream[pos++] & 0x01);
	stopNPT = stopNPT << 8;
	stopNPT = stopNPT | (stream[pos++] & 0xFF);
	stopNPT = stopNPT << 8;
	stopNPT = stopNPT | (stream[pos++] & 0xFF);
	stopNPT = stopNPT << 8;
	stopNPT = stopNPT | (stream[pos++] & 0xFF);
	stopNPT = stopNPT << 8;
	stopNPT = stopNPT | (stream[pos++] & 0xFF);

	return pos;
}

int NPTEndpoint::updateStream() {
	MpegDescriptor::updateStream();

	stream[2] = 0xFF;
	stream[3] = 0xFE;

	stream[3] = stream[3] |	(((startNPT >> 16) >> 16) & 0x01);
	stream[4] = ((startNPT >> 16) >> 8) & 0xFF;
	stream[5] = (startNPT >> 16) & 0xFF;
	stream[6] = (startNPT >> 8) & 0xFF;
	stream[7] = startNPT & 0xFF;

	stream[8] = 0xFF;
	stream[9] = 0xFF;
	stream[10] = 0xFF;
	stream[11] = 0xFE;

	stream[11] = stream[11] | (((stopNPT >> 16) >> 16) & 0x01);
	stream[12] = ((stopNPT >> 16) >> 8) & 0xFF;
	stream[13] = (stopNPT >> 16) & 0xFF;
	stream[14] = (stopNPT >> 8) & 0xFF;
	stream[15] = stopNPT & 0xFF;

	return 16;
}

unsigned int NPTEndpoint::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();
	return pos + 14;
}

uint64_t NPTEndpoint::getStartNPT() {
	return startNPT;
}

uint64_t NPTEndpoint::getStopNPT() {
	return stopNPT;
}

void NPTEndpoint::setStartNPT(uint64_t npt) {
	startNPT = npt;
}

void NPTEndpoint::setStopNPT(uint64_t npt) {
	stopNPT = npt;
}

}
}
}
}
}
