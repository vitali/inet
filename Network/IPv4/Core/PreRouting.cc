//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//


#include <omnetpp.h>

#include "PreRouting.h"

Define_Module(PreRouting);


void PreRouting::endService(cMessage *msg)
{
    IPDatagram *datagram = check_and_cast<IPDatagram *>(msg);

    // check for header biterror
    if (datagram->hasBitError())
    {
        // probability of bit error in header = size of header / size of total message
        double relativeHeaderLength = datagram->headerLength() / (double)datagram->length()/8;
        if (dblrand() <= relativeHeaderLength)
        {
            icmpAccess.get()->sendErrorMessage(datagram, ICMP_PARAMETER_PROBLEM, 0);
            return;
        }
        // FIXME ignore bit error if in payload?
    }

    // hop counter decrement
    datagram->setTimeToLive (datagram->timeToLive()-1);

    send(datagram, "routingOut");
}

