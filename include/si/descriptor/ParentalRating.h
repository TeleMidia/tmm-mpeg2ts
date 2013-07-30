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
