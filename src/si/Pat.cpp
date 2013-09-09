/******************************************************************************
Este arquivo eh parte da implementacao das ferramentas DTV do TeleMidia

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
This file is part of the TeleMidia DTV Tools

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

#include "si/Pat.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Pat::Pat() : PrivateSection() {
	pmtList = new map<unsigned short, unsigned short>;
	tableId = 0x00;
}

Pat::~Pat() {
	if (pmtList != NULL) {
		delete (pmtList);
	}
}

int Pat::processSectionPayload() {
	int pos = PrivateSection::processSectionPayload();
	int sectionSize = privateSectionLength + 3;
	unsigned short pnum, pmtPid;

	pmtList->clear();

	while ((pos + 4) < sectionSize) {
		pnum = (((stream[pos] & 0xFF) << 8) | (stream[pos+1] & 0xFF));
		pmtPid = (((stream[pos+2] & 0x1F) << 8) | (stream[pos+3] & 0xFF));
		addPmt(pnum, pmtPid);
		pos = pos + 4;
	}

	if (sectionSyntaxIndicator) {
		crc32 = ((stream[pos] & 0xFF) << 24) |
					((stream[pos + 1] & 0xFF) << 16) |
					((stream[pos + 2] & 0xFF) << 8) |
					(stream[pos + 3] & 0xFF);
		pos += 4;
	}

	return pos;
}

int Pat::updateStream() {
	unsigned short value;
	map<unsigned short, unsigned short>::iterator i;
	int pos = PrivateSection::updateStream();

	if ((pmtList != NULL) && (!pmtList->empty())) {
		i = pmtList->begin();
		while (i != pmtList->end()) {
			value = i->second;
			stream[pos++] = (value >> 8) & 0xFF;
			stream[pos++] = value & 0xFF;
			value = i->first;
			stream[pos] = 0xE0;
			stream[pos] = stream[pos] | ((value >> 8) & 0x1F);
			pos++;
			stream[pos++] = value & 0xFF;
			++i;
		}
	}

	if (sectionSyntaxIndicator) {
		Crc32 crc;
		unsigned int value = crc.crc(stream, pos);
		crc32 = value;
		stream[pos++] = (crc32 >> 24) & 0xFF;
		stream[pos++] = (crc32 >> 16) & 0xFF;
		stream[pos++] = (crc32 >> 8) & 0xFF;
		stream[pos++] = crc32 & 0xFF;
	}
	return pos;
}

int Pat::calculateSectionSize() {
	unsigned int pos = PrivateSection::calculateSectionSize();

	pos = pos + (pmtList->size() * 4);

	return pos + 4;
}

map<unsigned short, unsigned short>* Pat::getPmtList() {
	return pmtList;
}

void Pat::addPmt(unsigned short programNumber, unsigned short pid) {
	(*pmtList)[pid] = programNumber;
}

void Pat::removePmt(unsigned short pid) {
	map<unsigned short, unsigned short>::iterator i;
	if ((pmtList != NULL) && (!pmtList->empty())) {
		i = pmtList->begin();
		while (i != pmtList->end()) {
			if (i->first == pid) {
				pmtList->erase(i);
				break;
			}
			++i;
		}
	}
}

void Pat::releasePmtList() {
	if (pmtList != NULL) {
		pmtList->clear();
	}
}

}
}
}
}
