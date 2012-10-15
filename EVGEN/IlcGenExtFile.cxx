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

/* $Id: IlcGenExtFile.cxx 52948 2011-11-17 07:51:15Z morsch $ */

// Event generator that using an instance of type IlcGenReader
// reads particles from a file and applies cuts.
// Example: In your Config.C you can include the following lines
//  IlcGenExtFile *gener = new IlcGenExtFile(-1);
//  gener->SetMomentumRange(0,999);
//  gener->SetPhiRange(-180.,180.);
//  gener->SetThetaRange(0,180);
//  gener->SetYRange(-999,999);
//  IlcGenReaderTreeK * reader = new IlcGenReaderTreeK();
//  reader->SetFileName("myFileWithTreeK.root");
//  gener->SetReader(reader);
//  gener->Init();


#include <Riostream.h>

#include "IlcGenExtFile.h"
#include "IlcRunLoader.h"
#include "IlcHeader.h"
#include "IlcStack.h"
#include "IlcGenEventHeader.h"
#include "IlcGenReader.h"

#include <TParticle.h>
#include <TFile.h>
#include <TTree.h>


ClassImp(IlcGenExtFile)

IlcGenExtFile::IlcGenExtFile()
    :IlcGenMC(),
     fFileName(0),
     fReader(0)
{
//  Constructor
//
//  Read all particles
    fNpart  = -1;
}

IlcGenExtFile::IlcGenExtFile(Int_t npart)
    :IlcGenMC(npart),
     fFileName(0),
     fReader(0)
{
//  Constructor
    fName   = "ExtFile";
    fTitle  = "Primaries from ext. File";
}

//____________________________________________________________
IlcGenExtFile::~IlcGenExtFile()
{
// Destructor
    delete fReader;
}

//___________________________________________________________
void IlcGenExtFile::Init()
{
// Initialize
    if (fReader) fReader->Init();
}

    
void IlcGenExtFile::Generate()
{
// Generate particles

  Double_t polar[3]  = {0,0,0};
  //
  Double_t origin[3] = {0,0,0};
  Double_t time = 0.;
  Double_t p[4];
  Float_t random[6];
  Int_t i = 0, j, nt;
  //
  //
  if (fVertexSmear == kPerEvent) Vertex();

  while(1) {
    Int_t nTracks = fReader->NextEvent(); 	
    if (nTracks == 0) {
      // printf("\n No more events !!! !\n");
      Warning("IlcGenExtFile::Generate","\nNo more events in external file!!!\nLast event may be empty or incomplete.\n");
      return;
    }

    //
    // Particle selection loop
    //
    // The selection criterium for the external file generator is as follows:
    //
    // 1) All tracks are subject to the cuts defined by IlcGenerator, i.e.
    //    fThetaMin, fThetaMax, fPhiMin, fPhiMax, fPMin, fPMax, fPtMin, fPtMax,
    //    fYMin, fYMax.
    //    If the particle does not satisfy these cuts, it is not put on the
    //    stack.
    // 2) If fCutOnChild and some specific child is selected (e.g. if
    //    fForceDecay==kSemiElectronic) the event is rejected if NOT EVEN ONE
    //    child falls into the child-cuts.
    TParticle* iparticle = 0x0;
    
    if(fCutOnChild) {
      // Count the selected children
      Int_t nSelected = 0;
      while ((iparticle=fReader->NextParticle()) ) {
	  Int_t kf = CheckPDGCode(iparticle->GetPdgCode());
	  kf = TMath::Abs(kf);
	  if (ChildSelected(kf) && KinematicSelection(iparticle, 1)) {
	      nSelected++;
	  }
      }
      if (!nSelected) continue;    // No particle selected:  Go to next event
      fReader->RewindEvent();
    }

    //
    // Stack filling loop
    //
    fNprimaries = 0;
    for (i = 0; i < nTracks; i++) {
	TParticle* jparticle = fReader->NextParticle();
	Bool_t selected = KinematicSelection(jparticle,0); 
	if (!selected) continue;
	p[0] = jparticle->Px();
	p[1] = jparticle->Py();
	p[2] = jparticle->Pz();
	p[3] = jparticle->Energy();
	
	Int_t idpart = jparticle->GetPdgCode();
	if(fVertexSmear==kPerTrack) 
	{
	    Rndm(random,6);
	    for (j = 0; j < 3; j++) {
		origin[j]=fOrigin[j]+
		    fOsigma[j]*TMath::Cos(2*random[2*j]*TMath::Pi())*
		    TMath::Sqrt(-2*TMath::Log(random[2*j+1]));
	    }
	    Rndm(random,2);
	    time = fTimeOrigin + fOsigma[2]/TMath::Ccgs()*
	      TMath::Cos(2*random[0]*TMath::Pi())*
	      TMath::Sqrt(-2*TMath::Log(random[1]));
	} else {
	    origin[0] = fVertex[0] + jparticle->Vx();
	    origin[1] = fVertex[1] + jparticle->Vy();
	    origin[2] = fVertex[2] + jparticle->Vz();
	    time = fTime + jparticle->T();
	}
	Int_t doTracking = fTrackIt && selected && (jparticle->TestBit(kTransportBit));
	Int_t parent     = jparticle->GetFirstMother();
	
	PushTrack(doTracking, parent, idpart,
		  p[0], p[1], p[2], p[3], origin[0], origin[1], origin[2], time,
		  polar[0], polar[1], polar[2],
		  kPPrimary, nt, 1., jparticle->GetStatusCode());

	KeepTrack(nt);
	fNprimaries++;
    } // track loop

    // Generated event header
    
    IlcGenEventHeader * header = new IlcGenEventHeader();
    header->SetNProduced(fNprimaries);
    header->SetPrimaryVertex(fVertex);
    header->SetInteractionTime(fTime);
    AddHeader(header);
    break;
    
  } // event loop
  
  SetHighWaterMark(nt);
  CdEventFile();
}

void IlcGenExtFile::CdEventFile()
{
// CD back to the event file
  IlcRunLoader::Instance()->CdGAFile();
}





