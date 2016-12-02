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

#ifndef PAT_H_
#define PAT_H_

#include "PrivateSection.h"
#include "Crc32.h"

#include <map>
#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Pat : public PrivateSection {
	protected:
		map<unsigned short, unsigned short>* pmtList;

		int processSectionPayload();
		int calculateSectionSize();

	public:
		Pat();
		virtual ~Pat();

		int updateStream();

		map<unsigned short, unsigned short>* getPmtList();

		void addPmt(unsigned short programNumber, unsigned short pid);
		void removePmt(unsigned short pid);

		void releasePmtList();
};

}
}
}
}

#endif /* PAT_H_ */
