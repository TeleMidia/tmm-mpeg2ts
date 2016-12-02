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
 * Component.h
 *
 *  Created on: 30/07/2013
 *      Author: Felippe Nagato
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Component : public MpegDescriptor {
	private:
		unsigned char streamContent;
		unsigned char componentType;
		unsigned char componentTag;
		string iso639LanguageCode;
		string text;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Component();
		~Component();

		void setStreamContent(unsigned char sc);
		unsigned char getStreamContent();
		void setComponentType(unsigned char ct);
		unsigned char getComponentType();
		void setComponentTag(unsigned char ct);
		unsigned char getComponentTag();
		void setIso639LanguageCode(string code);
		string getIso639LanguageCode();
		unsigned char getTextLength();
		void setText(string text);
		string getText();
};

}
}
}
}


#endif /* COMPONENT_H_ */
