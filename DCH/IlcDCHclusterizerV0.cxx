/**************************************************************************
 * Copyright(c) 1998-1999, ILC Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ILC Off-line Project.                                    *
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

/*
  $Log: IlcDCHclusterizerV0.cxx,v $
  Revision 1.1.1.1  2008/03/11 14:53:01  vitomeg
  Initial commit

  Revision 1.2  2007/07/05 11:04:59  ilcrootcvs
  Fixed Bugs to use DCH and MUD together

  Revision 1.1  2007/04/06 11:37:10  ilcrootcvs
  Drift Chamber

  Revision 1.5  2006/12/21 14:19:00  ilcrootcvs
  DCH Fast Recpoints: Setted (rphi,z) sigma smearing to (200microns,3mm)

  Revision 1.4  2006/12/20 13:23:24  ilcrootcvs
  DCH fast recpoints production with smearing
  
  Revision 1.3  2006/12/20 12:12:23  ilcrootcvs
  DCH Fast RecPoints Production
  
  Revision 1.2  2006/12/15 17:03:15  ilcrootcvs
  Fast RecPoints Production
  
  Revision 1.1  2006/12/15 16:58:19  ilcrootcvs
  Fast Recpoint production
  
  Revision 1.1.4.1  2000/05/08 15:08:41  cblume
  Replace IlcDCHcluster by IlcDCHrecPoint
  
  Revision 1.1  2000/02/28 18:58:33  cblume
  Add new DCH classes
  
*/

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// DCH cluster finder for the fast simulator. It takes the hits from the     //
// fast simulator (one hit per plane) and transforms them                    //
// into cluster, by applying position smearing and merging                   //
// of nearby cluster. The searing is done uniformly in z-direction           //
// over the length of a readout pad. In rphi-direction a Gaussian            //
// smearing is applied with a sigma given by fRphiSigma.                     //
// Clusters are considered as overlapping when they are closer in            //
// rphi-direction than the value defined in fRphiDist.                       //
// Use the macro fastClusterCreate.C to create the cluster.                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TF1.h>
#include <TTree.h>
#include <TH1.h>
#include <TFile.h>
#include <TRandom.h>

#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcLoader.h"
#include "IlcRawReader.h"

#include "IlcDCHclusterizerV0.h"
#include "IlcDCHmatrix.h"
#include "IlcDCHgeometry.h"
#include "IlcDCHdataArrayF.h"
#include "IlcDCHdataArrayI.h"
#include "IlcDCHdigitsManager.h"
#include "IlcDCHpadPlane.h"
#include "IlcDCHrawData.h"
#include "IlcDCHcalibDB.h"
#include "IlcDCHSimParam.h"
#include "IlcDCHRecParam.h"
#include "IlcDCHCommonParam.h"
#include "IlcDCHcluster.h"
#include "IlcDCHhit.h"

ClassImp(IlcDCHclusterizerV0)

  //_____________________________________________________________________________
  IlcDCHclusterizerV0::IlcDCHclusterizerV0():IlcDCHclusterizer()
{
  //
  // IlcDCHclusterizerV0 default constructor
  //

  fDigitsManager = 0;

}

//_____________________________________________________________________________
IlcDCHclusterizerV0::IlcDCHclusterizerV0(const Text_t* name, const Text_t* title)
  :IlcDCHclusterizer(name,title)
{
  //
  // IlcDCHclusterizerV0 default constructor
  //
  fDigitsManager=0;
  //fDigitsManager = new IlcDCHdigitsManager();
  //fDigitsManager->CreateArrays();

  Init();

}


//_____________________________________________________________________________
//_____________________________________________________________________________
void IlcDCHclusterizerV0::Init()
{
  // Position resolution in rphi-direction
  fRphiSigma  = 0.02;
  // Minimum distance of non-overlapping cluster
  fZSigma  = 0.3;


}


//_____________________________________________________________________________
IlcDCHclusterizerV0::IlcDCHclusterizerV0(const IlcDCHclusterizerV0 &c)
  :IlcDCHclusterizer(c)
{
  //
  // IlcDCHclusterizerV0 copy constructor
  //

  ((IlcDCHclusterizerV0 &) c).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHclusterizerV0::~IlcDCHclusterizerV0()
{
  //
  // IlcDCHclusterizerV0 destructor
  //

  if (fDigitsManager) {
    delete fDigitsManager;
    fDigitsManager = NULL;
  }

}

//_____________________________________________________________________________
IlcDCHclusterizerV0 &IlcDCHclusterizerV0::operator=(const IlcDCHclusterizerV0 &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHclusterizerV0 &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHclusterizerV0::Copy(TObject &c) const
{
  //
  // Copy function
  //

  ((IlcDCHclusterizerV0 &) c).fDigitsManager = 0;

  IlcDCHclusterizer::Copy(c);

}

//_____________________________________________________________________________
Bool_t IlcDCHclusterizerV0::ReadDigits()
{
  //
  // Reads the digits arrays from the input ilcroot file
  //

  if (!fRunLoader) {
    printf("<IlcDCHclusterizerV0::ReadDigits> ");
    printf("No input file open\n");
    return kFALSE;
  }
  IlcLoader* loader = fRunLoader->GetLoader("DCHLoader");
  if (!loader->TreeD()) loader->LoadDigits();

  // Read in the digit arrays
  return (fDigitsManager->ReadDigits(loader->TreeD()));

}

//_____________________________________________________________________________
Bool_t IlcDCHclusterizerV0::ReadDigits(IlcRawReader* rawReader)
{
  //
  // Reads the digits arrays from the ddl file
  //

  IlcDCHrawData raw;
  raw.SetDebug(1);

  fDigitsManager = raw.Raw2Digits(rawReader);

  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHclusterizerV0::MakeClusters()
{
  //
  // Generates the cluster.
  //
  Double_t local2[3]={0.,0.,0.}; 
  Int_t nclusters=0;
  Int_t retval = fRunLoader->LoadKinematics();
  if (retval)
    {
      ::Error("fastClusterCreate.C","LoadKinematics returned error");
      delete fRunLoader;
      return kFALSE;
    }

 
  IlcLoader* pDCHloader = fRunLoader->GetLoader("DCHLoader");
  pDCHloader->LoadRecPoints("recreate");


  

  pDCHloader->LoadHits("read");


  Double_t amp=0.;
  Double_t local[3],global[3];

  IlcDCHgeometry *Geo = IlcDCHgeometry::GetGeometry(fRunLoader);  
//  Geo->Init();

 

  TClonesArray *Hitarray = new TClonesArray("IlcDCHhit",1560);
  IlcDCHhit *Hit;


  
 
  TTree *treeH=pDCHloader->TreeH();
 
  Char_t branchname[100];
  sprintf(branchname,"DCH");
  TBranch *branch = treeH->GetBranch(branchname);
  if (!branch) {
    Error("FillModules","%s branch in TreeH not found",branchname);
    return kFALSE;
  } // end if !branch
    
  branch->SetAddress(&Hitarray);

  Int_t nTracks =(Int_t) treeH->GetEntries();

  printf("nTracks = %d\n", nTracks); 

  Int_t ClusterTrack[9];
  Double_t ClusterSig[2];

  ClusterSig[0]=fRphiSigma*fRphiSigma;
  ClusterSig[1]=fZSigma*fZSigma;




        
  Float_t yNearest=100.;	
  // Loop through all entries in the tree
  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {

    gIlc->ResetHits();
    treeH->GetEvent(iTrack);

    // Get the number of hits in the DCH created by this particle
    Int_t nHit = Hitarray->GetEntriesFast();

    // Loop through the DCH hits  
    for (Int_t iHit = 0; iHit < nHit; iHit++) {

      Hit=(IlcDCHhit *) Hitarray->UncheckedAt(iHit);
      Float_t pos[3];
      pos[0]   = Hit->X();
      pos[1]   = Hit->Y();
      pos[2]   = Hit->Z();
      amp      = Hit->GetCharge();
      Int_t   track    = Hit->GetTrack();
      Int_t   detector = Hit->GetDetector();
      Float_t   yPos_local=Hit->GetPosY();
      if ((TMath::Abs(yPos_local)>yNearest) && (Hit->GetStatus()!=2)) continue;
      if (Hit->GetStatus()!=2) yNearest=TMath::Abs(yPos_local);
	    

      Int_t isect = Geo->GetSector(detector);  
      Float_t r=(Geo->Rmax() - Geo->Rmin())/2.;
      if ((isect%2)!=0) {
	Float_t radius=Geo->Rmin()*TMath::Cos(Geo->GetAlpha1())/TMath::Cos(Geo->GetAlpha2())+r*TMath::Cos(30.*TMath::DegToRad());
	    
	global[0]=pos[0]-(radius*TMath::Sin(((-360./Geo->Nsect()) * ((Float_t) isect)+210.)*TMath::DegToRad()));
	global[1]=pos[1]-(radius*TMath::Cos(((-360./Geo->Nsect()) * ((Float_t) isect)+210.)*TMath::DegToRad()));
	global[2]=pos[2];
	Geo->Rotate(detector,global,local);
      }
      else {
	Float_t radius=Geo->Rmin()+r;
	    
	global[0]=pos[0]-(radius*TMath::Sin(((-360./Geo->Nsect()) * ((Float_t) isect)+210.)*TMath::DegToRad()));
	global[1]=pos[1]-(radius*TMath::Cos(((-360./Geo->Nsect()) * ((Float_t) isect)+210.)*TMath::DegToRad()));
	global[2]=pos[2];
	Geo->Rotate(detector,global,local);
      }
      for (Int_t i=0;i<9;i++) ClusterTrack[i]=track;
            
      Double_t deltaXl, deltaZl;
      deltaXl = gRandom->Gaus(0, fRphiSigma);
      deltaZl = gRandom->Gaus(0, fZSigma);
      if (Hit->GetStatus()!=2) {
	local2[0]=local[0]+deltaXl;;
	local2[1]=local[1];
	local2[2]=local[2]+deltaZl;;

      }
      if (Hit->GetStatus()==2) { 
        AddCluster(local2,1,detector,amp,ClusterTrack,ClusterSig,0);
	nclusters++;
	WriteClusters(detector);
	ResetRecPoints();	 
        yNearest=100.;
      }
    }
  }
	
  printf("Number clusters: %d\n",nclusters);
  if (fVerbose > 0) {
    printf("<IlcDCHclusterizerV0::MakeCluster> ");
    printf("Done.\n");
  }




  return kTRUE;


}

//_____________________________________________________________________________
Double_t IlcDCHclusterizerV0::GetCOG(Double_t signal[5])
{
  //
  // get COG position
  // used for clusters with more than 3 pads - where LUT not applicable
  Double_t sum = signal[0]+signal[1]+signal[2]+signal[3]+signal[4];
  Double_t res = (0.0*(-signal[0]+signal[4])+(-signal[1]+signal[3]))/sum;
  return res;		  
}

//_____________________________________________________________________________
Double_t IlcDCHclusterizerV0::Unfold(Double_t eps, Int_t plane, Double_t* padSignal)
{
  //
  // Method to unfold neighbouring maxima.
  // The charge ratio on the overlapping pad is calculated
  // until there is no more change within the range given by eps.
  // The resulting ratio is then returned to the calling method.
  //

  IlcDCHcalibDB* calibration = IlcDCHcalibDB::Instance();
  if (!calibration)
    {
      printf("<IlcDCHclusterizerMI::Unfold> ");
      printf("ERROR getting instance of IlcDCHcalibDB");
      return kFALSE;  
    }
  
  Int_t   irc                = 0;
  Int_t   itStep             = 0;      // Count iteration steps

  Double_t ratio             = 0.5;    // Start value for ratio
  Double_t prevRatio         = 0;      // Store previous ratio

  Double_t newLeftSignal[3]  = {0};    // Array to store left cluster signal
  Double_t newRightSignal[3] = {0};    // Array to store right cluster signal
  Double_t newSignal[3]      = {0};

  // Start the iteration
  while ((TMath::Abs(prevRatio - ratio) > eps) && (itStep < 10)) {

    itStep++;
    prevRatio = ratio;

    // Cluster position according to charge ratio
    Double_t maxLeft  = (ratio*padSignal[2] - padSignal[0]) 
      / (padSignal[0] + padSignal[1] + ratio*padSignal[2]);
    Double_t maxRight = (padSignal[4] - (1-ratio)*padSignal[2]) 
      / ((1-ratio)*padSignal[2] + padSignal[3] + padSignal[4]);

    // Set cluster charge ratio
    irc = calibration->PadResponse(1.0,maxLeft ,plane,newSignal);
    Double_t ampLeft  = padSignal[1] / newSignal[1];
    irc = calibration->PadResponse(1.0,maxRight,plane,newSignal);
    Double_t ampRight = padSignal[3] / newSignal[1];

    // Apply pad response to parameters
    irc = calibration->PadResponse(ampLeft ,maxLeft ,plane,newLeftSignal );
    irc = calibration->PadResponse(ampRight,maxRight,plane,newRightSignal);

    // Calculate new overlapping ratio
    ratio = TMath::Min((Double_t)1.0,newLeftSignal[2] / 
		       (newLeftSignal[2] + newRightSignal[0]));

  }

  return ratio;

}

//_____________________________________________________________________________
void IlcDCHclusterizerV0::Transform(IlcDCHdataArrayI* digitsIn,
				    IlcDCHdataArrayF* digitsOut,
				    Int_t idet, Int_t nRowMax,
				    Int_t nColMax, Int_t nTimeTotal,
				    Float_t ADCthreshold)
{

  //
  // Apply gain factor
  // Apply tail cancellation: Transform digitsIn to digitsOut
  //


  IlcDCHRecParam* recParam = IlcDCHRecParam::Instance();
  if (!recParam)
    {
      printf("<IlcDCHclusterizerV0::Transform> ");
      printf("ERROR getting instance of IlcDCHRecParam");
      return;
    }
  //IlcDCHcalibDB* calibration = IlcDCHcalibDB::Instance();

  Double_t *inADC  = new Double_t[nTimeTotal];  // adc data before tail cancellation
  Double_t *outADC = new Double_t[nTimeTotal];  // adc data after tail cancellation

  if (fVerbose > 0) {
    printf("<IlcDCHclusterizerV0::Transform> ");
    printf("Tail cancellation (nExp = %d) for detector %d.\n",
	   recParam->GetTCnexp(),idet);
  }

  for (Int_t iRow  = 0; iRow  <  nRowMax;   iRow++ ) {
    for (Int_t iCol  = 0; iCol  <  nColMax;   iCol++ ) {
      for (Int_t iTime = 0; iTime < nTimeTotal; iTime++) {
	//
	// add gain
	//
	Double_t gain =0.5;// calibration->GetGainFactor(idet, iCol, iRow);
	if (gain==0) {
	  IlcError("Not a vilcd gain\n");
	}
	inADC[iTime]  = digitsIn->GetDataUnchecked(iRow, iCol, iTime);

        inADC[iTime] /= gain;
        outADC[iTime] = inADC[iTime];
      }

      // Apply the tail cancelation via the digital filter
      if (recParam->TCOn())
	{
	  DeConvExp(inADC,outADC,nTimeTotal,recParam->GetTCnexp());
	}

      for (Int_t iTime = 0; iTime < nTimeTotal; iTime++) {
	// Store the amplitude of the digit if above threshold
	if (outADC[iTime] > ADCthreshold) {
	  if (fVerbose > 1)
	    {
	      printf("  iRow = %d, iCol = %d, iTime = %d, adc = %f\n"
		     ,iRow,iCol,iTime,outADC[iTime]);
	    }
	  digitsOut->SetDataUnchecked(iRow,iCol,iTime,outADC[iTime]);
	}

      }

    }

  }

  delete [] inADC;
  delete [] outADC;

  return;

}


//_____________________________________________________________________________
void IlcDCHclusterizerV0::DeConvExp(Double_t *source, Double_t *target,
				    Int_t n, Int_t nexp) 
{
  //
  // Tail Cancellation by Deconvolution for PASA v4 TRF
  //

  Double_t rates[2];
  Double_t coefficients[2];

  // initialize (coefficient = alpha, rates = lambda)

  Double_t R1 = 1.0;
  Double_t R2 = 1.0;
  Double_t C1 = 0.5;
  Double_t C2 = 0.5;

  if (nexp == 1) {   // 1 Exponentials
    R1 = 1.156;
    R2 = 0.130;
    C1 = 0.066;
    C2 = 0.000;
  }
  if (nexp == 2) {   // 2 Exponentials
    R1 = 1.156;
    R2 = 0.130;
    C1 = 0.114;
    C2 = 0.624;
  }

  coefficients[0] = C1;
  coefficients[1] = C2;

  Double_t Dt = 0.100;

  rates[0] = TMath::Exp(-Dt/(R1));
  rates[1] = TMath::Exp(-Dt/(R2));
  
  Int_t i, k;
  Double_t reminder[2];
  Double_t correction, result;

  /* attention: computation order is important */
  correction=0.0;

  for ( k=0; k<nexp; k++ ) reminder[k]=0.0;

  for ( i=0; i<n; i++ ) {
    result = ( source[i] - correction );    // no rescaling
    target[i] = result;

    for ( k=0; k<nexp; k++ ) reminder[k] = rates[k] *
			       ( reminder[k] + coefficients[k] * result);
    correction=0.0;
    for ( k=0; k<nexp; k++ ) correction += reminder[k];
  }

}
