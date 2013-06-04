/*
 * GingaApplication.cpp
 *
 *  Created on: 19/03/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/GingaApplication.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

GingaApplication::GingaApplication(unsigned char descriptorTag) {
	this->descriptorTag = descriptorTag;
	//Ginga-J 	= 0x03
	//Ginga-NCL = 0x06
}

GingaApplication::~GingaApplication() {

}

int GingaApplication::process() {
	unsigned char len = 0;
	string value;
	int pos = MpegDescriptor::process();

	while (len < descriptorLength) {
		len += stream[pos++];
		value.assign(stream + pos, stream[pos-1]);
		pos += stream[pos-1];
		parameterList.push_back(value);
	}

	return pos;
}

int GingaApplication::updateStream() {
	vector<string>::iterator it;
	int pos = MpegDescriptor::updateStream();

	it = parameterList.begin();
	while (it != parameterList.end()) {
		stream[pos++] = (*it).size();
		memcpy(stream + pos, (*it).c_str(), (*it).size());
		pos += (*it).size();
		++it;
	}

	return pos;
}

unsigned int GingaApplication::calculateDescriptorSize() {
	vector<string>::iterator it;
	int pos = MpegDescriptor::calculateDescriptorSize();

	it = parameterList.begin();
	while (it != parameterList.end()) {
		pos += (*it).size() + 1;
		++it;
	}
	return pos;
}

void GingaApplication::addParameter(string param) {
	parameterList.push_back(param);
}

vector<string>* GingaApplication::getParameterList() {
	return &parameterList;
}

}
}
}
}


