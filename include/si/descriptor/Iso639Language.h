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
