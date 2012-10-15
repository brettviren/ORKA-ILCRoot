#include <TTree.h>

#include <IlcESDVertex.h>
#include <IlcTARGETVertexer.h>
#include <IlcRunLoader.h>
#include <IlcTARGETLoader.h>
#include <IlcMultiplicity.h>
#include <IlcTARGETMultReconstructor.h>

ClassImp(IlcTARGETVertexer)

//////////////////////////////////////////////////////////////////////
// Base class for primary vertex reconstruction                     //
// IlcESDVertexer is a class for full 3D primary vertex finding     //
// derived classes: IlcTARGETVertexerIons IlcTARGETvertexerPPZ            //
//                  IlcTARGETVertexerTracks                            //
//////////////////////////////////////////////////////////////////////

//______________________________________________________________________
IlcTARGETVertexer::IlcTARGETVertexer():IlcVertexer(),
fFirstEvent(0),
fLastEvent(-1)
{
  // Default Constructor
}

IlcTARGETVertexer::IlcTARGETVertexer(TString filename):
fFirstEvent(0),
fLastEvent(-1)
{
  // Standard constructor
  IlcRunLoader *rl = IlcRunLoader::Instance();
  if(!rl){
    Fatal("IlcTARGETVertexer","Run Loader not found");
  }
  if(rl->LoadgIlc()){
    Fatal("IlcTARGETVertexer","The IlcRun object is not available - nothing done");
  }
  fCurrentVertex  = 0;   
// // // //   fDebug = 0;
  SetFirstEvent(0);
  SetLastEvent(-1);
  rl->LoadHeader();

  if (fLastEvent < 0) SetLastEvent(rl->GetNumberOfEvents()-1);

  IlcTARGETLoader* itsLoader =  (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");
  if(!filename.Contains("default"))itsLoader->SetVerticesFileName(filename);
  if(!filename.Contains("null"))itsLoader->LoadVertices("recreate");
  itsLoader->LoadRecPoints();
  Int_t lst;
  if(rl->TreeE()){
    lst = static_cast<Int_t>(rl->TreeE()->GetEntries());
    SetLastEvent(lst-1);
  }
}

// // // // //______________________________________________________________________
// // // // IlcTARGETVertexer::IlcTARGETVertexer(const IlcTARGETVertexer &vtxr) : IlcVertexer(vtxr) {
// // // //   // Copy constructor
// // // //   // Copies are not allowed. The method is protected to avoid misuse.
// // // //   Error("IlcTARGETVertexer","Copy constructor not allowed\n");
// // // // }

//______________________________________________________________________
IlcTARGETVertexer& IlcTARGETVertexer::operator=(const IlcTARGETVertexer& /* vtxr */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}

//______________________________________________________________________
void IlcTARGETVertexer::FindMultiplicity(Int_t evnumber){
  // Invokes IlcTARGETMultReconstructor to determine the
  // charged multiplicity in the pixel layers
  if(fMult){delete fMult; fMult = 0;}
  Bool_t success=kTRUE;
  if(!fCurrentVertex)success=kFALSE;
  if(fCurrentVertex && fCurrentVertex->GetNContributors()<1)success=kFALSE;
  if(!success){
    IlcWarning("Tracklets multiplicity not determined because the primary vertex was not found");
    return;
  }
  IlcTARGETMultReconstructor* multReco = new IlcTARGETMultReconstructor();
  IlcRunLoader *rl =IlcRunLoader::Instance();
  IlcTARGETLoader* itsLoader = (IlcTARGETLoader*)rl->GetLoader("TARGETLoader");
  multReco->SetGeometry(itsLoader->GetTARGETgeom());
  itsLoader->LoadRecPoints();
  rl->GetEvent(evnumber);
  TTree* itsClusterTree = itsLoader->TreeR();
  if (!itsClusterTree) {
    IlcError(" Can't get the TARGET cluster tree !\n");
    return;
  }
  Double_t vtx[3];
  fCurrentVertex->GetXYZ(vtx);
  Float_t vtxf[3];
  for(Int_t i=0;i<3;i++)vtxf[i]=vtx[i];
  multReco->SetHistOn(kFALSE);
  multReco->Reconstruct(itsClusterTree,vtxf,vtxf);

  Int_t notracks=multReco->GetNTracklets();
  Float_t *trk = new Float_t [notracks];
  Float_t *phi = new Float_t [notracks];
  Float_t *dphi = new Float_t [notracks];
  for(Int_t i=0;i<multReco->GetNTracklets();i++){
    trk[i] = multReco->GetTracklet(i)[0];
    phi[i] =  multReco->GetTracklet(i)[1];
    dphi[i] = multReco->GetTracklet(i)[2];
  }
// // // //   fMult = new IlcMultiplicity(notracks,trk,phi, dphi); //FIXME
  delete [] trk;
  delete [] phi;
  delete [] dphi;
  itsLoader->UnloadRecPoints();
  delete multReco;
  return;
}

//______________________________________________________________________
void IlcTARGETVertexer::WriteCurrentVertex(){
  // Write the current IlcVertex object to file fOutFile
  IlcRunLoader *rl = IlcRunLoader::Instance();
  IlcTARGETLoader* itsLoader =  (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");
  fCurrentVertex->SetName("Vertex");
  //  const char * name = fCurrentVertex->GetName();
  //  itsLoader->SetVerticesContName(name);
  Int_t rc = itsLoader->PostVertex(fCurrentVertex);
  rc = itsLoader->WriteVertices();
}
