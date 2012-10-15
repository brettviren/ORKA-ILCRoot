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

/* $Id: IlcABSO.cxx 49869 2011-05-18 04:49:51Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Muon ABSOrber                                                            //
//  This class contains the description of the muon absorber geometry        //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcABSOClass.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>The responsible person for this module is
<a href="mailto:andreas.morsch@cern.ch">Andreas Morsch</a>.
</font>
<pre>
*/
//End_Html
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TGeoGlobalMagField.h>
#include <TArrayI.h>

#include "IlcABSO.h"
#include "IlcRun.h"
#include "IlcMagF.h"
#include "IlcConst.h"
 
ClassImp(IlcABSO)
 
//_____________________________________________________________________________
IlcABSO::IlcABSO()
{
  //
  // Default constructor
  //
    for (Int_t i = 0; i < 2; i++) 
    {
	fNLayers[i] = 0;
	for (Int_t j = 0; j < 15; j++) 
	{
	    fZLayers[i][j] = 0.;
	    fMLayers[i][j] = 0;
	}
    }
}
 
//_____________________________________________________________________________
IlcABSO::IlcABSO(const char *name, const char *title)
       : IlcModule(name,title)
{
  //
  // Standard constructor
  //
    for (Int_t i = 0; i < 2; i++) 
    {
	fNLayers[i] = 0;
	for (Int_t j = 0; j < 15; j++) 
	{
	    fZLayers[i][j] = 0.;
	    fMLayers[i][j] = 0;
	}
    }
}
 
//_____________________________________________________________________________
void IlcABSO::CreateGeometry()
{
  //
  // Creation of the geometry of the muon absorber
  //
}

//_____________________________________________________________________________
void IlcABSO::CreateMaterials()
{
  //
  // Define materials for muon absorber
  //
  Int_t   isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ();
  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max();
//
// Air
//
  Float_t aAir[4]={12.0107,14.0067,15.9994,39.948};
  Float_t zAir[4]={6.,7.,8.,18.};
  Float_t wAir[4]={0.000124,0.755267,0.231781,0.012827};
  Float_t dAir = 1.20479E-3;
  Float_t dAir1 = 1.20479E-10;
//
// Polyethylene
//
  Float_t apoly[2]  = { 12.01,1. };
  Float_t zpoly[2]  = { 6.,1. };
  Float_t wpoly[2]  = { .33,.67 };
//
// Concrete
//
  Float_t aconc[10] = { 1.,12.01,15.994,22.99,24.305,26.98,28.086,39.1,40.08,55.85 };
  Float_t zconc[10] = { 1.,6.,8.,11.,12.,13.,14.,19.,20.,26. };
  Float_t wconc[10] = { .01,.001,.529107,.016,.002,.033872, .337021,.013,.044,.014 };
//
// Steel
//  
  Float_t asteel[4] = { 55.847,51.9961,58.6934,28.0855 };
  Float_t zsteel[4] = { 26.,24.,28.,14. };
  Float_t wsteel[4] = { .715,.18,.1,.005 };
//
// Ni-Cu-W alloy
//
  Float_t aniwcu[3] ={58.6934, 183.84, 63.546};
  Float_t zniwcu[3] ={28., 74., 29};
  Float_t wniwcu[3] ={0.015,0.95,0.035};
//
// Poly Concrete
//                      H     Li     F       C      Al     Si      Ca      Pb     O
  Float_t aPolyCc[9] = {1. ,  6.941, 18.998, 12.01, 26.98, 28.086, 40.078, 207.2, 15.999};
  Float_t zPolyCc[9] = {1. ,  3.   ,  9.   ,  6.  , 13.  , 14.   , 20.   ,  82. ,  8.   };
  Float_t wPolyCc[9] = {4.9,  1.2  ,  1.3  ,  1.1 ,  0.15,  0.02 ,  0.06 ,   0.7,  1.1  };
  Float_t wtot=0;
  Int_t   i=0;

  for (i=0; i<9; i++) wtot+=wPolyCc[i];
  for (i=0; i<9; i++) wPolyCc[i]/=wtot;  

//
// Insulation powder
//                    Si         O       Ti     Al
  Float_t ains[4] ={28.0855, 15.9994, 47.867,  26.982};
  Float_t zins[4] ={14.,      8.    , 22.   ,  13.   };
  Float_t wins[4] ={ 0.3019,  0.4887,  0.1914,  0.018};
  
//
  Float_t epsil, stmin, tmaxfd, deemax, stemax;
  //
  //     Carbon 
  IlcMaterial( 6, "CARBON0$   ", 12.01, 6., 1.75, 24.4, 49.9);
  IlcMaterial(26, "CARBON1$   ", 12.01, 6., 1.75, 24.4, 49.9);
  IlcMaterial(46, "CARBON2$   ", 12.01, 6., 1.75, 24.4, 49.9);
  //
  //     Magnesium
  IlcMaterial( 7, "MAGNESIUM$ ", 24.31, 12., 1.74, 25.3, 46.0);
  //
  //     Aluminum 
  IlcMaterial(9,  "ALUMINIUM0$", 26.98, 13., 2.7, 8.9, 37.2);
  IlcMaterial(29, "ALUMINIUM1$", 26.98, 13., 2.7, 8.9, 37.2);
  IlcMaterial(49, "ALUMINIUM2$", 26.98, 13., 2.7, 8.9, 37.2);
  //
  //     Iron 
  IlcMaterial(10, "IRON0$     ", 55.85, 26., 7.87, 1.76, 17.1);
  IlcMaterial(30, "IRON1$     ", 55.85, 26., 7.87, 1.76, 17.1);
  IlcMaterial(50, "IRON2$     ", 55.85, 26., 7.87, 1.76, 17.1);
  //
  //     Copper
  IlcMaterial(11, "COPPER0$   ", 63.55, 29., 8.96, 1.43, 15.1);
  IlcMaterial(31, "COPPER1$   ", 63.55, 29., 8.96, 1.43, 15.1);
  IlcMaterial(51, "COPPER2$   ", 63.55, 29., 8.96, 1.43, 15.1);
  //
  //     Tungsten 
  IlcMaterial(12, "TUNGSTEN0$ ", 183.85, 74., 19.3, .35, 10.3);
  IlcMaterial(32, "TUNGSTEN1$ ", 183.85, 74., 19.3, .35, 10.3);
  IlcMaterial(52, "TUNGSTEN2$ ", 183.85, 74., 19.3, .35, 10.3);
  //
  //     Ni-W-Cu 
  IlcMixture(21, "Ni-W-Cu0$", aniwcu, zniwcu, 18.78, 3, wniwcu);
  IlcMixture(41, "Ni-W-Cu1$", aniwcu, zniwcu, 18.78, 3, wniwcu);
  IlcMixture(61, "Ni-W-Cu2$", aniwcu, zniwcu, 18.78, 3, wniwcu);
  //
  //     Lead 
  IlcMaterial(13, "LEAD0$     ", 207.19, 82., 11.35, .56, 18.5);
  IlcMaterial(33, "LEAD1$     ", 207.19, 82., 11.35, .56, 18.5);
  IlcMaterial(53, "LEAD2$     ", 207.19, 82., 11.35, .56, 18.5);
  //
  //     Air 
  IlcMixture(15, "AIR0$      ", aAir, zAir, dAir, 4, wAir);
  IlcMixture(35, "AIR1$      ", aAir, zAir, dAir, 4, wAir);
  IlcMixture(55, "AIR2$      ", aAir, zAir, dAir, 4, wAir);
  //
  //     Vacuum 
  IlcMixture(16, "VACUUM0$ ", aAir, zAir, dAir1, 4, wAir);
  IlcMixture(36, "VACUUM1$ ", aAir, zAir, dAir1, 4, wAir);
  IlcMixture(56, "VACUUM2$ ", aAir, zAir, dAir1, 4, wAir);
  //
  //     Concrete 
  IlcMixture(17, "CONCRETE0$", aconc, zconc, 2.35, 10, wconc);
  IlcMixture(37, "CONCRETE1$", aconc, zconc, 2.35, 10, wconc);
  IlcMixture(57, "CONCRETE2$", aconc, zconc, 2.35, 10, wconc);
  //
  //     Poly CH2 
  IlcMixture(18, "POLYETHYLEN0$", apoly, zpoly, .95, -2, wpoly);
  //
  // After a call with ratios by number (negative number of elements), 
  // the ratio array is changed to the ratio by weight, so all successive 
  // calls with the same array must specify the number of elements as 
  // positive 
  //
  IlcMixture(38, "POLYETHYLEN1$", apoly, zpoly, .95, 2, wpoly);
  IlcMixture(58, "POLYETHYLEN2$", apoly, zpoly, .95, 2, wpoly);
  //
  //     stainless Steel 
  IlcMixture(19, "STAINLESS STEEL0$", asteel, zsteel, 7.88, 4, wsteel);
  IlcMixture(39, "STAINLESS STEEL1$", asteel, zsteel, 7.88, 4, wsteel);
  IlcMixture(59, "STAINLESS STEEL2$", asteel, zsteel, 7.88, 4, wsteel);
  //
  //     Insulation powder 
  IlcMixture(14, "INSULATION0$", ains, zins, 0.41, 4, wins);
  IlcMixture(34, "INSULATION1$", ains, zins, 0.41, 4, wins);
  IlcMixture(54, "INSULATION2$", ains, zins, 0.41, 4, wins);
  // Polymere Concrete 
  IlcMixture(20, "Poly Concrete0$", aPolyCc, zPolyCc, 3.53, -9, wPolyCc);
  IlcMixture(40, "Poly Concrete1$", aPolyCc, zPolyCc, 3.53,  9, wPolyCc);
  IlcMixture(60, "Poly Concrete2$", aPolyCc, zPolyCc, 3.53,  9, wPolyCc);

  //
  // **************** 
  //     Defines tracking media parameters. 
  //
  epsil  = .001;    // Tracking precision, 
  stemax = -0.01;   // Maximum displacement for multiple scat 
  tmaxfd = -20.;    // Maximum angle due to field deflection 
  deemax = -.3;     // Maximum fractional energy loss, DLS 
  stmin  = -.8;
  // *************** 
  //
  //    Carbon 
  IlcMedium(6,  "C_C0             ",  6, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(26, "C_C1             ", 26, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(46, "C_C2             ", 46, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Aluminum 
  IlcMedium(9,  "ALU_C0          ",  9, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(29, "ALU_C1          ", 29, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(49, "ALU_C2          ", 49, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Magnesium
  IlcMedium(7,  "MG_C0           ",  7, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Iron 
  IlcMedium(10, "FE_C0           ", 10, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(30, "FE_C1           ", 30, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(50, "FE_C2           ", 50, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Copper
  IlcMedium(11, "Cu_C0            ", 11, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(31, "Cu_C1            ", 31, 0, isxfld, sxmgmx, tmaxfd, -stemax, deemax, epsil, stmin);
  IlcMedium(51, "Cu_C2            ", 51, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Tungsten 
  IlcMedium(12, "W_C0            ", 12, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(32, "W_C1            ", 32, 0, isxfld, sxmgmx, tmaxfd, -stemax, deemax, epsil, stmin);
  IlcMedium(52, "W_C2            ", 52, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //  
  //    Ni/Tungsten 
  IlcMedium(21, "Ni/W0           ", 21, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(41, "Ni/W1           ", 41, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(61, "Ni/W3           ", 61, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Lead 
  IlcMedium(13, "PB_C0           ", 13, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(33, "PB_C1           ", 33, 0, isxfld, sxmgmx, tmaxfd, -stemax, deemax, epsil, stmin);
  IlcMedium(53, "PB_C2           ", 53, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Insulation Powder 
  IlcMedium(14, "INS_C0          ", 14, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(34, "INS_C1          ", 34, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(54, "INS_C2          ", 54, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Air 
  IlcMedium(15, "AIR_C0          ", 15, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(35, "AIR_C1          ", 35, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(55, "AIR_C2          ", 55, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Vacuum 
  IlcMedium(16, "VA_C0           ", 16, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(36, "VA_C1           ", 36, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(56, "VA_C2           ", 56, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Concrete 
  IlcMedium(17, "CC_C0           ", 17, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(37, "CC_C1           ", 37, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(57, "CC_C2           ", 57, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Polyethilene 
  IlcMedium(18, "CH2_C0          ", 18, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(38, "CH2_C1          ", 38, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(58, "CH2_C2          ", 58, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  //    Steel 
  IlcMedium(19, "ST_C0           ", 19, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(39, "ST_C1           ", 39, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(59, "ST_C3           ", 59, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  //
  // Polymer Concrete 
  IlcMedium(20, "PCc_C0           ", 20, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(40, "PCc_C1           ", 40, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(60, "PCc_C3           ", 60, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
}

//_____________________________________________________________________________
void IlcABSO::Init()
{
  //
  // Initialisation of the muon absorber after it has been built
  Int_t i;
  //
  printf("\n");
  for(i=0;i<35;i++) printf("*");
  printf(" ABSO_INIT ");
  for(i=0;i<35;i++) printf("*");
  printf("\n");
  //
  for(i=0;i<80;i++) printf("*");
  printf("\n");
}
 
Int_t  IlcABSO::GetMatId(Int_t imat) const 
{
// Get geant material number
    Int_t kmat=(*fIdmate)[imat]; 
    return kmat;
}
