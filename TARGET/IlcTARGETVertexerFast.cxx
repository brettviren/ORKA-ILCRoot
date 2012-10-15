/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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
#include <Riostream.h>
#include <TArrayF.h>
#include <TRandom.h>
#include "IlcESDVertex.h"
#include <IlcTARGETVertexerFast.h>
#include "IlcHeader.h"
#include "IlcGenEventHeader.h"
#include "IlcRun.h"
#include "IlcTARGETLoader.h"
#include "IlcRunLoader.h"

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Fast vertexer - True (i.e. generated) vertex coordinates            //
// are smeared with gaussians of given width                           //
// Origin: masera@to.infn.it     25/09/2003                            //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
ClassImp(IlcTARGETVertexerFast)



//______________________________________________________________________
IlcTARGETVertexerFast::IlcTARGETVertexerFast():IlcTARGETVertexer() {
  // Default Constructor
  fSmear = 0;
}

//______________________________________________________________________
IlcTARGETVertexerFast::IlcTARGETVertexerFast(Double_t *smear):IlcTARGETVertexer() {
  // Standard constructor
  fSmear = new Double_t[3];
  for(Int_t i=0;i<3;i++)fSmear[i]=smear[i];
  Info("IlcTARGETVertexerFast","Gaussian smaring of the generated vertex. Parameters %f12.5 , %f12.5 , %f12.5 \n",fSmear[0],fSmear[1],fSmear[2]);
}

//______________________________________________________________________
IlcTARGETVertexerFast::~IlcTARGETVertexerFast(){
  // Destructor
  if(fSmear)delete [] fSmear;
  fSmear = 0;
}

//______________________________________________________________________
IlcESDVertex* IlcTARGETVertexerFast::FindVertexForCurrentEvent(Int_t evnumb){
  // Defines the IlcTARGETVertex for the current event
  fCurrentVertex = 0;
  IlcRunLoader *rl =IlcRunLoader::Instance();
  rl->GetEvent(evnumb);
  TArrayF primaryVertex(3);  // true vertex
  IlcHeader* header = rl->GetHeader();
  IlcGenEventHeader* genEventHeader = header->GenEventHeader();   
  genEventHeader->PrimaryVertex(primaryVertex); 

  // Smearing
  Double_t vrttrue[3],vrtx[3];
  for(Int_t k=0; k<3;k++){
    vrttrue[k] = static_cast<Double_t>(primaryVertex[k]);
    vrtx[k] = gRandom->Gaus(vrttrue[k],fSmear[k]);
  }
  char name[30];
  sprintf(name,"Vertex_%d",evnumb);
  fCurrentVertex = new IlcESDVertex(vrtx,fSmear,name);
  fCurrentVertex->SetTitle("vertexer: smearMC");
  // // // //   fCurrentVertex->SetTruePos(vrttrue);
  return fCurrentVertex;
}

//______________________________________________________________________
void IlcTARGETVertexerFast::FindVertices(){
  // computes the vertices of the events in the range FirstEvent - LastEvent

  IlcRunLoader *rl = IlcRunLoader::Instance();
  IlcTARGETLoader* iTSloader =  (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");
  iTSloader->ReloadRecPoints();
  for(Int_t i=fFirstEvent;i<=fLastEvent;i++){
    rl->GetEvent(i);
    FindVertexForCurrentEvent(i);   
    if(fCurrentVertex) WriteCurrentVertex();
    else {
      cout<<"Vertex not found for event "<<i<<endl;

    }

  }

}

//________________________________________________________
void IlcTARGETVertexerFast::PrintStatus() const {
  // Print current status
  cout <<"=======================================================\n";

  cout<<"First event to be processed "<<fFirstEvent;
  cout<<"\n Last event to be processed "<<fLastEvent<<endl;
  cout<<"RMS for gaussian smearing: ";
  for(Int_t k=0;k<3;k++)cout<<" "<<fSmear[k];
  cout<<endl;
}

// // // // //______________________________________________________________________
// // // // IlcTARGETVertexerFast::IlcTARGETVertexerFast(const IlcTARGETVertexerFast &vtxr) : 
// // // //                     IlcTARGETVertexer(vtxr) {
// // // //   // Copy constructor
// // // //   // Copies are not allowed. The method is protected to avoid misuse.
// // // //   Error("IlcTARGETVertexerFast","Copy constructor not allowed\n");
// // // // }

//______________________________________________________________________
IlcTARGETVertexerFast& IlcTARGETVertexerFast::operator=(const 
                    IlcTARGETVertexerFast& /* vtxr */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}
