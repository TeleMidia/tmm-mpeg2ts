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
 * ParentalRating.h
 *
 *  Created on: 30/07/2013
 *      Author: Felippe Nagato
 */

#ifndef PARENTALRATING_H_
#define PARENTALRATING_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

struct ParentalRatingInfo {
	string countryCode;
	unsigned char rating;
};

class ParentalRating : public MpegDescriptor {
	private:
		vector<ParentalRatingInfo*> parentalRatingList;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		ParentalRating();
		~ParentalRating();

		vector<ParentalRatingInfo*>* getParentalRatingList();
		void addParentalRating(ParentalRatingInfo* pri);
};

}
}
}
}


#endif /* PARENTALRATING_H_ */
