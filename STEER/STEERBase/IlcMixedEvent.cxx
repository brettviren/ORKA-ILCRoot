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

/* $Id$ */


//-------------------------------------------------------------------------
//                          Class IlcMixedEvent
// VEvent which is the container of several VEvents 
// Use Case: Event Mixing     
// Origin: Andreas Morsch, CERN, Andreas.Morsch@cern.ch 
//-------------------------------------------------------------------------


#include "IlcMixedEvent.h"
#include "IlcExternalTrackParam.h"
#include "TVector3.h"
#include "IlcVVertex.h"
#include <TMath.h>
#include <TMatrix.h>
#include <TMatrixD.h>
#include "IlcLog.h"
#include "IlcVCaloCells.h"


ClassImp(IlcMixedEvent)


IlcMixedEvent::IlcMixedEvent() :
  IlcVEvent(),
  fEventList(),
  fNEvents(0),       
  fNumberOfTracks(0),
  fNumberOfCaloClusters(0), 
  fNumberOfPHOSCells(0), 
  fNumberOfEMCALCells(0),
  fNTracksCumul(0),
  fNCaloClustersCumul(0),
  fNPHOSCellsCumul(0), 
  fNEMCALCellsCumul(0), 
  fPHOSCells(NULL), 
  fEMCALCells(NULL), 
  fMeanVertex(0)
{
    // Default constructor
}

IlcMixedEvent::IlcMixedEvent(const IlcMixedEvent& Evnt) :
  IlcVEvent(Evnt),
  fEventList(),
  fNEvents(0),
  fNumberOfTracks(0),
  fNumberOfCaloClusters(0), 
  fNumberOfPHOSCells(0), 
  fNumberOfEMCALCells(0),
  fNTracksCumul(0),
  fNCaloClustersCumul(0),
  fNPHOSCellsCumul(0), 
  fNEMCALCellsCumul(0), 
  fPHOSCells(NULL), 
  fEMCALCells(NULL), 
  fMeanVertex(0)
{ } // Copy constructor

IlcMixedEvent& IlcMixedEvent::operator=(const IlcMixedEvent& vEvnt)
{ 
// Assignment operator
    if (this!=&vEvnt) { 
    IlcVEvent::operator=(vEvnt); 
}
  
  return *this; 
}

IlcMixedEvent::~IlcMixedEvent() 
{
    // dtor
  Reset();
  delete fPHOSCells ; 
  delete fEMCALCells ; 
} 


void IlcMixedEvent::AddEvent(IlcVEvent* evt)
{
    // Add a new event to the list
    fEventList.AddLast(evt);
}


void IlcMixedEvent::Init()
{
    // Initialize meta information
  fNEvents = fEventList.GetEntries();
  fNTracksCumul = new Int_t[fNEvents];
  fNumberOfTracks = 0;
  fNCaloClustersCumul = new Int_t[fNEvents];
  fNumberOfCaloClusters = 0;
  fNumberOfPHOSCells    = 0;  
  fNumberOfEMCALCells   = 0; 
  fNPHOSCellsCumul  = new Int_t[fNEvents];
  fNEMCALCellsCumul = new Int_t[fNEvents];

  TIter next(&fEventList);
  IlcVEvent* event;
  Int_t iev = 0;
    
  while((event = (IlcVEvent*)next())) {
    fNTracksCumul[iev] = fNumberOfTracks;
    fNumberOfTracks += (event->GetNumberOfTracks());
    fNCaloClustersCumul[iev] = fNumberOfCaloClusters;
    fNumberOfCaloClusters += event->GetNumberOfCaloClusters(); 
    fNPHOSCellsCumul[iev] = fNumberOfPHOSCells;
    if (event->GetPHOSCells()) 
      fNumberOfPHOSCells += event->GetPHOSCells()->GetNumberOfCells(); 
    fNEMCALCellsCumul[iev] = fNumberOfEMCALCells;
    if (event->GetEMCALCells()) 
      fNumberOfEMCALCells += event->GetEMCALCells()->GetNumberOfCells(); 
    iev++ ;  
  }

  next.Reset() ; 
  Short_t phosPos = 0, emcalPos = 0; 
  Int_t firstPHOSEvent  = kTRUE;
  Int_t firstEMCALEvent = kTRUE;
  
  while((event = (IlcVEvent*)next())) {
    IlcVCaloCells * phosCells = event->GetPHOSCells() ; 
    if (phosCells) {
      
      //Create the container
      if(firstPHOSEvent)
      {
        if(!fPHOSCells) fPHOSCells = phosCells->CopyCaloCells(kFALSE) ;// Just recover the first event type:  ESD/AOD
        else fPHOSCells->DeleteContainer(); //delete the previous container 
        //Now create a new container with the adequate size
        fPHOSCells->SetType(IlcVCaloCells::kPHOSCell) ; 
        fPHOSCells->CreateContainer(fNumberOfPHOSCells) ;
        firstPHOSEvent=kFALSE;

      }//First event

      Int_t ncells = event->GetPHOSCells()->GetNumberOfCells() ;
      for (Int_t icell = 0; icell < ncells; icell++) {
          fPHOSCells->SetCell(phosPos++, phosCells->GetCellNumber(icell), phosCells->GetAmplitude(icell), 
                              phosCells->GetTime(icell),phosCells->GetMCLabel(icell),phosCells->GetEFraction(icell)) ; 
      }
     
    }// phos cells
    
    IlcVCaloCells * emcalCells = event->GetEMCALCells() ; 
    if (emcalCells) {
      
      //Create the container
      if(firstEMCALEvent)
      {
        if(!fEMCALCells)fEMCALCells = emcalCells->CopyCaloCells(kFALSE) ; // Just recover the first event type:  ESD/AOD
        else fEMCALCells->DeleteContainer();       // delete the previous container
        //Now create a new container with the adequate size
        fEMCALCells->SetType(IlcVCaloCells::kEMCALCell) ; 
        fEMCALCells->CreateContainer(fNumberOfEMCALCells) ;
        firstEMCALEvent=kFALSE;
      }//First event
      
      Int_t ncells = emcalCells->GetNumberOfCells() ;
      for (Int_t icell = 0; icell < ncells; icell++) {
          fEMCALCells->SetCell(emcalPos++, emcalCells->GetCellNumber(icell), emcalCells->GetAmplitude(icell), 
                               emcalCells->GetTime(icell),emcalCells->GetMCLabel(icell),emcalCells->GetEFraction(icell)) ; 
      }
    }//EMCAL cells
  }//while event
  
}

IlcVParticle* IlcMixedEvent::GetTrack(Int_t i) const
{
    // Return track # i
    Int_t iEv  = TMath::BinarySearch(fNEvents, fNTracksCumul, i);
    while((iEv < (fNEvents - 1)) && (fNTracksCumul[iEv] == fNTracksCumul[iEv+1])) {iEv++;}

    Int_t irel = i - fNTracksCumul[iEv];
    IlcVEvent* evt = (IlcVEvent*) (fEventList.At(iEv));
    return (evt->GetTrack(irel));
}

IlcVCluster* IlcMixedEvent::GetCaloCluster(Int_t i) const
{
    // Return calo cluster # i
  Int_t iEv  = TMath::BinarySearch(fNEvents, fNCaloClustersCumul, i);
  while((iEv < (fNEvents - 1)) && (fNCaloClustersCumul[iEv] == fNCaloClustersCumul[iEv+1])) {iEv++;}
  
  Int_t irel = i - fNCaloClustersCumul[iEv];
  IlcVEvent* evt = (IlcVEvent*) (fEventList.At(iEv));
  return (evt->GetCaloCluster(irel));
}

const IlcVVertex* IlcMixedEvent::GetEventVertex(Int_t i) const
{
    // Return vertex of track # i
    Int_t iEv  = TMath::BinarySearch(fNEvents, fNTracksCumul, i);
    while((iEv < (fNEvents - 1)) && (fNTracksCumul[iEv] == fNTracksCumul[iEv+1])) {iEv++;}
    IlcVEvent* evt = (IlcVEvent*) (fEventList.At(iEv));
    return (evt->GetPrimaryVertex());
}

const IlcVVertex* IlcMixedEvent::GetVertexOfEvent(Int_t i) const
{
    // Return vertex of event # i
  if (i > fNEvents)
    IlcFatal(Form("%d events in buffer, event %d requested", fNEvents, i)) ;  
  IlcVEvent* evt = (IlcVEvent*) (fEventList.At(i));
  return (evt->GetPrimaryVertex());
}

void IlcMixedEvent::Reset()
{
    // Reset the event
  fEventList.Clear();
  fNEvents = 0;
  fNumberOfTracks = 0;
  fNumberOfCaloClusters = 0;
  fNumberOfPHOSCells = 0;
  fNumberOfEMCALCells = 0;
  if (fNTracksCumul) {
    delete[]  fNTracksCumul;
    fNTracksCumul = 0;
  }
  if (fNCaloClustersCumul) {
    delete[]  fNCaloClustersCumul;
    fNCaloClustersCumul = 0;
  }
  if (fNPHOSCellsCumul) {
    delete[]  fNPHOSCellsCumul;
    fNPHOSCellsCumul = 0;
  }
  if (fNEMCALCellsCumul) {
    delete[]  fNEMCALCellsCumul;
    fNEMCALCellsCumul = 0;
  }
  
  if (fPHOSCells) {	 
    fPHOSCells->DeleteContainer();	 
  }	 
  if (fEMCALCells) {	 
    fEMCALCells->DeleteContainer();	 
  }
  
}

Int_t IlcMixedEvent::EventIndex(Int_t itrack) const
{
  // Return the event index for track #itrack
  return  TMath::BinarySearch(fNEvents, fNTracksCumul, itrack);
}

Int_t IlcMixedEvent::EventIndexForCaloCluster(Int_t icluster) const
{
    // Return the event index for track #itrack
  return  TMath::BinarySearch(fNEvents, fNCaloClustersCumul, icluster);
}

Int_t IlcMixedEvent::EventIndexForPHOSCell(Int_t icell) const
{
    // Return the event index for track #itrack
  return  TMath::BinarySearch(fNEvents, fNPHOSCellsCumul, icell);
}

Int_t IlcMixedEvent::EventIndexForEMCALCell(Int_t icell) const
{
    // Return the event index for track #itrack
  return  TMath::BinarySearch(fNEvents, fNEMCALCellsCumul, icell);
}

Bool_t IlcMixedEvent::ComputeVtx(const TObjArray *vertices, Double_t *pos,Double_t *sig,Int_t *nContributors)  {
//
// Calculate the mean vertex psoitions from events in the buffer
 
    Int_t nentries = vertices->GetEntriesFast();
    Double_t sum[3]={0.,0.,0.};
    Double_t sumsigma[6]={0.,0.,0.,0.,0.,0.};

    
    for(Int_t ivtx = 0; ivtx < nentries; ivtx++){
	IlcVVertex *vtx=(IlcVVertex*)vertices->UncheckedAt(ivtx);
	Double_t covariance[6];
	vtx->GetCovarianceMatrix(covariance);
	Double_t vtxPos[3];
	vtx->GetXYZ(vtxPos);
	if(TMath::Abs(covariance[0])<1.e-13) {
	return kFALSE;
	}else{
	sum[0]+=vtxPos[0]*(1./covariance[0]);
	sumsigma[0]+=(1./covariance[0]);
	}
	if(TMath::Abs(covariance[2])<1.e-13) {
	return kFALSE;
	}else{
	sum[1]+=vtxPos[1]*(1./covariance[2]);
	sumsigma[2]+=(1./covariance[2]);
	}
	if(TMath::Abs(covariance[5])<1.e-13) {
	return kFALSE;
	}else{
	sum[2]+=vtxPos[2]*(1./covariance[5]);
	sumsigma[5]+=(1./covariance[5]);
	}
	if(TMath::Abs(covariance[1])<1.e-13) {
         sumsigma[1]+=0.;
	}else{
	sumsigma[1]+=(1./covariance[1]);
	}
	if(TMath::Abs(covariance[3])<1.e-13) {
	sumsigma[3]+=0.;
	}else{
	sumsigma[3]+=(1./covariance[3]);
	}
	if(TMath::Abs(covariance[4])<1.e-13) {
	sumsigma[4]+=0.;
	}else{
	sumsigma[4]+=(1./covariance[4]);
	}

     nContributors[0]=nContributors[0]+vtx->GetNContributors();
    }
    
    for(Int_t i=0;i<3;i++){
	if(TMath::Abs(sumsigma[i])<1.e-13) continue;
	pos[i]=sum[i]/sumsigma[i];
    }
    for(Int_t i2=0;i2<3;i2++){
	if(TMath::Abs(sumsigma[i2])<1.e-13) {sig[i2]=0.; continue;}
	sig[i2]=1./sumsigma[i2];
    }
    return kTRUE;
}


Double_t IlcMixedEvent::GetMagneticField() const
{
    // Return magnetic field of the first event in the list
    if (fEventList.GetEntries() == 0) return -999.;
    
    IlcVEvent* evt = (IlcVEvent*) (fEventList.At(0));
    return evt->GetMagneticField();
}
