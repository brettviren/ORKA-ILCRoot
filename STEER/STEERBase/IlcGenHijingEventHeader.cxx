/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcGenHijingEventHeader.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcGenHijingEventHeader.h"
ClassImp(IlcGenHijingEventHeader)

IlcGenHijingEventHeader::IlcGenHijingEventHeader():
    fTotalEnergy(0.),
    fTrials(0),
    fJet1(0., 0., 0., 0.),
    fJet2(0., 0., 0., 0.),
    fJetFsr1(0., 0., 0., 0.),
    fJetFsr2(0., 0., 0., 0.)
{
    // Constructor
}

IlcGenHijingEventHeader::IlcGenHijingEventHeader(const char* name):
    IlcGenEventHeader(name),
    fTotalEnergy(0.),
    fTrials(0),
    fJet1(0., 0., 0., 0.),
    fJet2(0., 0., 0., 0.),
    fJetFsr1(0., 0., 0., 0.),
    fJetFsr2(0., 0., 0., 0.)
{
    // Copy Constructor
}
