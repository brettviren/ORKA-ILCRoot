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

/* $Id: IlcAODPmdCluster.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD PMD cluster class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include "IlcAODPmdCluster.h"

ClassImp(IlcAODPmdCluster)

//______________________________________________________________________________
IlcAODPmdCluster::IlcAODPmdCluster() : 
  IlcAODCluster(),
  fAssocCluster(NULL)
{
  // default constructor
}

//______________________________________________________________________________
IlcAODPmdCluster::IlcAODPmdCluster(Int_t id,
				   Int_t nLabel,
				   Int_t *label, 
				   Double_t energy,
				   Double_t x[3],
				   Double_t pid[13],
				   Char_t ttype,
				   UInt_t /*selectInfo*/,
				   IlcAODPmdCluster* assoc) :
  IlcAODCluster(id, nLabel, label, energy, x, pid, ttype),
  fAssocCluster(assoc)
{
  // constructor
}

//______________________________________________________________________________
IlcAODPmdCluster::IlcAODPmdCluster(Int_t id,
				   Int_t nLabel,
				   Int_t *label, 
				   Float_t energy,
				   Float_t x[3],
				   Float_t pid[13],
				   Char_t ttype,
				   UInt_t /*selectInfo*/,
				   IlcAODPmdCluster* assoc) :
  IlcAODCluster(id, nLabel, label, energy, x, pid, ttype),
  fAssocCluster(assoc)
{
  // constructor
}


//______________________________________________________________________________
IlcAODPmdCluster::~IlcAODPmdCluster() 
{
  // destructor
}


//______________________________________________________________________________
IlcAODPmdCluster::IlcAODPmdCluster(const IlcAODPmdCluster& clus) :
  IlcAODCluster(clus),
  fAssocCluster(clus.fAssocCluster)
{
  // Copy constructor
}

//______________________________________________________________________________
IlcAODPmdCluster& IlcAODPmdCluster::operator=(const IlcAODPmdCluster& clus)
{
  // Assignment operator
  if(this!=&clus) {

    IlcAODCluster::operator=(clus);
    
    fAssocCluster = clus.fAssocCluster;
  }

  return *this;
}
