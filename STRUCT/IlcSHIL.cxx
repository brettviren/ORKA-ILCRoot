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

/* $Id: IlcSHIL.cxx 47909 2011-02-28 15:05:53Z fca $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Muon Shield Class                                                        //
//  This class contains a description of the muon shield                     //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcSHILClass.gif">
*/
//End_Html
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TGeoGlobalMagField.h>

#include "IlcSHIL.h"
#include "IlcRun.h"
#include "IlcMagF.h"
#include "IlcConst.h"
#include "IlcLog.h"

ClassImp(IlcSHIL)
 
//_____________________________________________________________________________
IlcSHIL::IlcSHIL()
{
  //
  // Default constructor for muon shield
  //
}
 
//_____________________________________________________________________________
IlcSHIL::IlcSHIL(const char *name, const char *title)
  : IlcModule(name,title)
{
  //
  // Standard constructor for muon shield
  //
  //PH  SetMarkerColor(7);
  //PH  SetMarkerStyle(2);
  //PH  SetMarkerSize(0.4);
}
 
//_____________________________________________________________________________
void IlcSHIL::CreateGeometry()
{
  //
  // Build muon shield geometry
  //
}

//_____________________________________________________________________________
void IlcSHIL::CreateMaterials()
{
  //
  // Defines materials for the muon shield
  //
  Int_t   isxfld1 = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ();
  Int_t   isxfld2 = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->PrecInteg();

  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max();
// Steel  
  Float_t asteel[4] = { 55.847,51.9961,58.6934,28.0855 };
  Float_t zsteel[4] = { 26.,24.,28.,14. };
  Float_t wsteel[4] = { .715,.18,.1,.005 };
// PbW
  Float_t apbw[2]   = { 207.2,183.85 };
  Float_t zpbw[2]   = { 82.,74. };
  Float_t wpbw[2]   = { .5,.5 };
// Concrete
  Float_t aconc[10] = { 1.,12.01,15.994,22.99,24.305,26.98,
			28.086,39.1,40.08,55.85 };
  Float_t zconc[10] = { 1.,6.,8.,11.,12.,13.,14.,19.,20.,26. };
  Float_t wconc[10] = { .01,.001,.529107,.016,.002,.033872,
			.337021,.013,.044,.014 };
// Ni-Cu-W alloy
  Float_t aniwcu[3] ={58.6934, 183.84, 63.546};
  Float_t zniwcu[3] ={28.,      74.,   29.};
  Float_t wniwcu[3] ={ 0.015,    0.95,  0.035};
//
// Insulation powder
//                    Si         O       Ti     Al
  Float_t ains[4] ={28.0855, 15.9994, 47.867,  26.982};
  Float_t zins[4] ={14.,      8.    , 22.   ,  13.   };
  Float_t wins[4] ={ 0.3019,  0.4887,  0.1914,  0.018};
//
// Air
//
  Float_t aAir[4]={12.0107,14.0067,15.9994,39.948};
  Float_t zAir[4]={6.,7.,8.,18.};
  Float_t wAir[4]={0.000124,0.755267,0.231781,0.012827};
  Float_t dAir = 1.20479E-3;
  Float_t dAir1 = 1.20479E-10;

  
  Float_t epsil, stmin, tmaxfd, deemax, stemax;
  
  //     STEEL 
  
  
  //     LEAD/TUNGSTEN MIXTURE 
  
  
  // --- Define the various materials for GEANT --- 
  //     Aluminum 
  IlcMaterial(9,  "ALU1      ", 26.98, 13., 2.7, 8.9, 37.2);
  IlcMaterial(29, "ALU2      ", 26.98, 13., 2.7, 8.9, 37.2);
  IlcMaterial(49, "ALU3      ", 26.98, 13., 2.7, 8.9, 37.2);
  
  //     Iron 
  IlcMaterial(10, "IRON1     ", 55.85, 26., 7.87, 1.76, 17.1);
  IlcMaterial(30, "IRON2     ", 55.85, 26., 7.87, 1.76, 17.1);
  IlcMaterial(50, "IRON3     ", 55.85, 26., 7.87, 1.76, 17.1);

  //
  //     Copper
  IlcMaterial(11, "COPPER1   ", 63.55, 29., 8.96, 1.43, 15.1);
  IlcMaterial(31, "COPPER2   ", 63.55, 29., 8.96, 1.43, 15.1);
  IlcMaterial(51, "COPPER3   ", 63.55, 29., 8.96, 1.43, 15.1);
  
  //     Tungsten 
  IlcMaterial(12, "TUNGSTEN1 ", 183.85, 74., 19.3, .35, 10.3);
  IlcMaterial(32, "TUNGSTEN2 ", 183.85, 74., 19.3, .35, 10.3);
  IlcMaterial(52, "TUNGSTEN3 ", 183.85, 74., 19.3, .35, 10.3);
  
  //     Lead 
  IlcMaterial(13, "LEAD1     ", 207.19, 82., 11.35, .56, 18.5);
  IlcMaterial(33, "LEAD2     ", 207.19, 82., 11.35, .56, 18.5);
  IlcMaterial(53, "LEAD3     ", 207.19, 82., 11.35, .56, 18.5);
  
  //     Air 
  IlcMixture(15, "AIR1      ", aAir, zAir, dAir, 4, wAir);
  IlcMixture(35, "AIR2      ", aAir, zAir, dAir, 4, wAir);
  IlcMixture(55, "AIR3      ", aAir, zAir, dAir, 4, wAir);
  IlcMixture(75, "AIR_MUON  ", aAir, zAir, dAir, 4, wAir);

  //     Vacuum 
  IlcMixture(16, "VACUUM1 ", aAir, zAir, dAir1, 4, wAir);
  IlcMixture(36, "VACUUM2 ", aAir, zAir, dAir1, 4, wAir);
  IlcMixture(56, "VACUUM3 ", aAir, zAir, dAir1, 4, wAir);
  
  //     Stainless Steel 
  IlcMixture(19, "STAINLESS STEEL1", asteel, zsteel, 7.88, 4, wsteel);
  IlcMixture(39, "STAINLESS STEEL2", asteel, zsteel, 7.88, 4, wsteel);
  IlcMixture(59, "STAINLESS STEEL3", asteel, zsteel, 7.88, 4, wsteel);
  
  //     Lead/Tungsten 
  IlcMixture(20, "LEAD/TUNGSTEN1", apbw, zpbw, 15.325, 2, wpbw);
  IlcMixture(40, "LEAD/TUNGSTEN2", apbw, zpbw, 15.325, 2, wpbw);
  IlcMixture(60, "LEAD/TUNGSTEN3", apbw, zpbw, 15.325, 2, wpbw);

  //     Ni-W-Cu 
  IlcMixture(21, "Ni-W-Cu1", aniwcu, zniwcu, 18.78, 3, wniwcu);
  IlcMixture(41, "Ni-W-Cu2", aniwcu, zniwcu, 18.78, 3, wniwcu);
  IlcMixture(61, "Ni-W-Cu3", aniwcu, zniwcu, 18.78, 3, wniwcu);

  //     Concrete 
  IlcMixture(17, "CONCRETE1", aconc, zconc, 2.35, 10, wconc);
  IlcMixture(37, "CONCRETE2", aconc, zconc, 2.35, 10, wconc);
  IlcMixture(57, "CONCRETE3", aconc, zconc, 2.35, 10, wconc);

  //     Insulation powder 
  IlcMixture(14, "INSULATION1", ains, zins, 0.41, 4, wins);
  IlcMixture(34, "INSULATION2", ains, zins, 0.41, 4, wins);
  IlcMixture(54, "INSULATION3", ains, zins, 0.41, 4, wins);
  
  // **************** 
  //     Defines tracking media parameters. 
  //     Les valeurs sont commentees pour laisser le defaut 
  //     a GEANT (version 3-21, page CONS200), f.m. 
  epsil  = .001;  // Tracking precision, 
  stemax = -1.;   // Maximum displacement for multiple scat 
  tmaxfd = -20.;  // Maximum angle due to field deflection 
  deemax = -.3;   // Maximum fractional energy loss, DLS 
  stmin  = -.8;
  // *************** 
  
  //    Aluminum 
  IlcMedium(9,  "ALU_C0          ", 9, 0,  isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(29, "ALU_C1          ", 29, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(49, "ALU_C2          ", 49, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Iron 
  IlcMedium(10, "FE_C0           ", 10, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(30, "FE_C1           ", 30, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(50, "FE_C2           ", 50, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);

  //    Copper 
  IlcMedium(11, "Cu_C0           ", 11, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(31, "Cu_C1           ", 31, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(51, "Cu_C2           ", 51, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Tungsten 
  IlcMedium(12, "W_C0            ", 12, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(32, "W_C1            ", 32, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(52, "W_C2            ", 52, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Lead 
  IlcMedium(13, "PB_C0           ", 13, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(33, "PB_C1           ", 33, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(53, "PB_C2           ", 53, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);

  //    Insulation Powder 
  IlcMedium(14, "INS_C0          ", 14, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(34, "INS_C1          ", 34, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(54, "INS_C2          ", 54, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Air 
  IlcMedium(15, "AIR_C0          ", 15, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(35, "AIR_C1          ", 35, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(55, "AIR_C2          ", 55, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(75, "AIR_MUON        ", 75, 0, isxfld2, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Vacuum 
  IlcMedium(16, "VA_C0           ", 16, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(36, "VA_C1           ", 36, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(56, "VA_C2           ", 56, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Steel 
  IlcMedium(19, "ST_C0           ", 19, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(39, "ST_C1           ", 39, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(59, "ST_C3           ", 59, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  
  //    Lead/Tungsten 
  IlcMedium(20, "PB/W0           ", 20, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(40, "PB/W1           ", 40, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(60, "PB/W3           ", 60, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);

  //    Ni/Tungsten 
  IlcMedium(21, "Ni/W0           ", 21, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(41, "Ni/W1           ", 41, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(61, "Ni/W3           ", 61, 0, isxfld1, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);

//    Concrete 
  IlcMedium(17, "CC_C0           ", 17, 0, 0,      sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(37, "CC_C1           ", 37, 0, 0,      sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(57, "CC_C2           ", 57, 0, 0,      sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);

}

//_____________________________________________________________________________
void IlcSHIL::Init()
{
  //
  // Initialise the muon shield after it has been built
  //
  Int_t i;
  //
  if(IlcLog::GetGlobalDebugLevel()>0) {
    printf("\n%s: ",ClassName());
    for(i=0;i<35;i++) printf("*");
    printf(" SHIL_INIT ");
    for(i=0;i<35;i++) printf("*");
    printf("\n%s: ",ClassName());
    //
    // Here the SHIL initialisation code (if any!)
    for(i=0;i<80;i++) printf("*");
    printf("\n");
  }
}

 
