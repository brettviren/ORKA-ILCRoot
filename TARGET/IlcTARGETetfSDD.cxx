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

/* $Id: IlcTARGETetfSDD.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */


#include <Riostream.h>
#include <TMath.h>
#include "IlcTARGETetfSDD.h"

////////////////////////////////////////////////////////////////////////
// Version: 0
// Written by Piergiorgio Cerello
// November 23 1999
// Revised to comply with coding conventions: Nov, 21 2003 m.m.
//_____________________________________________________________________________


ClassImp(IlcTARGETetfSDD)

const Int_t IlcTARGETetfSDD::IlcTARGETetfSDDparam::fgkMaxNofPoles = 5;
const Int_t IlcTARGETetfSDD::IlcTARGETetfSDDparam::fgkMaxNofSamples = 1024;

Int_t ppower(Int_t b, Int_t e) {
  Int_t power = 1;
  for(Int_t i=0; i<e; i++) power *= b;
  return power;
}

IlcTARGETetfSDD::IlcTARGETetfSDD() {
  // Default constructor
  fTimeDelay = 0.;
  fSamplingTime = 0.;
  fT0 = 0.;
  fDf = 0.;
  fA0 = 0.;
  fZeroM = 0;
  fZeroR = 0;
  fPoleM = 0;
  fPoleR = 0;
  fPoleI = 0;
  fTfR = 0;
  fTfI = 0;
  fWR = 0;
  fWI = 0;
}

IlcTARGETetfSDD::IlcTARGETetfSDD(Double_t timestep, Int_t amplif)
{
  // Standard constructor. sampling time in ns

  /*
  cout<<"Number of poles: "<<IlcTARGETetfSDDparam::NumberOfPoles()<<endl;
  cout<<"Number of samples: "<<IlcTARGETetfSDDparam::NumberOfSamples()<<endl;
  */
  fTimeDelay = 53.5;
  if(amplif == 2) fTimeDelay = 35.5;
  fSamplingTime = timestep;

  fT0 = 0.;
  fDf = ppower(10,9)/(IlcTARGETetfSDDparam::NumberOfSamples()*fSamplingTime);

  Int_t i,j;
  fZeroM = new Double_t[IlcTARGETetfSDDparam::NumberOfPoles()];
  fZeroR = new Double_t [IlcTARGETetfSDDparam::NumberOfPoles()];
  fZeroI = new Double_t [IlcTARGETetfSDDparam::NumberOfPoles()];
  fPoleM = new Double_t [IlcTARGETetfSDDparam::NumberOfPoles()];
  fPoleR = new Double_t [IlcTARGETetfSDDparam::NumberOfPoles()];
  fPoleI = new Double_t [IlcTARGETetfSDDparam::NumberOfPoles()];
  fTfR = new Double_t [IlcTARGETetfSDDparam::NumberOfSamples()];
  fTfI = new Double_t [IlcTARGETetfSDDparam::NumberOfSamples()];
  fWR = new Double_t [IlcTARGETetfSDDparam::NumberOfSamples()];
  fWI = new Double_t [IlcTARGETetfSDDparam::NumberOfSamples()];

  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) {
    fZeroM[i] = 0.;
    fZeroR[i] = 0.;
    fZeroI[i] = 0.;
    fPoleM[i] = 0.;
    fPoleR[i] = 0.;
    fPoleI[i] = 0.;
  }
  // Ilc

  // PASCAL amplif
  fA0 = 5.53269815e+11; 
  fPoleM[0] = 3.;
  fPoleR[0] = -8280000.; 
  fPoleI[0] = 0.; 

  if(amplif == 2) { // OLA amplif.
    fA0 = 24000.;
    fPoleM[0] = 1.;
    fPoleR[0] = -3000000.;
    fPoleI[0] = 4000000.;
    fPoleM[1] = 1.;
    fPoleR[1] = fPoleR[0];
    fPoleI[1] = -fPoleI[0]; 
  }

  if( amplif == 3 ) { // old PASCAL
    fA0 = 16500.; // AL: 16500.;  // TB: 24000.; // 26000.; // 24000.; // 18000.; 
    fPoleM[0] = 1.;
    fPoleR[0] = -4140000.; // AL: -4140000.; // TB: -3000000.; // -3750000.; // -3500000; // -3000000.; 
    fPoleI[0] = 0.; // AL: 0.; // TB: 4000000.; // 3750000.; // 3500000.; // 3000000.; 
    fPoleM[1] = 1.;
    fPoleR[1] = fPoleR[0];
    fPoleI[1] = -fPoleI[0]; 
  }

  //cout << "fA0: " << fA0 << endl;
  //cout << "fTimeDelay: " << fTimeDelay << endl;
  
  // Compute Transfer Function

  Double_t pigr = acos(-1.);
  for(i=0; i<=IlcTARGETetfSDDparam::NumberOfSamples()/2; i++) {
    Double_t frequency = fDf*i;
    Double_t vVM = fA0;
    Double_t vVA = 0.;
    for(Int_t k=0; k<IlcTARGETetfSDDparam::NumberOfPoles(); k++) {
      if(fZeroM[k]) {
        Double_t vVZR = -fZeroR[k];
        Double_t vVZI = frequency - fZeroI[k];
        Double_t vVZM = TMath::Sqrt(vVZR*vVZR+vVZI*vVZI);
        Double_t vVZA = TMath::ATan2(vVZI,vVZR);
	//	cout << "VZM: " << vVZM << ", VZA: " << vVZA << endl;
	//	cout << "VZR: " << vVZR << ", VZI: " << vVZI << endl;
        for(j=1; j<= (Int_t) fZeroM[k]; j++) {
          vVM *= vVZM;
          vVA += vVZA;
          if(vVA >= pigr) vVA -= (2.*pigr);
          if(vVA <= -pigr) vVA += (2.*pigr);
	  //cout << "vVM: " << vVM << ", VA: " << vVA << endl;
        }
      }

      if(fPoleM[k]) {
        Double_t vVPR = -fPoleR[k];
        Double_t vVPI = frequency - fPoleI[k];
	Double_t vVPM = TMath::Sqrt(vVPR*vVPR+vVPI*vVPI);
	Double_t vVPA = TMath::ATan2(vVPI,vVPR);
	//cout << "VPM: " << vVPM << ", VPA: " << vVPA << endl;
	//cout << "VPR: " << vVPR << ", VPI: " << vVPI << endl;
        for(j=1; j<= (Int_t) fPoleM[k]; j++) {
          vVM /= vVPM;
          vVA -= vVPA;
          if(vVA >= pigr) vVA -= (2.*pigr);
          if(vVA <= -pigr) vVA += (2.*pigr);
	  //cout << "VM: " << vVM << ", vVA: " << vVA << endl;
        }
      }
      Double_t vVR = vVM*cos(vVA);
      Double_t vVI = vVM*sin(vVA);
      //cout << "VM: " << vVM << ", VA: " << vVA << endl;
      //cout << "VR: " << vVR << ", VI: " << vVI << endl;
      fTfR[i] = vVR*ppower(10,9);
      fTfI[i] = vVI*ppower(10,9);
      //cout << "fTfR[" << i << "] = " << fTfR[i] << endl;
      //cout << "fTfI[" << i << "] = " << fTfI[i] << endl;
      if(i) {
        fTfR[IlcTARGETetfSDDparam::NumberOfSamples()-i] = fTfR[i];
        fTfI[IlcTARGETetfSDDparam::NumberOfSamples()-i] = -fTfI[i];
      }
    }
  }
  
  // Compute Fourier Weights

  for(i=0; i<=IlcTARGETetfSDDparam::NumberOfSamples()/2; i++) {
    fWR[i] = cos(-2.*pigr*i/IlcTARGETetfSDDparam::NumberOfSamples());
    fWI[i] = sin(-2.*pigr*i/IlcTARGETetfSDDparam::NumberOfSamples());
    if(i) {
      fWR[IlcTARGETetfSDDparam::NumberOfSamples()-i] = fWR[i];
      fWI[IlcTARGETetfSDDparam::NumberOfSamples()-i] = -fWI[i];
    }
  }

}

//______________________________________________________________________
IlcTARGETetfSDD::IlcTARGETetfSDD(const IlcTARGETetfSDD &obj) : TObject(obj) {
  // Copy constructor
  // Copies are not allowed. The method is protected to avoid misuse.
  Error("IlcTARGETetfSDD","Copy constructor not allowed\n");
}

//______________________________________________________________________
IlcTARGETetfSDD& IlcTARGETetfSDD::operator=(const IlcTARGETetfSDD& /* obj */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}

IlcTARGETetfSDD::~IlcTARGETetfSDD(){
  // Destructor
  if(fZeroM) delete []fZeroM;
  if(fZeroR) delete []fZeroR;
  if(fZeroI) delete []fZeroI;
  if(fPoleM) delete []fPoleM;
  if(fPoleR) delete []fPoleR;
  if(fPoleI) delete []fPoleI;
  if(fTfR) delete []fTfR;
  if(fTfI) delete []fTfI;
  if(fWR) delete []fWR;
  if(fWI) delete []fWI;
}

void IlcTARGETetfSDD::PrintElectronics() const {
  // Printout of the parameters defining the f.e. electronics

  cout << "Time Delay " << fTimeDelay << endl;
  cout << "Sampling Time " << fSamplingTime << endl;
  cout << "Number of Time Samples " << IlcTARGETetfSDDparam::NumberOfSamples() << endl;
  cout << "fT0 " << fT0 << endl;
  cout << "fDf " << fDf << endl;
  cout << "fA0 " << fA0 << endl;

  cout << "Zero's and Pole's" << endl;
  cout << "fZeroM " << endl;
  Int_t i;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) cout << fZeroM[i] << endl;
  cout << "fZero_R " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) cout << fZeroR[i] << endl;
  cout << "fZeroI " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) cout << fZeroI[i] << endl;
  cout << "fPoleM " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) cout << fPoleM[i] << endl;
  cout << "fPoleR " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) cout << fPoleR[i] << endl;
  cout << "fPoleI " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfPoles(); i++) cout << fPoleI[i] << endl;

  cout << "Transfer function" << endl;
  cout << "Real Part" << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfSamples(); i++) cout << fTfR[i] << endl;
  cout << "Imaginary Part " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfSamples(); i++) cout << fTfI[i] << endl;

  cout << "Fourier Weights" << endl;
  cout << "Real Part" << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfSamples(); i++) cout << fWR[i] << endl;
  cout << "Imaginary Part " << endl;
  for(i=0; i<IlcTARGETetfSDDparam::NumberOfSamples(); i++) cout << fWI[i] << endl;
}








