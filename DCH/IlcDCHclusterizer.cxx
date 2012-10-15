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

/* $Id: IlcDCHclusterizer.cxx,v 1.3 2008/05/12 12:54:36 cgatto Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH cluster finder base class                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <Riostream.h>

#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcLog.h"

#include "IlcDCHclusterizer.h"

ClassImp(IlcDCHclusterizer)

//_____________________________________________________________________________
IlcDCHclusterizer::IlcDCHclusterizer()
{
  //
  // IlcDCHclusterizer default constructor
  //

  fClusterTree = NULL;
  fRecPoints   = 0;
  fVerbose     = 0;
  fDCHLoader   = 0;
  fNclus       = 0;
}

//_____________________________________________________________________________
IlcDCHclusterizer::IlcDCHclusterizer(const Text_t* name, const Text_t* title):TNamed(name, title)
{
  //
  // IlcDCHclusterizer default constructor
  //

  fClusterTree = NULL;
  fRecPoints   = 0;
  fVerbose     = 0;
  fDCHLoader   = 0;
  fNclus       = 0;

}

//_____________________________________________________________________________
IlcDCHclusterizer::IlcDCHclusterizer(const IlcDCHclusterizer &c):TNamed(c)
{
  //
  // IlcDCHclusterizer copy constructor
  //

  ((IlcDCHclusterizer &) c).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHclusterizer::~IlcDCHclusterizer()
{
  //
  // IlcDCHclusterizer destructor
  //

  if (fRecPoints) {
    fRecPoints->Delete();
    delete fRecPoints;
  }
}

//_____________________________________________________________________________
IlcDCHclusterizer &IlcDCHclusterizer::operator=(const IlcDCHclusterizer &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHclusterizer &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHclusterizer::Copy(TObject &c) const
{
  //
  // Copy function
  //

  ((IlcDCHclusterizer &) c).fClusterTree = NULL;
  ((IlcDCHclusterizer &) c).fRecPoints   = NULL;  
  ((IlcDCHclusterizer &) c).fVerbose     = fVerbose;  
  ((IlcDCHclusterizer &) c).fDCHLoader   = fDCHLoader;  
  ((IlcDCHclusterizer &) c).fNclus       = fNclus;  
}

//_____________________________________________________________________________
Bool_t IlcDCHclusterizer::Open(const Char_t *name, Int_t nEvent)
{
  //
  // Opens the IlcROOT file. Output and input are in the same file
  //
  TString evfoldname = IlcConfig::GetDefaultEventFolderName();
  fRunLoader = IlcRunLoader::GetRunLoader(evfoldname);
  if (!fRunLoader)
    fRunLoader = IlcRunLoader::Open(name);
  if (!fRunLoader)
   {
     Error("Open","Can not open session for file %s.",name);
     return kFALSE;
   }

  fDCHLoader = fRunLoader->GetLoader("DCHLoader");
  if (!fDCHLoader ) {
    IlcError("Can not find DCH Loader");
    return kFALSE;
  }

//  OpenInput(nEvent);
  OpenOutput();
  return kTRUE;
}


//_____________________________________________________________________________
Bool_t IlcDCHclusterizer::OpenOutput()
{
  //
  // Open the output file
  //

//  TClonesArray *ioArray = new TClonesArray("IlcDCHcluster");
  if (!fRecPoints) fRecPoints  = new TClonesArray("IlcDCHcluster");

  fDCHLoader->MakeTree("R");
  fClusterTree = fDCHLoader->TreeR();
  fClusterTree->Branch("DCHcluster","TClonesArray",&fRecPoints,32000);

//  delete ioArray;
  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHclusterizer::OpenInput(Int_t nEvent)
{
  //
  // Opens a ROOT-file with DCH-hits and reads in the digits-tree
  //

  // Connect the IlcRoot file containing Geometry, Kine, and Hits
  if (fRunLoader->GetIlcRun() == 0x0) fRunLoader->LoadgIlc();
  gIlc = fRunLoader->GetIlcRun();

  if (!(gIlc)) {
    fRunLoader->LoadgIlc();
    gIlc = fRunLoader->GetIlcRun();
      if (!(gIlc)) {
        printf("IlcDCHclusterizer::OpenInput -- ");
        printf("Could not find IlcRun object.\n");
        return kFALSE;
      }
  }

  // Import the Trees for the event nEvent in the file
  fRunLoader->GetEvent(nEvent);
  
  fNclus=0;
  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHclusterizer::WriteClusters()
{
  //
  // Fills DCHcluster branch in the tree with the clusters 
  // found in detector = det. For det=-1 writes the tree. 
  //
/* 
  if (((det < -1) || (det >= IlcDCHgeometry::Ndet()))||((detEC < -1) || (detEC >= IlcDCHgeometry::NdetEC()))) {
    printf("IlcDCHclusterizer::WriteClusters -- ");
    printf("Unexpected Barrel detector index %d.\n",det);
    return kFALSE;
  }
 
 */
//   TBranch *branch = fClusterTree->GetBranch("DCHcluster");
//   cout <<" branch "<<branch<<endl; 
//   if (!branch) {
//     TClonesArray *ioArray = new TClonesArray("IlcDCHcluster");
//     branch = fClusterTree->Branch("DCHcluster","TClonesArray",&ioArray,32000);
//     delete ioArray;
//   }

/*   if (((det >= 0) && (det < IlcDCHgeometry::Ndet()))||((detEC >= 0) && (detEC < IlcDCHgeometry::NdetEC()))) {
    //printf("qui va bene\n");
    Int_t nRecPoints = RecPoints()->GetEntriesFast();
    TClonesArray *detRecPoints = new TClonesArray("IlcDCHcluster",400);
    int nadded=0;
    for (Int_t i = 0; i < nRecPoints; i++) {
      IlcDCHcluster *c = (IlcDCHcluster *) RecPoints()->UncheckedAt(i);
      if (det == c->GetDetector()|| detEC == c->GetDetectorEC()) {
	new((*detRecPoints)[nadded]) IlcDCHcluster(*c);
        //detRecPoints->AddLast(c);
	nadded++;
      }
      else {
        printf("IlcDCHclusterizer::WriteClusters --");
        printf("Attempt to write a cluster with unexpected detector index\n");
      }
    }

    branch->SetAddress(&detRecPoints);
    fClusterTree->Fill();

    delete detRecPoints;

    return kTRUE;

  }

 */
//   if ((det == -1)&&(detEC =DCH/IlcDCHclusterizer.cxx:35:28:= -1)) {

    Info("WriteClusters","Writing the cluster tree %s for event %d."
	 ,fClusterTree->GetName(),fRunLoader->GetEventNumber());
    /*
    fClusterTree->Write();
    IlcDCHgeometry *geo = fDCH->GetGeometry();
    geo->SetName("DCHgeometry");
    geo->Write();
    */
//    IlcLoader* loader = fRunLoader->GetLoader("DCHLoader");
//    loader->WriteRecPoints("OVERWRITE");
  
//    return kTRUE;  

//  }
  /*
  IlcLoader* loader = fRunLoader->GetLoader("DCHLoader");
  loader->WriteDigits("OVERWRITE");
  */

  fClusterTree->Fill();
  if (fDCHLoader->WriteRecPoints("OVERWRITE")==1) {
    IlcError("IlcDCHclusterizer::WriteClusters -- "); 
    return kFALSE;  
  }
//  fRecPoints->Clear();
//  fRecPoints->Delete();
  fNclus=0;
  return kTRUE;
  
}

//_____________________________________________________________________________
void IlcDCHclusterizer::FillEv(){
cout<<fClusterTree<<endl;
  if (fClusterTree) fClusterTree->Fill();
}

//_____________________________________________________________________________
IlcDCHcluster* IlcDCHclusterizer::AddCluster(Float_t *pos, Float_t *sig, Int_t *tracks, ULong_t Id, Float_t amp)
//IlcDCHcluster* IlcDCHclusterizer::AddCluster(Float_t *pos, Float_t *sig, Int_t *tracks, Float_t amp,)

{
  //
  // Add a cluster for the DCH
  //
  fNclus++;
  IlcDCHcluster *c = new((*fRecPoints)[fNclus-1]) IlcDCHcluster;

//   fRecPoints->ExpandCreateFast(fNclus);
//   IlcDCHcluster *c = (IlcDCHcluster *) fRecPoints->Last();

  c->SetX(pos[0]);
  c->SetY(pos[1]);
  c->SetZ(pos[2]);
  c->SetImP(pos[3]);
  c->SetQ(amp);
  c->SetSigmaX2(sig[0]);   
  c->SetSigmaY2(sig[1]);   
  c->SetSigmaZ2(sig[2]);
  c->SetSigmaImP2(sig[3]);
  c->SetSigmaYZ(sig[4]);
  c->SetLabelAll(tracks);
//da qui  
//   c->SetSL(wp[0]);
//   c->SetR(wp[1]);
//   c->SetW(wp[2]);
  c->SetId(Id);
//  RecPoints()->Add(c);
  return c;
}

/* //_____________________________________________________________________________
Double_t IlcDCHclusterizer::CalcXposFromTimebin(Float_t timebin, Int_t idet, Int_t col, Int_t row)
{
  //
  // Calculates the local x position in the detector from the timebin, depends on the drift velocity
  // and t0
  //
  
  IlcDCHcalibDB* calibration = IlcDCHcalibDB::Instance();
  if (!calibration)
    return -1;

  Float_t vdrift =1.5; calibration->GetVdrift(idet, col, row);  
  Float_t t0 = calibration->GetT0(idet, col, row);
  Float_t samplingFrequency =50; calibration->GetSamplingFrequency();

  timebin -= t0;

  return timebin / samplingFrequency * vdrift;
}
 */
