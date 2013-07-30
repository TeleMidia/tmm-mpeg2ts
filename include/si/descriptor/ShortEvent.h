/*
 * ShortEvent.h
 *
 *  Created on: 30/07/2013
 *      Author: Felippe Nagato
 */

#ifndef SHORTEVENT_H_
#define SHORTEVENT_H_

#include "MpegDescriptor.h"
#include <string>
#include <cstring>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class ShortEvent : public MpegDescriptor {
	private:
		string iso639LanguageCode;
		string eventName;
		string text;

	protected:

		virtual int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		ShortEvent();
		~ShortEvent();

		void setIso639LanguageCode(string code);
		string getIso639LanguageCode();
		unsigned char getEventNameLength();
		void setEventName(string name);
		string getEventName();
		unsigned char getTextLength();
		void setText(string text);
		string getText();
};

}
}
}
}

#endif /* SHORTEVENT_H_ */
