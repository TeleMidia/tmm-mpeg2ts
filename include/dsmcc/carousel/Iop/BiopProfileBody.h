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
