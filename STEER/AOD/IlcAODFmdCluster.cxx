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

/* $Id: IlcAODFmdCluster.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD FMD cluster class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include "IlcAODFmdCluster.h"

ClassImp(IlcAODFmdCluster)
  
//______________________________________________________________________________
IlcAODFmdCluster::IlcAODFmdCluster() : 
  IlcAODCluster(),
  fProdVertex(NULL),
  fPrimTrack(NULL)
{
  // default constructor
}

//______________________________________________________________________________
IlcAODFmdCluster::IlcAODFmdCluster(Int_t id,
				   Int_t nLabel,
				   Int_t *label, 
				   Double_t energy,
				   Double_t x[3],
				   Double_t pid[9],
				   Char_t ttype,
				   IlcAODVertex *prodVertex,
				   IlcAODTrack *primTrack) :
  IlcAODCluster(id, nLabel, label, energy, x, pid, ttype),
  fProdVertex(prodVertex),
  fPrimTrack(primTrack)
{
  // constructor
}

//______________________________________________________________________________
IlcAODFmdCluster::IlcAODFmdCluster(Int_t id,
				   Int_t nLabel,
				   Int_t *label, 
				   Float_t energy,
				   Float_t x[3],
				   Float_t pid[9],
				   Char_t ttype,
				   IlcAODVertex *prodVertex,
				   IlcAODTrack *primTrack) :
  IlcAODCluster(id, nLabel, label, energy, x, pid, ttype),
  fProdVertex(prodVertex),
  fPrimTrack(primTrack)
{
  // constructor
}


//______________________________________________________________________________
IlcAODFmdCluster::~IlcAODFmdCluster() 
{
  // destructor
}


//______________________________________________________________________________
IlcAODFmdCluster::IlcAODFmdCluster(const IlcAODFmdCluster& clus) :
  IlcAODCluster(clus),
  fProdVertex(clus.fProdVertex),
  fPrimTrack(clus.fPrimTrack)
{
  // Copy constructor
}

//______________________________________________________________________________
IlcAODFmdCluster& IlcAODFmdCluster::operator=(const IlcAODFmdCluster& clus)
{
  // Assignment operator
  if(this!=&clus) {

    IlcAODCluster::operator=(clus);

    fProdVertex = clus.fProdVertex;
    fPrimTrack = clus.fPrimTrack;
  }

  return *this;
}
