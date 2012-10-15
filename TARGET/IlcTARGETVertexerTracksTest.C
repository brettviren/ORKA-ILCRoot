#if !defined(__CINT__) || defined(__MAKECINT__)
//-- --- standard headers------------- 
#include <Riostream.h>
//--------Root headers ---------------
#include <TSystem.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <TObject.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <TKey.h>
//----- IlcRoot headers ---------------
#include "alles.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcMagF.h"
#include "IlcESDVertex.h"
#include "IlcTARGETVertexer.h"
#include "IlcTARGETVertexerTracks.h"
#include "IlcTracker.h"
//-------------------------------------
#endif
void IlcTARGETVertexerTracksTest(Int_t evFirst=0,Int_t evLast=0,Bool_t esd=kTRUE,
			      const Char_t *gilcName="gilc.root",
			      const Char_t *inName="IlcESDs.root",
                              const Char_t *outName="IlcESDs.root") {
  /*******************************************************************
   *                                                                 *
   * Test macro for vertexing in pp using tracks.                    *
   * If(esd) {                                                       *
   *   Input file must contain ESD events with tracks reco in TARGET    *
   *   if output file = input file, vertices will be stored in the   *
   *   ESD events and these rewritten to the same file; otherwise    *
   *   they will be written to a new file                            *
   * } else {                                                        *
   *   input file must contain a tree with IlcTARGETtracksV2 and        *
   *   can be written to the same file (output file = input file)    *
   *   or to a new one                                               *
   * }                                                               *
   * If file gilc.root is present field will be read from it       *
   * otherwise it can be set here "by hand".                         *
   *                                                                 *
   * Origin: A.Dainese, Padova  andrea.dainese@pd.infn.it            *
   *******************************************************************/     

  TStopwatch timer;
  timer.Start();

  // Look for field value in gilc.root
  Double_t field = 0.4;
  if(!gSystem->AccessPathName(gilcName,kFileExists)) {
    IlcRunLoader *rl = IlcRunLoader::Open(gilcName);
    rl->LoadgIlc();
    IlcMagF *fiel = (IlcMagF*)gIlc->Field();
    field=(Double_t)fiel->SolenoidField()/10.;
    printf(" B = %3.1f read from gIlc and set\n",field);

    IlcTracker::SetFieldMap(gIlc->Field(),1); // 1 means uniform magnetic field
  } else {
    printf(" File gilc.root not found: default 0.4 T being used!\n");
  }

  // Open input and output files
  TFile *inFile  = TFile::Open(inName);
  TFile *outFile = TFile::Open(outName,"update");

  // Create vertexer
  IlcTARGETVertexerTracks *vertexer = 
    new IlcTARGETVertexerTracks(inFile,outFile,evFirst,evLast);
  // Find vertices
  if(esd) {
    vertexer->FindVerticesESD();
  } else {
    vertexer->FindVertices();
  }

  timer.Stop(); timer.Print();

  delete vertexer;

  inFile->Close();
  outFile->Close();
  delete inFile;
  delete outFile;

  return;
}
//----------------------------------------------------------------------------
void VertexRecoInESDChain() {

  TStopwatch timer;
  timer.Start();

  // Look for field value in gilc.root
  Double_t field = 0.4;
  if(!gSystem->AccessPathName("gilc.root",kFileExists)) {
    IlcRunLoader *rl = IlcRunLoader::Open("gilc.root");
    rl->LoadgIlc();
    IlcMagF *fiel = (IlcMagF*)gIlc->Field();
    field=(Double_t)fiel->SolenoidField()/10.;
    printf(" B = %3.1f read from gIlc and set\n",field);

    IlcTracker::SetFieldMap(gIlc->Field(),1); // 1 means uniform magnetic field


  } else {
    printf(" File gilc.root not found: default 0.4 T being used!\n");
  }

  // open file with ESD events (this would not be necessary in the chain...)
  if(gSystem->AccessPathName("IlcESDs.root",kFileExists))
    { printf(" ESD file not found!\n"); return; }
  TFile *esdFile = new TFile("IlcESDs.root","update");
 
  IlcTARGETVertexerTracks *vertexer = new IlcTARGETVertexerTracks();
  vertexer->SetField(field);

  Int_t n=0;
  TKey *key=0;
  TIter next(esdFile->GetListOfKeys());

  //******* The loop over events
  while ((key=(TKey*)next())!=0) {
    printf("--- Processing event number : %d",n++);

    IlcESD *event=(IlcESD*)key->ReadObj();

    // find vertex and store it in the ESD (only pos and cov matrix)
    vertexer->FindVertexForCurrentEvent(event);

    // write event on file
    event->Write();

  }

  timer.Stop(); timer.Print();

  delete vertexer;
  esdFile->Close();
  delete esdFile;

  return;
}







