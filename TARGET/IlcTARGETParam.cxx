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

/* $Id: IlcTARGETParam.cxx,v 1.7 2008/11/10 21:08:05 cgatto Exp $ */

///////////////////////////////////////////////////////////////////////
// TARGET Parameter manipulation routines.                              //
// Created April 15 1999.                                            //
// version: 0.0.0                                                    //
// By: Bjorn S. Nilsen                                               //
// version: 0.0.1                                                    //
// Updated May 27 1999.                                              //
// Added Cylindrical random and global based changes.               //
// Added  function PrintComparison.                                  //
// Modified and added functions Feb. 7 2006                          //
///////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//     The local coordinate system by, default, is show in the following
// figures. Also shown are the ladder numbering scheme.
//Begin_Html
#include <Riostream.h>
#include <TString.h>
#include <ctype.h>
#include <TArrayI.h>
#include "IlcLog.h"
#include "IlcTARGETParam.h"
#include "TMath.h"

ClassImp(IlcTARGETParam)

//______________________________________________________________________
IlcTARGETParam::IlcTARGETParam(Int_t detVersion):
TObject(),
fNlad(),         //[] Array of the number of ladders/layer(layer)
fNdet(),          //[] Array of the number of detector/ladder(layer)
fSigmaRPhi(),
fSigmaZ(),
fSigmaDe(),
fThrDe(),
fSingleHitStripEfficiency(0.),
fSingleHitPixelEfficiency(0.),
fbiasVoltage(0.),
fRangeCluY(0),
fRangeCluZ(0),
fFda(0.),
fCouplCol(0.),
fCouplRow(0.),
fStereoP(0),
fStereoN(0),
fSensor(0),
fLayerThick(0),
fTargetThick(0),
fNlayers(0),
fTargetLength(0),
fTargetLowerZ(0),
fNLadders(0)
{
  
  fOldMagField=5.0; //Previous Magnetic Field
  fNewMagField=3.5; //New Magnetic Field
  // fScale=fOldMagField/fNewMagField;  //Scale's factor = Old Magnetic Field/New Old Magnetic Field
  fScale=20./9.;  //Rmin inner layer ~= 3 cm
  // fScale=100./27.;  //Rmin inner layer ~= 5 cm
  
  fSiThick=0.005000;//Half Silicon Thickness
  fCThick=0.003800;//Half CarbonFiber Thickness
  
  //fScaleSiD=1.0; //Scale's factorfor SiD version - IlcTARGETvSiD
  
  
  fNendcaps=0;       // The number of endcaps.
  fNendcapsStrip=0;  // The number of endcaps strip.
  fNmodulesSSD=0;    // The total number of SSD modules
  fNlayersStrip=0;   // The number of strip layers.
  Init(detVersion);
  
  return;
}


//______________________________________________________________________
void IlcTARGETParam::Init(Int_t detVersion){

  if(detVersion == 1){  //square fiber version
    fLayerThick = 0.5;
    fTargetThick = 6.;
    fNlayers = (Int_t)(fTargetThick/fLayerThick)-1;
    fTargetLength = 100.;
    fTargetLowerZ = -12.;
    fNLadders = 4;
    fNmodulesSPD=fNlayers*fNLadders;
    fSigmaRPhi.Set(fNlayers);
    fSigmaZ.Set(fNlayers);
    fSigmaDe.Set(fNlayers);
    fThrDe.Set(fNlayers);
    Int_t ladders[fNlayers];
    Int_t dets[fNlayers];
    for (Int_t i=0;i<fNlayers;i++) ladders[i]=fNLadders;
    for (Int_t i=0;i<fNlayers;i++) dets[i]=1;
    fNlad.Set(fNlayers,ladders);//[] Array of the number of ladders/layer(layer)

    fNdet.Set(fNlayers,dets);//[] Array of the number of detector/ladder(layer)

    for (Int_t i=0;i<fNlayers;i++) SetSigmaRPhi(i,0.5/TMath::Sqrt(12.));

    for (Int_t i=0;i<fNlayers;i++) SetSigmaZ(i,5./TMath::Sqrt(12.));

    for (Int_t i=0;i<fNlayers;i++) SetSigmaDe(i,0.18e-6);
    for (Int_t i=0;i<fNlayers;i++) SetThrDe(i,0.);

    fRangeCluY=20;
    fRangeCluZ=20;
    fbiasVoltage=100.;
    fFda=0.85;
    fCouplCol=0.0;
    fCouplRow=0.0;
    fSingleHitPixelEfficiency=0.995;
  }
  else if(detVersion == 2){  //poligonal version
    fLayerThick = 0.5;
    fTargetThick = 6.;
    fNlayers = (Int_t)(fTargetThick/fLayerThick);
    fTargetLength = 100.;
    fTargetLowerZ = -12.;
    fNLadders = 24;
    fNmodulesSPD=(fNlayers-1)*fNLadders+1;
    fSigmaRPhi.Set(fNlayers);
    fSigmaZ.Set(fNlayers);
    fSigmaDe.Set(fNlayers);
    fThrDe.Set(fNlayers);
    Int_t ladders[fNlayers];
    Int_t dets[fNlayers];
    ladders[0]=1;
    for (Int_t i=1;i<fNlayers;i++) ladders[i]=fNLadders;
    for (Int_t i=0;i<fNlayers;i++) dets[i]=1;
    fNlad.Set(fNlayers,ladders);//[] Array of the number of ladders/layer(layer)
    
    fNdet.Set(fNlayers,dets);//[] Array of the number of detector/ladder(layer)
    
    for (Int_t i=0;i<fNlayers;i++) SetSigmaRPhi(i,0.15/TMath::Sqrt(12.));
    
    for (Int_t i=0;i<fNlayers;i++) SetSigmaZ(i,5./TMath::Sqrt(12.));
    
    for (Int_t i=0;i<fNlayers;i++) SetSigmaDe(i,0.18e-6);
    for (Int_t i=0;i<fNlayers;i++) SetThrDe(i,0.);
    
    fRangeCluY=20;
    fRangeCluZ=20;
    fbiasVoltage=100.;
    fFda=0.85;
    fCouplCol=0.0;
    fCouplRow=0.0;
    fSingleHitPixelEfficiency=0.995;
    
  }
  else IlcFatal(Form("TARGET version %d doesn't exist", detVersion));
  
  return;
}

//______________________________________________________________________
IlcTARGETParam::~IlcTARGETParam(){
    //     The destructor for the IlcTARGETParam class. If the arrays fNlad,
    // fNdet, or fGm have had memory allocated to them, there pointer values
    // are non zero, then this memory space is freed and they are set
    // to zero. In addition, fNlayers is set to zero. The destruction of
    // TObjArray fShape is, by default, handled by the TObjArray destructor.
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    none.
    delete fStereoP;
    delete fStereoN;
    delete fSensor;
    return;
}
