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
/* $Id: IlcRSTACK.cxx 52261 2011-10-23 15:46:57Z hristov $ */


//_________________________________________________________________________
// Base Class for RSTACK description:
//   RSTACK consists of a Scintillator tiles
//   The only provided method here is CreateMaterials, 
//    which defines the materials common to all RSTACK versions.   
// 
//*-- Author: Laurent Aphecetche & Yves Schutz (SUBATECH) 
//////////////////////////////////////////////////////////////////////////////


// --- ROOT system ---
class TFile;
#include <TF1.h> 
#include <TFolder.h> 
#include <TGeoGlobalMagField.h>
#include <TH1F.h> 
#include <TRandom.h> 
#include <TTree.h>
#include <TVirtualMC.h> 

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcMagF.h"
#include "IlcRSTACK.h"
#include "IlcRSTACKLoader.h"
#include "IlcRun.h"
#include "IlcRawReader.h"
#include "IlcRSTACKDigitizer.h"
#include "IlcRSTACKSDigitizer.h"
#include "IlcRSTACKDigit.h"
#include "IlcAltroBuffer.h"
#include "IlcAltroMapping.h"
#include "IlcCaloAltroMapping.h"
#include "IlcLog.h"
#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcCDBStorage.h"
#include "IlcRSTACKCalibData.h"
#include "IlcRSTACKPulseGenerator.h"
#include "IlcDAQ.h"
#include "IlcRSTACKRawFitterv0.h"
#include "IlcRSTACKCalibData.h"
#include "IlcRSTACKRawDigiProducer.h"
#include "IlcRSTACKQAChecker.h"
#include "IlcRSTACKRecoParam.h"
#include "IlcRSTACKSimParam.h"

ClassImp(IlcRSTACK)

//____________________________________________________________________________
  IlcRSTACK:: IlcRSTACK() : IlcDetector(),fgCalibData(0)
{
  // Default ctor
  fName   = "RSTACK" ;

}

//____________________________________________________________________________
IlcRSTACK::IlcRSTACK(const char* name, const char* title): IlcDetector(name, title),
fgCalibData(0)
{
  //   ctor : title is used to identify the layout
}

//____________________________________________________________________________
IlcRSTACK::~IlcRSTACK() 
{  
  if(fgCalibData) delete fgCalibData ;
}

//____________________________________________________________________________
IlcDigitizer* IlcRSTACK::CreateDigitizer(IlcDigitizationInput* digInput) const
{
  return new IlcRSTACKDigitizer(digInput);
}

//____________________________________________________________________________
void IlcRSTACK::CreateMaterials()
{
  // Definitions of materials to build RSTACK and associated tracking media.

  // --- The PbWO4 crystals ---
  Float_t aX[3] = {207.19, 183.85, 16.0} ;
  Float_t zX[3] = {82.0, 74.0, 8.0} ;
  Float_t wX[3] = {1.0, 1.0, 4.0} ;
  Float_t dX = 8.28 ;

  IlcMixture(0, "PbWO4$", aX, zX, dX, -3, wX) ;


  // --- The BC408 Polyvinyltoluene scintillator (C9H10) ---
  Float_t aP[2] = {12.011, 1.00794} ;
  Float_t zP[2] = {6.0, 1.0} ;
  Float_t wP[2] = {9.0, 10.0} ;
  Float_t dP = 1.032 ;

  IlcMixture(1, "BC408", aP, zP, dP, -2, wP) ;

  // --- Aluminium ---
  IlcMaterial(2, "Al$", 26.98, 13., 2.7, 8.9, 999., 0, 0) ;
  // ---         Absorption length is ignored ^

 // --- Tyvek (CnH2n) ---
  Float_t aT[2] = {12.011, 1.00794} ;
  Float_t zT[2] = {6.0, 1.0} ;
  Float_t wT[2] = {1.0, 2.0} ;
  Float_t dT = 0.331 ;

  IlcMixture(3, "Tyvek$", aT, zT, dT, -2, wT) ;

  // --- Polystyrene foam ---
  Float_t aF[2] = {12.011, 1.00794} ;
  Float_t zF[2] = {6.0, 1.0} ;
  Float_t wF[2] = {1.0, 1.0} ;
  Float_t dF = 0.12 ;

  IlcMixture(4, "Foam$", aF, zF, dF, -2, wF) ;

 // --- Titanium ---
  Float_t aTIT[3] = {47.88, 26.98, 54.94} ;
  Float_t zTIT[3] = {22.0, 13.0, 25.0} ;
  Float_t wTIT[3] = {69.0, 6.0, 1.0} ;
  Float_t dTIT = 4.5 ;

  IlcMixture(5, "Titanium$", aTIT, zTIT, dTIT, -3, wTIT);

 // --- Silicon ---
  IlcMaterial(6, "Si$", 28.0855, 14., 2.33, 9.36, 42.3, 0, 0) ;



  // --- Foam thermo insulation ---
  Float_t aTI[2] = {12.011, 1.00794} ;
  Float_t zTI[2] = {6.0, 1.0} ;
  Float_t wTI[2] = {1.0, 1.0} ;
  Float_t dTI = 0.04 ;

  IlcMixture(7, "Thermo Insul.$", aTI, zTI, dTI, -2, wTI) ;

  // --- Textolith ---
  Float_t aTX[4] = {16.0, 28.09, 12.011, 1.00794} ;
  Float_t zTX[4] = {8.0, 14.0, 6.0, 1.0} ;
  Float_t wTX[4] = {292.0, 68.0, 462.0, 736.0} ;
  Float_t dTX    = 1.75 ;

  IlcMixture(8, "Textolit$", aTX, zTX, dTX, -4, wTX) ;

  //--- FR4  ---
  Float_t aFR[4] = {16.0, 28.09, 12.011, 1.00794} ;
  Float_t zFR[4] = {8.0, 14.0, 6.0, 1.0} ;
  Float_t wFR[4] = {292.0, 68.0, 462.0, 736.0} ;
  Float_t dFR = 1.8 ; 

  IlcMixture(9, "FR4$", aFR, zFR, dFR, -4, wFR) ;

  // --- The Composite Material for  micromegas (so far polyetylene) ---                                       
  Float_t aCM[2] = {12.01, 1.} ; 
  Float_t zCM[2] = {6., 1.} ; 
  Float_t wCM[2] = {1., 2.} ; 
  Float_t dCM = 0.935 ; 

  IlcMixture(10, "Compo Mat$", aCM, zCM, dCM, -2, wCM) ;

  // --- Copper ---                                                                    
  IlcMaterial(11, "Cu$", 63.546, 29, 8.96, 1.43, 14.8, 0, 0) ;
 
  // --- G10 : Printed Circuit material ---                                                  
  Float_t aG10[4] = { 12., 1., 16., 28.} ;
  Float_t zG10[4] = { 6., 1., 8., 14.} ;
  Float_t wG10[4] = { .259, .288, .248, .205} ;
  Float_t dG10  = 1.7 ; 

  
  IlcMixture(12, "G10$", aG10, zG10, dG10, -4, wG10);

  // --- Lead ---                                                                     
  IlcMaterial(13, "Pb$", 207.2, 82, 11.35, 0.56, 0., 0, 0) ;

 // --- The gas mixture ---                                                                
 // Co2
  Float_t aCO[2] = {12.0, 16.0} ; 
  Float_t zCO[2] = {6.0, 8.0} ; 
  Float_t wCO[2] = {1.0, 2.0} ; 
  Float_t dCO = 0.001977 ; 

  IlcMixture(14, "CO2$", aCO, zCO, dCO, -2, wCO);

 // Ar
  Float_t dAr = 0.001782 ; 
  IlcMaterial(15, "Ar$", 39.948, 18.0, dAr, 14.0, 0., 0, 0) ;   
 
  // Ar+CO2 Mixture (80% / 20%)
  Float_t arContent = 0.80 ;  // Ar-content of the ArCO2-mixture
  Float_t aArCO[3]  = {39.948, 12.0, 16.0} ;
  Float_t zArCO[3]  = {18.0  ,  6.0,  8.0} ;
  Float_t wArCO[3];
  wArCO[0] = arContent;
  wArCO[1] = (1-arContent)*1;
  wArCO[2] = (1-arContent)*2;
  Float_t dArCO = arContent*dAr + (1-arContent)*dCO ;
  IlcMixture(16, "ArCO2$", aArCO, zArCO, dArCO,  -3, wArCO) ; 


  // --- Stainless steel (let it be pure iron) ---
  IlcMaterial(17, "Steel$", 55.845, 26, 7.87, 1.76, 0., 0, 0) ;


  // --- Fiberglass ---
  Float_t aFG[4] = {16.0, 28.09, 12.011, 1.00794} ;
  Float_t zFG[4] = {8.0, 14.0, 6.0, 1.0} ;
  Float_t wFG[4] = {292.0, 68.0, 462.0, 736.0} ;
  Float_t dFG    = 1.9 ;

  IlcMixture(18, "Fibergla$", aFG, zFG, dFG, -4, wFG) ;

  // --- Cables in Air box  ---
  // SERVICES

  Float_t aCA[4] = { 1.,12.,55.8,63.5 };
  Float_t zCA[4] = { 1.,6.,26.,29. }; 
  Float_t wCA[4] = { .014,.086,.42,.48 };
  Float_t dCA    = 0.8 ;  //this density is raw estimation, if you know better - correct

  IlcMixture(19, "Cables  $", aCA, zCA, dCA, -4, wCA) ;


  // --- Air ---
  Float_t aAir[4]={12.0107,14.0067,15.9994,39.948};
  Float_t zAir[4]={6.,7.,8.,18.};
  Float_t wAir[4]={0.000124,0.755267,0.231781,0.012827};
  Float_t dAir = 1.20479E-3;
 
  IlcMixture(99, "Air$", aAir, zAir, dAir, 4, wAir) ;

  // DEFINITION OF THE TRACKING MEDIA

  Int_t   isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ() ;
  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max() ;

  // The scintillator of the calorimeter made of PBW04                  
  IlcMedium(0, "RSTACK Xtal    $", 0, 1,
	    isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The BC408 scintillator for the RSTACK tiles
  IlcMedium(1, "Scint", 1, 1,
	    isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Various Aluminium parts made of Al                                 
  IlcMedium(2, "Al parts     $", 2, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.001, 0.001, 0, 0) ;

  // The Tywek which wraps the calorimeter crystals                     
  IlcMedium(3, "Tyvek wrapper$", 3, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.001, 0.001, 0, 0) ;

  // The Polystyrene foam around the calorimeter module                 
  IlcMedium(4, "Polyst. foam $", 4, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The Titanium around the calorimeter crystal                        
  IlcMedium(5, "Titan. cover $", 5, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.0001, 0.0001, 0, 0) ;

  // The Silicon of the pin diode to read out the calorimeter crystal   
 IlcMedium(6, "Si PIN       $", 6, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.01, 0.01, 0, 0) ;

 // The thermo insulating material of the box which contains the calorimeter module 
  IlcMedium(7, "Thermo Insul.$", 7, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The Textolit which makes up the box which contains the calorimeter module
  IlcMedium(8, "Textolit     $", 8, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // FR4: The Plastic which makes up the frame of micromegas      
  IlcMedium(9, "FR4 $", 9, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.0001, 0, 0) ; 


  // The Composite Material for  micromegas                       
  IlcMedium(10, "CompoMat   $", 10, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Copper                                                       
  IlcMedium(11, "Copper     $", 11, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.0001, 0, 0) ;

  // G10: Printed Circuit material                                
 
  IlcMedium(12, "G10        $", 12, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.01, 0, 0) ;

  // The Lead                                                     
 
  IlcMedium(13, "Lead      $", 13, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The gas mixture: ArCo2                                       
 
  IlcMedium(16, "ArCo2      $", 16, 1,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.01, 0, 0) ;
 
  // Stainless steel                                              
  IlcMedium(17, "Steel     $", 17, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.0001, 0, 0) ;

  // Fibergalss                                                   
  IlcMedium(18, "Fiberglass$", 18, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Cables in air                                                
  IlcMedium(19, "Cables    $", 19, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Air                                                          
  IlcMedium(99, "Air", 99, 0,
	     isxfld, sxmgmx, 10.0, 1.0, 0.1, 0.1, 10.0, 0, 0) ;
}

//_____________________________________________________________________________
void IlcRSTACK::Init()
{
}

//____________________________________________________________________________
void IlcRSTACK::Digits2Raw()
{
// convert digits of the current event to raw data
  IlcError("method not implemented");
}

//____________________________________________________________________________
void IlcRSTACK::Hits2SDigits()  
{ 
// create summable digits

  IlcRSTACKSDigitizer RSTACKDigitizer(fLoader->GetRunLoader()->GetFileName().Data()) ;
  RSTACKDigitizer.SetEventRange(0, -1) ; // do all the events
 
  RSTACKDigitizer.Digitize("all") ; 
}


//____________________________________________________________________________
IlcLoader* IlcRSTACK::MakeLoader(const char* topfoldername)
{
//different behaviour than standard (singleton getter)
// --> to be discussed and made eventually coherent
 fLoader = new IlcRSTACKLoader(GetName(),topfoldername);
 return fLoader;
}

//____________________________________________________________________________
void IlcRSTACK::SetTreeAddress()
{ 
  // Links Hits in the Tree to Hits array
  TBranch *branch;
  char branchname[20];
  snprintf(branchname,20,"%s",GetName());
  // Branch address for hit tree
    TTree *treeH = fLoader->TreeH();
  if (treeH) {
    branch = treeH->GetBranch(branchname);
    if (branch) 
     { 
       if (fHits == 0x0) fHits= new TClonesArray("IlcRSTACKHit",1000);
       //IlcInfo(Form("<%s> Setting Hits Address",GetName()));
       branch->SetAddress(&fHits);
     }
  }
}

//____________________________________________________________________________ 	 
Bool_t IlcRSTACK::Raw2SDigits(IlcRawReader* /*rawReader*/) 	 
{ 	 
	  	 
  IlcError("method not implemented");
  return 0;

}
