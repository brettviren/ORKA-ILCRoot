/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                         *
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


/* $Id: IlcTARGETTrigger.cxx,v 1.2 2008/03/17 18:20:42 vitomeg Exp $ */


#include <TClonesArray.h>
#include <TTree.h>

#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcTriggerInput.h"

#include "IlcTARGETTrigger.h"
#include "IlcTARGETdigitSPD.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETLoader.h"

//______________________________________________________________________
ClassImp(IlcTARGETTrigger)
////////////////////////////////////////////////////////////////////////
//
// Version 1
// Modified by D. Elia, C. Jorgensen
// March 2006
//
// Version 0
// Written by J. Conrad, E. Lopez Torres 
// October 2005
//
// IlcTARGETTrigger: implementation of the SPD Fast-OR based triggers.
//
////////////////////////////////////////////////////////////////////////

//______________________________________________________________________
IlcTARGETTrigger::IlcTARGETTrigger()
  : IlcTriggerDetector()
{
   SetName("TARGET");
   CreateInputs();

   // set parameters to define trigger condition thresholds
   fGlobalFOThreshold = 1;
   fHighMultFOThreshold = 150; 
}

//______________________________________________________________________
void IlcTARGETTrigger::CreateInputs()
{
   // inputs

   // Do not create inputs again!!
   if( fInputs.GetEntriesFast() > 0 ) return;
   
   fInputs.AddLast( new IlcTriggerInput( "TARGET_SPD_GFO_L0",     "Global, Fast OR all detectors", 0x01 ) );
   fInputs.AddLast( new IlcTriggerInput( "TARGET_SPD_HMULT_L0",   "High Multiplicity",             0x02 ) );

}

//______________________________________________________________________
void IlcTARGETTrigger::Trigger()
{

  //  ********** Get run loader for the current event **********
  IlcRunLoader* runLoader = IlcRunLoader::Instance();

  IlcTARGETLoader* loader = (IlcTARGETLoader* )runLoader->GetLoader( "TARGETLoader" );
  IlcTARGETgeom* geom = loader->GetTARGETgeom();
  loader->LoadDigits("READ");
  TTree *treeD = loader->TreeD();
  if (!treeD) return;

  TObjArray *digDet = 0;
  digDet = new TObjArray(3);


  // Cut on Signal In the Pixel Detector
  TBranch* br = treeD->GetBranch( "TARGETDigitsSPD" );
  br->SetAddress( &((*digDet)[0]) );
  ((TClonesArray*)(digDet->At(0)))->Clear();

  MultiplicityTriggers(digDet, treeD, geom);
  //  GeometryTriggers(digDet, treeD, geom);

  // Debug : FIX change to IlcLog
//  cout << "=================================================" << endl;
//  cout << "  Pixel Trigger Mask ( " << hex << "0x" << GetMask() << " )" << endl << endl;
//  cout << " Global Fast OR        " << "0x" << GetInput( "TARGET_SPD_GFO_L0"      )->GetValue() << endl;
//  cout << " High Multiplicity     " << "0x" << GetInput( "TARGET_SPD_HMULT_L0"      )->GetValue() << endl;
//  cout << "=================================================" << endl << endl;

}

//______________________________________________________________________
void IlcTARGETTrigger::MultiplicityTriggers(TObjArray* digDet, TTree* treeD, IlcTARGETgeom* geom)
{
  // simple FO triggers that only cares about the multiplicity

  Int_t startSPD = geom->GetStartSPD();
  Int_t lastSPD  = geom->GetLastSPD();

  Int_t totalNumberOfFO = 0;
  Int_t ndigitsInChip[5];

  // loop over modules (ladders)
  for (Int_t moduleIndex=startSPD; moduleIndex<lastSPD+1; moduleIndex++) {
    treeD->GetEvent(moduleIndex);
    TClonesArray* digits = (TClonesArray*) (digDet->At(0)); // SPD only.
    
    // get number of digits in this module
    Int_t ndigitsInModule = digits->GetEntriesFast();

    // get number of digits in each chip of the module
    for( Int_t iChip=0; iChip<5; iChip++ ) {
	ndigitsInChip[iChip]=0;
    }	
    for( Int_t iDig=0; iDig<ndigitsInModule; iDig++ ) {
	IlcTARGETdigitSPD* dp = (IlcTARGETdigitSPD*) digits->At(iDig);
	Int_t column = dp->GetCoord1();
	Int_t isChip = Int_t(column/32.);
	ndigitsInChip[isChip]++;
    }
    // get number of FOs in the module
    for( Int_t ifChip=0; ifChip<5; ifChip++ ) {
	if( ndigitsInChip[ifChip] >= 1 ) {
    		totalNumberOfFO++;
    	}	
    }
  // end of loop over modules
  }

  // produce input trigger condition
  if (totalNumberOfFO>=fGlobalFOThreshold) 
    SetInput( "TARGET_SPD_GFO_L0" );

  if (totalNumberOfFO>=fHighMultFOThreshold) 
    SetInput( "TARGET_SPD_HMULT_L0" );

  return;

}

//______________________________________________________________________
// void IlcTARGETTrigger::GeometryTriggers(TObjArray* digDet, TTree* treeD, IlcTARGETgeom* geom)
void IlcTARGETTrigger::GeometryTriggers()
{

//   //   const Int_t nlay = 2;      // not used
//   //   const Int_t nlad = 240;    // not used
//   //   const Int_t nstave = 40;   // not used
//   //   const Int_t ndet = 4;      // not used
//   //   const Int_t nchip = 5;     // not used
//   //   const Int_t ntotal = 1200;
  
//   Int_t ndigA[5];
//   Int_t iFOperlayer[2];
//   Int_t iFOperladder[240];
//   Int_t iFOperstave[40][2];
//   //   Int_t iFOperchip[ntotal];   // not used
//   Int_t iFOperChipinStave[20][40][2];
  
//   for (Int_t m=startSPD;m<lastSPD+1;m++) {
//     iFOperladder[m] = 0;
//   }

//   for (Int_t k = 0;k<2;k++){
//     iFOperlayer[k] = 0;
//     for (Int_t o=0; o<40; o++) {
//       iFOperstave[o][k] = 0;
//       for (Int_t ich=0; ich<20; ich++) {
// 	iFOperChipinStave[ich][o][k] = 0;
//       }
//     }
//   }

//   // nFO = 0.0;
//   Int_t mInStaveCounter = 0;
//   Int_t checkStave = 0;
  
//   // loop over modules
//   for (Int_t moduleIndex=startSPD; moduleIndex<lastSPD+1; moduleIndex++) {
//     treeD->GetEvent(moduleIndex);
//     TClonesArray* digits = (TClonesArray*) (digDet->At(0)); // SPD only.

//     Int_t lay, stav, det;
//     geom->GetModuleId(moduleIndex,lay,stav,det);

//     ndig = digits->GetEntriesFast();

//     for( Int_t l=0; l<5; l++ ) {
//       ndigA[l] = 0 ;
//     }
//     for( Int_t dig=0; dig<ndig; dig++) {
//       IlcTARGETdigitSPD* dp = (IlcTARGETdigitSPD*) digits->At(dig);
//       Int_t column = dp->GetCoord1();
//       //   Int_t row = dp->GetCoord2();
//       Int_t chip = Int_t(column/32.);
//       ndigA[chip]++;
//     }
    
//     if (checkStave != stav) {
//       mInStaveCounter = 0;
//     } else {
//       mInStaveCounter += 1;
//     }

//     // m 0,.., 239
//     // stav 1,..,40
//     // mInStave 0,..,3
//     // chipInStave 0,..,19
    
//     //cout << "m " << m << " stav "  << stav  << " mInStave " << mInStaveCounter << " " <<lay << endl;
    
//     for (Int_t ichip=0; ichip<5; ichip++) {
//       //Int_t seq = (m*5+ichip);
//       Int_t chipInStave = (mInStaveCounter *5) + ichip;
      
//       if (ndigA[ichip] >= 1) {
// 	iFOperladder[moduleIndex]++;
// 	iFOperlayer[lay-1]++;
// 	iFOperstave[stav-1][lay-1]++;
// 	//iFOperHstave[hstav-1][lay-1]++;
// 	iFOperChipinStave[chipInStave][stav-1][lay-1]++;
//         //    nFO++;
//       }
//     }
//     // SIMPLE FO ---> ANY HIT TRIGGERS
//     ndigfo += ndig;
//     checkStave = stav;
//   } // endl loop over SPD's



//    if ( ndigfo >= singhitthreshold ) {
//       // Set input GLOBAL FO
//       SetInput( "TARGET_SPD_GFO_L0" );
//       // or SetInput( "0x01" );
//       // or ((IlcTriggerInput*)fInputs.At(0))->Set();
//       // bit1 |= (1 << 1);
//    }

   // return bit1;
}
