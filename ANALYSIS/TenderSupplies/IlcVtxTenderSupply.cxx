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


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Redo primary vertex on the fly, using the diamond constraint              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include <IlcESDEvent.h>
#include <IlcESDInputHandler.h>
#include <IlcVertexerTracks.h>
#include <IlcTender.h>
#include <IlcCDBId.h>
#include <IlcCDBManager.h>
#include <IlcCDBEntry.h>
#include <IlcESDUtils.h>

#include "IlcVtxTenderSupply.h"

ClassImp(IlcVtxTenderSupply)

IlcVtxTenderSupply::IlcVtxTenderSupply() :
  IlcTenderSupply(),
  fDiamond(0x0),
  fRefitAlgo(-1)
{
  //
  // default ctor
  //
}

//_____________________________________________________
IlcVtxTenderSupply::IlcVtxTenderSupply(const char *name, const IlcTender *tender) :
  IlcTenderSupply(name,tender),
  fDiamond(0x0),
  fRefitAlgo(-1)
{
  //
  // named ctor
  //
}

//_____________________________________________________
void IlcVtxTenderSupply::ProcessEvent()
{
  //
  // Recalculate the Vertex with constraint
  //

  IlcESDEvent *event=fTender->GetEvent();
  if (!event) return;

  //
  if (fRefitAlgo >=0 ) { // refit with user supplied algo and using the diamond stored in ESD
    IlcESDUtils::RefitESDVertexTracks(event, fRefitAlgo);
    return;
  }
  //

  if (fTender->RunChanged()){
    fDiamond=0x0;
    IlcCDBEntry *meanVertex=fTender->GetCDBManager()->Get("GRP/Calib/MeanVertex",fTender->GetRun());
    if (!meanVertex) {
      IlcError("No new MeanVertex entry found");
      return;
    } else {
      fDiamond=(IlcESDVertex*)meanVertex->GetObject();
    }
    //printf("\nRun %d, sigmaX %f, sigmaY %f\n",fTender->GetRun(),fDiamond->GetXRes(),fDiamond->GetYRes());
  }

  if (!fDiamond) return;

  // Redo the primary with the constraint ONLY if the updated mean vertex was found in the OCDB
  if ( (fDiamond->GetXRes())<2){ 
    IlcVertexerTracks vertexer(event->GetMagneticField());
    vertexer.SetITSMode();
    vertexer.SetMinClusters(3);
    vertexer.SetVtxStart(fDiamond);
    IlcESDVertex *pvertex = vertexer.FindPrimaryVertex(event);
    event->SetPrimaryVertexTracks(pvertex);
    // write the diamond parameters
    event->SetDiamond(fDiamond);
    delete pvertex;
  }  
}
