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

/* $Id: IlcGenReader.cxx 8819 2003-12-12 17:29:30Z hristov $ */
//
// Interface for reading events from files.
// Realisations of this interface have to be used with IlcGenExFile.
// NextEvent() loops over events 
// and NextParticle() loops over particles. 
// Author: andreas.morsch@cern.ch

#include "IlcGenReader.h"
ClassImp(IlcGenReader)


IlcGenReader& IlcGenReader::operator=(const  IlcGenReader& rhs)
{
// Assignment operator
    rhs.Copy(*this);
    return *this;
}

void IlcGenReader::Copy(TObject&) const
{
    //
    // Copy 
    //
    Fatal("Copy","Not implemented!\n");
}



