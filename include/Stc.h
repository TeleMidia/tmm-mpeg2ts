/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#ifndef STC_H_
#define STC_H_

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <inttypes.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

class Stc {

	#define SYSTEM_CLOCK_FREQUENCY 27000000
	#define	SYSTEM_CLOCK_FREQUENCY_90 90000

	private:
#ifdef _WIN32
		int64_t instant;
		double period; // microsecond
		void processorFrequency();
#endif
	protected:
		int64_t stc;
		struct timeval clockRef;

		int64_t reference;

		virtual void refreshStcSample();

	public:
		Stc();
		virtual ~Stc();

		static int timevalSubtract(
				struct timeval *result, struct timeval *x, struct timeval *y);
		static int64_t baseExtToStc(int64_t base, short int ext);
		static int64_t stcToBase(int64_t stc);
		static int64_t stcToExt(int64_t stc);
		static double stcToSecond(int64_t stc);
		static double baseToSecond(int64_t base);
		static int64_t secondToStc(double seconds);
		static int64_t secondToBase(double seconds);
		static int64_t calcDiff90khz(int64_t a, int64_t b);
		static int64_t calcDiff27Mhz(int64_t a, int64_t b);
		static void waitFor(int64_t stcPeriod);

		void getClock(struct timeval* usrClk);
		int64_t getReference();
		void setReference(int64_t stc);
		void setReference(int64_t base, short int ext);
		int64_t getStc();
		int64_t getStcBase();
		int64_t getStcExt();
		double getSecond();
};

}
}
}
}

#endif /* STC_H_ */
