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
