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
 * BiopProfileBody.h
 *
 *  Created on: Jun 15, 2009
 *      Author: Felippe Nagato
 */

#ifndef BIOPPROFILEBODY_H_
#define BIOPPROFILEBODY_H_

#include "dsmcc/carousel/Iop/TaggedProfile.h"
#include "dsmcc/carousel/Biop/ObjectLocation.h"
#include "dsmcc/carousel/ConnBinder.h"

#include <iostream>

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {
	class BiopProfileBody : public TaggedProfile {

		private:

		protected:
			ObjectLocation* objectLocation;
			ConnBinder* connBinder;

			int updateStream();
			unsigned int calculateStreamSize();

		public:
			BiopProfileBody();
			~BiopProfileBody();

			ObjectLocation* getObjectLocation();
			ConnBinder* getConnBinder();

			void setObjectLocation(ObjectLocation* ol);
			void setConnBinder(ConnBinder* cb);
	 };
}
}
}
}
}

#endif /* BIOPPROFILEBODY_H_ */
