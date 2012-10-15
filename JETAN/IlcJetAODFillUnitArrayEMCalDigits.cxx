
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
 
//======================================================================
// Fill Unit Array 
// Called by AOD reader for jet analysis
// Author: Magali Estienne (magali.estienne@ires.in2p3.fr)
//======================================================================

class TSystem;
class TLorentzVector;
class TGeoManager;
class TArrayS;

// --- IlcRoot header files ---
#include "IlcJetUnitArray.h"
#include "IlcJetAODFillUnitArrayEMCalDigits.h"
class IlcJetFinder;

// #include "IlcEMCALCalibData.h"
// #include "IlcCDBManager.h"
// class IlcCDBStorage;
// #include "IlcCDBEntry.h"


ClassImp(IlcJetAODFillUnitArrayEMCalDigits)

//_____________________________________________________________________________
IlcJetAODFillUnitArrayEMCalDigits::IlcJetAODFillUnitArrayEMCalDigits()
  : IlcJetFillUnitArray(),
    fAOD(0),
    fNIn(0),
    fCluster(0),
    fNCEMCAL(0),
    fNCPHOS(0),
    fNCCalo(0),
    fApplyElectronCorrection(kFALSE),
    fApplyFractionHadronicCorrection(kFALSE),
    fFractionHadronicCorrection(0.3),
    fClus(0x0),
    fNDigitEmcal(0),
    fNDigitEmcalCut(0)
{
  // constructor
}

//_____________________________________________________________________________
IlcJetAODFillUnitArrayEMCalDigits::IlcJetAODFillUnitArrayEMCalDigits(IlcAODEvent *aod)
  : IlcJetFillUnitArray(),
    fAOD(aod),
    fNIn(0),
    fCluster(0),
    fNCEMCAL(0),
    fNCPHOS(0),
    fNCCalo(0),
    fApplyElectronCorrection(kFALSE),
    fApplyFractionHadronicCorrection(kFALSE),
    fFractionHadronicCorrection(0.3),
    fClus(0x0),
    fNDigitEmcal(0),
    fNDigitEmcalCut(0)
{
  // constructor
}

//_____________________________________________________________________________
IlcJetAODFillUnitArrayEMCalDigits::IlcJetAODFillUnitArrayEMCalDigits(const IlcJetAODFillUnitArrayEMCalDigits &det)
  : IlcJetFillUnitArray(det),
    fAOD(det.fAOD),
    fNIn(det.fNIn),
    fCluster(det.fCluster),
    fNCEMCAL(det.fNCEMCAL),
    fNCPHOS(det.fNCPHOS),
    fNCCalo(det.fNCCalo),
    fApplyElectronCorrection(det.fApplyElectronCorrection),
    fApplyFractionHadronicCorrection(det.fApplyFractionHadronicCorrection),
    fFractionHadronicCorrection(det.fFractionHadronicCorrection),
    fClus(det.fClus),
    fNDigitEmcal(det.fNDigitEmcal),
    fNDigitEmcalCut(det.fNDigitEmcalCut)
{
  // Copy constructor
}

//_____________________________________________________________________________
IlcJetAODFillUnitArrayEMCalDigits& IlcJetAODFillUnitArrayEMCalDigits::operator=(const IlcJetAODFillUnitArrayEMCalDigits& other)
{
  // Assignment

  fAOD = other.fAOD;
  fNIn = other.fNIn;
  fCluster = other.fCluster;
  fNCEMCAL = other.fNCEMCAL;
  fNCPHOS = other.fNCPHOS;
  fNCCalo = other.fNCCalo;
  fApplyElectronCorrection = other.fApplyElectronCorrection;
  fApplyFractionHadronicCorrection = other.fApplyFractionHadronicCorrection;
  fFractionHadronicCorrection = other.fFractionHadronicCorrection;
  fClus = other.fClus;
  fNDigitEmcal = other.fNDigitEmcal;
  fNDigitEmcalCut = other.fNDigitEmcalCut;

  return (*this);

}

//_____________________________________________________________________________
IlcJetAODFillUnitArrayEMCalDigits::~IlcJetAODFillUnitArrayEMCalDigits()
{
  // destructor
}



//_____________________________________________________________________________
void IlcJetAODFillUnitArrayEMCalDigits::Exec(Option_t* const /*option*/)
//void IlcJetAODFillUnitArrayEMCalDigits::Exec(Option_t* option)
{
  //
  // Main method.
  // Fill the unit array with the neutral particle information from the EMCal cells in AOD

  fDebug = fReaderHeader->GetDebug();
  fOpt = fReaderHeader->GetDetector();
  fCluster = fReaderHeader->GetCluster();

  // Init parameters
  //  InitParameters();

  Int_t   goodDigit      = 0;
  Int_t   index          = 0;

  if(!fCluster) { // Keep all digit information

    // Loop over all cell information
    //------------------------------------------------------------------
    IlcAODCaloCells &cells= *(fAOD->GetEMCALCells());
    Int_t ncell = cells.GetNumberOfCells() ;
//(not used ?)    Int_t type = cells.GetType();

    for (Int_t icell=  0; icell <  ncell; icell++) {
      Int_t   digitID   = cells.GetCellNumber(icell);
      Float_t digitAmp  = cells.GetAmplitude(icell);
      Float_t digitEn   = digitAmp*0.0153; // Last correct
      //  Float_t digitEn = Calibrate(digitAmp,digitID);

      Float_t etaD=-10., phiD=-10.;
//       fGeom->EtaPhiFromIndex(digitID,etaD,phiD); 
      //  fEMCalGrid->GetEtaPhiFromIndex2(digitID,phiD,etaD);

      phiD = ((phiD < 0) ? phiD + 2.* TMath::Pi() : phiD);
      
      //      Float_t digitEt = digitEn*TMath::Abs(TMath::Sin(EtaToTheta(etaD)));

      IlcJetUnitArray *uArray = (IlcJetUnitArray*)fUnitArray->At(digitID);

      if(uArray->GetUnitEnergy() == 0.) goodDigit++;
      uArray->SetUnitTrackID(digitID);
      
      Float_t unitEnergy = 0.;
      Bool_t ok = kFALSE;
      unitEnergy = uArray->GetUnitEnergy();

      if(unitEnergy==0){
	if(!fProcId){
	  new(fRefArray) TRefArray(TProcessID::GetProcessWithUID(uArray));
	  fProcId = kTRUE;
	}
	fRefArray->Add(uArray);
	fNDigitEmcal++;
	ok = kTRUE;
      }

      // Hadronic Correction
      double correction=0;
// Temporarily commented - will be updated removing the IlcAODpid dependence
/*
      if (fApplyFractionHadronicCorrection) {
        TArrayS* matched = new TArrayS();
	GetTracksPointingToCell(matched, etaD, phiD,0.02);

	double ptot = 0;
	for(int itrack = 0; itrack <  matched->GetSize(); itrack++)
	  {
	    const short indexS = matched->At(itrack);
	    if (indexS>-1)
	      {	
		IlcAODTrack *track = fAOD->GetTrack(indexS);
		ptot += track->P();
	      }
	  }
	
	correction = ptot * fFractionHadronicCorrection;

	if (ptot>0 && fDebug>1 ) {
	   printf("SUM of track momentum for this cell: p=%f \n", ptot);	
	   printf("fractional correction=%f \n", fFractionHadronicCorrection);
	   printf("TOTAL correction=%f \n", correction );
	}
	
	delete matched;
	
      }//end hadronic correction
*/
      double enerCorr = digitEn;
      if (correction >= enerCorr) enerCorr = 0;
      else enerCorr -= correction;
      if (correction>0 && fDebug>1) {
        printf("IlcJetAODFillUnitArrayEMCALDigits---Hadronic Correction: uncorrected E=%f, corrected E=%f \n", digitEn, enerCorr);
      }

      Float_t digitEt = enerCorr*TMath::Abs(TMath::Sin(EtaToTheta(etaD)));

      // Detector flag
      if(unitEnergy>0.)
	uArray->SetUnitDetectorFlag(kAll);
      else uArray->SetUnitDetectorFlag(kEmcal);
      
      uArray->SetUnitEnergy(unitEnergy+digitEt);

      uArray->SetUnitCutFlag(kPtHigher);

      // To be modified !!!
      uArray->SetUnitSignalFlag(kGood);
	
      // This is for jet multiplicity
      uArray->SetUnitClusterID(index);
	
      if(fDebug > 12) printf("goodDigit : %d\n", goodDigit);
      
    } // End loop over cells
  } // end if !fCluster
  else { // Keep digit information from clusterization

    // Loop over calo clusters
    //------------------------------------------------------------------

    // select EMCAL clusters only
    TRefArray * caloClusters  = new TRefArray();
    fAOD->GetEMCALClusters(caloClusters);

    // Total number of EMCAL cluster
    Int_t nclus = caloClusters->GetEntries() ;
    Int_t beg   = 0;
    Float_t pos[3] ;

    // Get CaloCells
    IlcAODCaloCells &cells= *(fAOD->GetEMCALCells());

    for(Int_t j = beg; j < nclus; j++) { // loop over clusters
      // Retrieve cluster from aod
      fClus = (IlcAODCaloCluster *) caloClusters->At(j) ;

      // Get the cluster info

      fClus->GetPosition(pos) ;
      TVector3 vpos(pos[0],pos[1],pos[2]) ;

      Int_t     digMult = fClus->GetNCells() ;
      UShort_t *digID   = fClus->GetCellsAbsId() ;
      Int_t     trackIndex = -1;
      if(fClus->GetNTracksMatched()!=0) 
	trackIndex = ((IlcAODTrack*)fClus->GetTrackMatched(0))->GetID();

      // Do double-counted electron correction 
      if (fApplyElectronCorrection != 0 && trackIndex !=-1 )
	{
          // The electron correction go there
          // Under construction !!!!
	}  // End of Electron correction

      // Get CaloCells of cluster and fill the unitArray
      for(Int_t i = 0; i < digMult ; i++) {
        Int_t    digitID      = digID[i]; // or clus->GetCellNumber(i) ;
        Float_t  digitAmp     = cells.GetCellAmplitude(digitID) ;

        // Calibration for an energy in GeV
        Float_t digitEn = digitAmp*0.0153;

        Float_t etaD=-10., phiD=-10.;
//         fGeom->EtaPhiFromIndex(digitID,etaD,phiD);
        //  fEMCalGrid->GetEtaPhiFromIndex2(digitID,phiD,etaD);

        phiD = ((phiD < 0) ? phiD + 2.* TMath::Pi() : phiD);

	// Hadronic Correction
	double correction=0;
// Temporarily commented - will be updated removing the IlcAODpid dependence
/*
	if (fApplyFractionHadronicCorrection) {
	  TArrayS* matched = new TArrayS();
	  GetTracksPointingToCell(matched, etaD, phiD,0.02);
	  
	  double ptot = 0;
	  for(int itrack = 0; itrack <  matched->GetSize(); itrack++)
	    {
	      const short indexS = matched->At(itrack);
	      if (indexS>-1)
		{	
		  IlcAODTrack *track = fAOD->GetTrack(indexS);
		  ptot += track->P();
		}
	    }
	
	  correction = ptot * fFractionHadronicCorrection;

	  if (ptot>0 && fDebug>1 ) {
	    printf("SUM of track momentum for this cell: p=%f \n", ptot);	
	    printf("fractional correction=%f \n", fFractionHadronicCorrection);
	    printf("TOTAL correction=%f \n", correction );
	  }
	
	  delete matched;
	
	}//end hadronic correction
*/	
	double enerCorr = digitEn;
	if (correction >= enerCorr) enerCorr = 0;
	else enerCorr -= correction;
	if (correction>0 && fDebug>1) {
	  printf("IlcJetAODFillUnitArrayEMCALDigits---Hadronic Correction: uncorrected E=%f, corrected E=%f \n", digitEn, enerCorr);
	}

	Float_t digitEt = enerCorr*TMath::Abs(TMath::Sin(EtaToTheta(etaD)));

	IlcJetUnitArray *uArray = (IlcJetUnitArray*)fUnitArray->At(digitID);
	if(uArray->GetUnitEnergy() == 0.) goodDigit++;
	uArray->SetUnitTrackID(digitID);

	Float_t unitEnergy = 0.;
	Bool_t ok = kFALSE;
	unitEnergy = uArray->GetUnitEnergy();

	if(unitEnergy==0){
	  if(!fProcId){
	    new(fRefArray) TRefArray(TProcessID::GetProcessWithUID(uArray));
	    fProcId = kTRUE;
	  }
	  fRefArray->Add(uArray);
	  fNDigitEmcal++;
	  ok = kTRUE;
	}

	// Detector flag
	if(unitEnergy>0.)
	  uArray->SetUnitDetectorFlag(kAll);
	else uArray->SetUnitDetectorFlag(kEmcal);
      
	uArray->SetUnitEnergy(unitEnergy+digitEt);

	uArray->SetUnitCutFlag(kPtHigher);

	// Signal or background
	// To be modified !!!
	uArray->SetUnitSignalFlag(kGood);

	// This is for jet multiplicity
	uArray->SetUnitClusterID(index);
	
	if(fDebug > 12) printf("goodDigit : %d\n", goodDigit);

      }// End loop over cells
    } // End loop over clusters
  } // end else

  fNIn += goodDigit;

  if(fDebug>1)
    {
      printf("End of digits %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
      printf("goodDigit : %d\n", goodDigit);
    }
}

////____________________________________________________________________________
//void IlcJetAODFillUnitArrayEMCalDigits::GetTracksPointingToCell(TArrayS* array,Double_t eta, Double_t phi, Double_t cut)
//{ // Temporarily commented -> will be corrected removing the dependence to IlcAODPid
//// Get all tracks pointing to cell 
//  int size=0;
//  
//  for (Int_t itrk =  0; itrk <  fAOD->GetNumberOfTracks() ; itrk++) { //track loop
//  
//    IlcAODTrack * track = (IlcAODTrack*) fAOD->GetTrack(itrk) ;  
//    IlcAODPid*    pid   = (IlcAODPid*) track->GetDetPid();
//    
//    if(pid) {
//      Double_t emcpos[3];
//      pid->GetEMCALPosition(emcpos);      
//      TVector3 tpos(emcpos[0],emcpos[1],emcpos[2]);
//
//      Double_t deta = tpos.Eta() - eta;
//      Double_t dphi = tpos.Phi() - phi;
//
//      if(dphi > TMath::Pi()) dphi -= 2*TMath::Pi();
//      if(dphi < -TMath::Pi()) dphi += 2*TMath::Pi();
//
//      Double_t res = sqrt(dphi*dphi + deta*deta);
//      
//      if (res< cut) {
//        //add this track-index
//        size++;
//        array->Set( size );
//        array->AddAt( itrk, (size-1) ); 
//	if(fDebug>1) printf("MTH:: track %d matched cell at eta=%f , phi=%f \n", itrk, eta, phi);
//		      
//      }
//    }
//  }
//
//}

/*
//____________________________________________________________________________
void IlcJetAODFillUnitArrayEMCalDigits::GetCalibrationParameters()
{
  // Set calibration parameters:
  // if calibration database exists, they are read from database,
  // otherwise, they are taken from digitizer.
  //
  // It is a user responsilibity to open CDB before reconstruction,
  // for example:
  // IlcCDBStorage* storage = IlcCDBManager::Instance()->GetStorage("local://CalibDB");

  //Check if calibration is stored in data base

  if(!fCalibData && (IlcCDBManager::Instance()->IsDefaultStorageSet()))
    {
      IlcCDBEntry *entry = (IlcCDBEntry*)
        IlcCDBManager::Instance()->Get("EMCAL/Calib/Data");
      if (entry) fCalibData =  (IlcEMCALCalibData*) entry->GetObject();
    }

  if(!fCalibData)
    printf("************* Calibration parameters not found in CDB! ****************");
//    IlcFatal("Calibration parameters not found in CDB!");


}

//____________________________________________________________________________
Float_t  IlcJetAODFillUnitArrayEMCalDigits::Calibrate(Int_t amp, Int_t AbsId)
{

  // Convert digitized amplitude into energy.
  // Calibration parameters are taken from calibration data base for raw data,
  // or from digitizer parameters for simulated data.

  if(fCalibData){

    if (fGeom==0)
      printf("************* Did not get geometry from EMCALLoader ***************");
//      IlcFatal("Did not get geometry from EMCALLoader") ;

    Int_t iSupMod = -1;
    Int_t nModule  = -1;
    Int_t nIphi   = -1;
    Int_t nIeta   = -1;
    Int_t iphi    = -1;
    Int_t ieta    = -1;

    Bool_t bCell = fGeom->GetCellIndex(AbsId, iSupMod, nModule, nIphi, nIeta) ;
    if(!bCell) {
      // fGeom->PrintGeometry();
      Error("Calibrate()"," Wrong cell id number : %i", AbsId);
      assert(0);
    }

    fGeom->GetCellPhiEtaIndexInSModule(iSupMod,nModule,nIphi, nIeta,iphi,ieta);

    fADCchannelECA  = fCalibData->GetADCchannel (iSupMod,ieta,iphi);
    fADCpedestalECA = fCalibData->GetADCpedestal(iSupMod,ieta,iphi);

   return -fADCpedestalECA + amp * fADCchannelECA ;

  }
  else //Return energy with default parameters if calibration is not available
    return -fADCpedestalECA + amp * fADCchannelECA ;

}
*/


