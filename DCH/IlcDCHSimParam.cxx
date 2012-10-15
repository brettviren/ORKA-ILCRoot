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

/* $Id: IlcDCHSimParam.cxx,v 1.2 2009/12/11 10:40:28 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Class containing constant simulation parameters                           //
//                                                                           //
// Request an instance with IlcDCHSimParam::Instance()                 //
// Then request the needed values                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TMath.h>

#include "IlcRun.h"

#include "IlcDCHSimParam.h"

ClassImp(IlcDCHSimParam)

IlcDCHSimParam* IlcDCHSimParam::fgInstance = 0;
Bool_t IlcDCHSimParam::fgTerminated = kFALSE;

//_ singleton implementation __________________________________________________
IlcDCHSimParam* IlcDCHSimParam::Instance()
{
  //
  // Singleton implementation
  // Returns an instance of this class, it is created if neccessary
  // 
  
  if (fgTerminated != kFALSE)
    return 0;

  if (fgInstance == 0)
    fgInstance = new IlcDCHSimParam();
  
  return fgInstance;
}

void IlcDCHSimParam::Terminate()
{
  //
  // Singleton implementation
  // Deletes the instance of this class and sets the terminated flag, instances cannot be requested anymore
  // This function can be called several times.
  //
  
  fgTerminated = kTRUE;
  
  if (fgInstance != 0)
  {
    delete fgInstance;
    fgInstance = 0;
  }
}

//_____________________________________________________________________________
IlcDCHSimParam::IlcDCHSimParam()
{
  //
  // default constructor
  //
  
  fGasGain            = 0.0;
  fNoise              = 0.0;
  fChipGain           = 0.0;
  
  fADCoutRange        = 0.0;
  fADCinRange         = 0.0;
  fADCthreshold       = 0;
  fADCbaseline        = 0;        
  
  fDiffusionOn        = kFALSE;
  
  fElAttachOn         = kFALSE;
  fElAttachProp       = 0.0;
  
  fTRFOn              = kFALSE;
  fTRFsmp             = 0;
  fTRFbin             = 0;
  fTRFlo              = 0.0;
  fTRFhi              = 0.0;
  fTRFwid             = 0.0;
  
  fCTOn               = kFALSE;
  fCTsmp              = 0;
  
  fAnodeWireOffset    = 0.0;
  fPadCoupling        = 0.0;
  fTimeCoupling       = 0.0;
  fTimeStructOn       = kFALSE;
  
  fPRFOn              = kFALSE;
  
  Init();
}

//_____________________________________________________________________________
void IlcDCHSimParam::Init()
{
  // 
  // default initializiation
  //
  
  // The default parameter for the digitization
  fGasGain        = 4000.;
  fChipGain       = 12.4;
  fNoise          = 1000.;
  fADCoutRange    = 1023.;          // 10-bit ADC
  fADCinRange     = 2000.;          // 2V input range
  fADCthreshold   = 1;
  fADCbaseline    = 0;

  // Diffusion on
  fDiffusionOn    = kTRUE;
  
  // Propability for electron attachment
  fElAttachOn     = kFALSE;
  fElAttachProp   = 0.0;

  // The time response function
  fTRFOn          = kTRUE;

  // The cross talk
  fCTOn           = kTRUE;

  // The pad coupling factor
  //fPadCoupling    = 0.3;
  // Use 0.46 instead which reproduces better the test beam
  // data, even tough it is not understood why.
  fPadCoupling    = 0.46;

  // The time coupling factor (same number as for the TPC)
  fTimeCoupling   = 0.4;

  // Distance of first Anode wire from first pad edge
  fAnodeWireOffset = 0.25;

  // Use drift time maps
  fTimeStructOn = kTRUE;
  
  // The pad response function
  fPRFOn          = kTRUE;

  ReInit();
}

//_____________________________________________________________________________
IlcDCHSimParam::~IlcDCHSimParam() 
{
  //
  // destructor
  //
  
  if (fTRFsmp) {
    delete [] fTRFsmp;
    fTRFsmp = 0;
  }

  if (fCTsmp) {
    delete [] fCTsmp;
    fCTsmp  = 0;
  }
}

//_____________________________________________________________________________
IlcDCHSimParam::IlcDCHSimParam(const IlcDCHSimParam &p):TObject(p)
{
  //
  // copy constructor
  //

  ((IlcDCHSimParam &) p).Copy(*this);
}


//_____________________________________________________________________________
IlcDCHSimParam &IlcDCHSimParam::operator=(const IlcDCHSimParam &p)
{
  //
  // Assignment operator
  //

  if (this != &p) ((IlcDCHSimParam &) p).Copy(*this);
  return *this;
}

//_____________________________________________________________________________
void IlcDCHSimParam::Copy(TObject &p) const
{
  //
  // Copy function
  //
  
  IlcDCHSimParam* target = dynamic_cast<IlcDCHSimParam*> (&p);
  if (!target)
    return;

  target->fGasGain            = fGasGain;
  //target->fField              = fField;
  target->fNoise              = fNoise;
  target->fChipGain           = fChipGain;
  
  target->fADCoutRange        = fADCoutRange;
  target->fADCinRange         = fADCinRange;
  target->fADCthreshold       = fADCthreshold;
  target->fADCbaseline        = fADCbaseline; 
  
  target->fDiffusionOn        = fDiffusionOn; 
  
  target->fElAttachOn         = fElAttachOn;
  target->fElAttachProp       = fElAttachProp;
  
  target->fTRFOn              = fTRFOn;
  if (target->fTRFsmp) 
    delete[] target->fTRFsmp;
  target->fTRFsmp = new Float_t[fTRFbin];
  for (Int_t iBin = 0; iBin < fTRFbin; iBin++) {
    target->fTRFsmp[iBin] = fTRFsmp[iBin];
  }
  target->fTRFbin             = fTRFbin;
  target->fTRFlo              = fTRFlo;
  target->fTRFhi              = fTRFhi;
  target->fTRFwid             = fTRFwid;
  
  target->fCTOn               = fCTOn;
  if (target->fCTsmp) 
    delete[] target->fCTsmp;
  target->fCTsmp  = new Float_t[fTRFbin];
  for (Int_t iBin = 0; iBin < fTRFbin; iBin++) {
    target->fCTsmp[iBin]  = fCTsmp[iBin];
  }
  
  target->fAnodeWireOffset    = fAnodeWireOffset;
  target->fPadCoupling        = fPadCoupling;
  target->fTimeCoupling       = fTimeCoupling;
  
  target->fPRFOn              = fPRFOn;
}

//_____________________________________________________________________________
void IlcDCHSimParam::ReInit()
{
  //
  // Reinitializes the parameter class after a change
  //

  // The range and the binwidth for the sampled TRF 
  fTRFbin = 200;
  // Start 0.2 mus before the signal
  fTRFlo  = -0.4;
  // End the maximum drift time after the signal 
  fTRFhi  = 3.58;
  // 
  fTRFwid = (fTRFhi - fTRFlo) / ((Float_t) fTRFbin);

  // Create the sampled TRF
  SampleTRF();
}


//_____________________________________________________________________________
void IlcDCHSimParam::SampleTRF()
{
  //
  // Samples the new time response function.
  // From Antons measurements with Fe55 source, adjusted by C. Lippmann.
  // time bins are -0.4, -0.38, -0.36, ...., 3.54, 3.56, 3.58 microseconds
  //

  const Int_t kNpasa     = 200;  // kNpasa should be equal to fTRFbin!

  Float_t signal[kNpasa]={0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
			  0.0002, 0.0007, 0.0026, 0.0089, 0.0253, 0.0612, 0.1319,
			  0.2416, 0.3913, 0.5609, 0.7295, 0.8662, 0.9581, 1.0000,
			  0.9990, 0.9611, 0.8995, 0.8269, 0.7495, 0.6714, 0.5987,
			  0.5334, 0.4756, 0.4249, 0.3811, 0.3433, 0.3110, 0.2837,
			  0.2607, 0.2409, 0.2243, 0.2099, 0.1974, 0.1868, 0.1776,
			  0.1695, 0.1627, 0.1566, 0.1509, 0.1457, 0.1407, 0.1362,
			  0.1317, 0.1274, 0.1233, 0.1196, 0.1162, 0.1131, 0.1102,
			  0.1075, 0.1051, 0.1026, 0.1004, 0.0979, 0.0956, 0.0934,
			  0.0912, 0.0892, 0.0875, 0.0858, 0.0843, 0.0829, 0.0815,
			  0.0799, 0.0786, 0.0772, 0.0757, 0.0741, 0.0729, 0.0718,
			  0.0706, 0.0692, 0.0680, 0.0669, 0.0655, 0.0643, 0.0630,
			  0.0618, 0.0607, 0.0596, 0.0587, 0.0576, 0.0568, 0.0558,
			  0.0550, 0.0541, 0.0531, 0.0522, 0.0513, 0.0505, 0.0497,
			  0.0490, 0.0484, 0.0474, 0.0465, 0.0457, 0.0449, 0.0441,
			  0.0433, 0.0425, 0.0417, 0.0410, 0.0402, 0.0395, 0.0388,
			  0.0381, 0.0374, 0.0368, 0.0361, 0.0354, 0.0348, 0.0342,
			  0.0336, 0.0330, 0.0324, 0.0318, 0.0312, 0.0306, 0.0301,
			  0.0296, 0.0290, 0.0285, 0.0280, 0.0275, 0.0270, 0.0265,
			  0.0260, 0.0256, 0.0251, 0.0246, 0.0242, 0.0238, 0.0233,
			  0.0229, 0.0225, 0.0221, 0.0217, 0.0213, 0.0209, 0.0206,
			  0.0202, 0.0198, 0.0195, 0.0191, 0.0188, 0.0184, 0.0181,
			  0.0178, 0.0175, 0.0171, 0.0168, 0.0165, 0.0162, 0.0159,
			  0.0157, 0.0154, 0.0151, 0.0148, 0.0146, 0.0143, 0.0140,
			  0.0138, 0.0135, 0.0133, 0.0131, 0.0128, 0.0126, 0.0124,
			  0.0121, 0.0119, 0.0120, 0.0115, 0.0113, 0.0111, 0.0109,
			  0.0107, 0.0105, 0.0103, 0.0101, 0.0100, 0.0098, 0.0096,
			  0.0094, 0.0092, 0.0091, 0.0089, 0.0088, 0.0086, 0.0084,
			  0.0083, 0.0081, 0.0080, 0.0078};

  Float_t xtalk[kNpasa];

  for (Int_t ipasa = 3; ipasa < kNpasa; ipasa++) {
    //signal[ipasa] /= 0.44; // normilcse area to 1
    // With undershoot, positive peak corresponds to ~3% of the main signal:
    xtalk[ipasa] = 0.2*(signal[ipasa-2]-signal[ipasa-3]);
  }

  xtalk[0] =  0.0;      signal[0] = 0.0;
  xtalk[1] =  0.0;      signal[1] = 0.0;
  xtalk[2] =  0.0;      signal[2] = 0.0;

  if (fTRFsmp) delete [] fTRFsmp;
  fTRFsmp = new Float_t[fTRFbin];
  if (fCTsmp)  delete [] fCTsmp;
  fCTsmp  = new Float_t[fTRFbin];

  for (Int_t iBin = 0; iBin < fTRFbin; iBin++) {
    fTRFsmp[iBin] = signal[iBin];
    fCTsmp[iBin]  = xtalk[iBin];
  }

}

//_____________________________________________________________________________
Double_t IlcDCHSimParam::TimeResponse(Double_t time) const
{
  //
  // Applies the preamp shaper time response
  // (We assume a signal rise time of 0.2us = fTRFlo/2.
  //

  Int_t iBin = ((Int_t) ((time - fTRFlo/2.) / fTRFwid)); 
  if ((iBin >= 0) && (iBin < fTRFbin)) {
    return fTRFsmp[iBin];
  }
  else {
    return 0.0;
  }    

}

//_____________________________________________________________________________
Double_t IlcDCHSimParam::CrossTalk(Double_t time) const
{
  //
  // Applies the pad-pad capacitive cross talk
  //

  Int_t iBin = ((Int_t) ((time - fTRFlo) / fTRFwid)); 
  if ((iBin >= 0) && (iBin < fTRFbin)) {
    return fCTsmp[iBin];
  }
  else {
    return 0.0;
  }    

}

