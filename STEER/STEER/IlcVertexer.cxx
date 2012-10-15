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

/* $Id: IlcVertexer.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcLog.h"
#include "IlcESDVertex.h"
#include "IlcVertexer.h"
#include "IlcMultiplicity.h"

ClassImp(IlcVertexer)

//////////////////////////////////////////////////////////////////////
// Base class for primary vertex reconstruction                     //
// IlcESDVertexer is a class for full 3D primary vertex finding     //
// derived classes: IlcITSVertexer                                  //
//                                                                  //
//////////////////////////////////////////////////////////////////////

//______________________________________________________________________
IlcVertexer::IlcVertexer() :
  fCurrentVertex(NULL),
  fMult(NULL)
{
  //
  // Default Constructor
  //
  SetVtxStart(0.,0.,0.);
  for(Int_t i=0;i<6;i++)fNominalCov[i]=0.;
}

//______________________________________________________________________
IlcVertexer::~IlcVertexer() {
  // Default Destructor

  if(fMult) delete fMult;
}


//---------------------------------------------------------------------------
void  IlcVertexer::SetVtxStart(IlcESDVertex *vtx) 
{ 
//
// Set initial vertex knowledge
//
  vtx->GetXYZ(fNominalPos);
  vtx->GetCovMatrix(fNominalCov);
  return; 
}
