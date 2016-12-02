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
 * Iso639Language.h
 *
 *  Created on: 24/10/2013
 *      Author: Felippe Nagato
 */

#ifndef ISO639LANGUAGE_H_
#define ISO639LANGUAGE_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Iso639Language : public MpegDescriptor {
	struct Iso639Lang {
		string iso639LanguageCode;
		unsigned char audioType;
	};

	private:
		vector<Iso639Lang*> iso639LanguageList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		Iso639Language();
		~Iso639Language();

		bool addIso639Language(string language, unsigned char audioType);
		void releaseIso639LanguageList();
};

}
}
}
}


#endif /* ISO639LANGUAGE_H_ */
