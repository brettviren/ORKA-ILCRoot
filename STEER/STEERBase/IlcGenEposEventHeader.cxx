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

/* $Id$ */

/*
 * IlcGenEposEventHeader.h
 * 
 * Header for EPOS generated event.
 *
 *      Author: Piotr Ostrowski
 */

#include "IlcGenEposEventHeader.h"

ClassImp(IlcGenEposEventHeader)


IlcGenEposEventHeader::IlcGenEposEventHeader(const char* name):
    IlcGenEventHeader(name),
    fBimevt(0),
    fPhievt(0),
    fKolevt(0),
    fKoievt(0),
    fPmxevt(0),
    fEgyevt(0),
    fNpjevt(0),
    fNtgevt(0),
    fNpnevt(0),
    fNppevt(0),
    fNtnevt(0),
    fNtpevt(0),
    fJpnevt(0),
    fJppevt(0),
    fJtnevt(0),
    fJtpevt(0),
    fXbjevt(0),
    fQsqevt(0),
    fNglevt(0),
    fZppevt(0),
    fZptevt(0)
{
// Constructor
}

IlcGenEposEventHeader::IlcGenEposEventHeader() :     fBimevt(0),
    fPhievt(0),
    fKolevt(0),
    fKoievt(0),
    fPmxevt(0),
    fEgyevt(0),
    fNpjevt(0),
    fNtgevt(0),
    fNpnevt(0),
    fNppevt(0),
    fNtnevt(0),
    fNtpevt(0),
    fJpnevt(0),
    fJppevt(0),
    fJtnevt(0),
    fJtpevt(0),
    fXbjevt(0),
    fQsqevt(0),
    fNglevt(0),
    fZppevt(0),
    fZptevt(0)
{
// Default constructor
}

