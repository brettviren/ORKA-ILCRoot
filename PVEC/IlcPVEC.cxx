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
/* $Id: IlcPVEC.cxx 52261 2011-10-23 15:46:57Z hristov $ */
/* History of cvs commits:
 *
 * $Log$
 * Revision 1.116  2007/10/10 09:05:10  schutz
 * Changing name QualAss to QA
 *
 * Revision 1.115  2007/08/22 09:20:50  hristov
 * Updated QA classes (Yves)
 *
 * Revision 1.114  2007/08/07 14:12:03  kharlov
 * Quality assurance added (Yves Schutz)
 *
 * Revision 1.113  2007/07/18 16:29:54  policheh
 * Raw Sdigits energy converted to GeV.
 *
 * Revision 1.112  2007/02/25 22:59:13  policheh
 * Digits2Raw(): ALTRO buffer and mapping created per each DDL.
 *
 * Revision 1.111  2007/02/18 15:21:47  kharlov
 * Corrections for memory leak in Digits2Raw due to IlcAltroMapping
 *
 * Revision 1.110  2007/02/13 10:52:08  policheh
 * Raw2SDigits() implemented
 *
 * Revision 1.109  2007/02/05 10:43:25  hristov
 * Changes for correct initialization of Geant4 (Mihaela)
 *
 * Revision 1.108  2007/02/01 10:34:47  hristov
 * Removing warnings on Solaris x86
 *
 * Revision 1.107  2007/01/29 16:29:37  kharlov
 * Digits2Raw(): special workaround for digits with time out of range
 *
 * Revision 1.106  2007/01/17 17:28:56  kharlov
 * Extract ALTRO sample generation to a separate class IlcPVECPulseGenerator
 *
 * Revision 1.105  2007/01/12 21:44:29  kharlov
 * Simulate and reconstruct two gains simulaneouslsy
 */

//_________________________________________________________________________
// Base Class for PVEC description:
//   PVEC consists of a PbWO4 calorimeter (EMCA) and a gazeous charged 
//    particles detector (CPV or PPSD).
//   The only provided method here is CreateMaterials, 
//    which defines the materials common to all PVEC versions.   
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
#include "IlcPVEC.h"
#include "IlcPVECLoader.h"
#include "IlcRun.h"
#include "IlcRawReader.h"
#include "IlcPVECDigitizer.h"
#include "IlcPVECSDigitizer.h"
#include "IlcPVECDigit.h"
#include "IlcAltroBuffer.h"
#include "IlcAltroMapping.h"
#include "IlcCaloAltroMapping.h"
#include "IlcLog.h"
#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcCDBStorage.h"
#include "IlcPVECCalibData.h"
#include "IlcPVECPulseGenerator.h"
#include "IlcDAQ.h"
#include "IlcPVECRawFitterv0.h"
#include "IlcPVECCalibData.h"
#include "IlcPVECRawDigiProducer.h"
#include "IlcPVECQAChecker.h"
#include "IlcPVECRecoParam.h"
#include "IlcPVECSimParam.h"

ClassImp(IlcPVEC)

//____________________________________________________________________________
  IlcPVEC:: IlcPVEC() : IlcDetector(),fgCalibData(0)
{
  // Default ctor
  fName   = "PVEC" ;

}

//____________________________________________________________________________
IlcPVEC::IlcPVEC(const char* name, const char* title): IlcDetector(name, title),
fgCalibData(0)
{
  //   ctor : title is used to identify the layout
}

//____________________________________________________________________________
IlcPVEC::~IlcPVEC() 
{  
  if(fgCalibData) delete fgCalibData ;
}

//____________________________________________________________________________
IlcDigitizer* IlcPVEC::CreateDigitizer(IlcDigitizationInput* digInput) const
{
  return new IlcPVECDigitizer(digInput);
}

//____________________________________________________________________________
void IlcPVEC::CreateMaterials()
{
  // Definitions of materials to build PVEC and associated tracking media.
  // media number in idtmed are 699 to 798.

  // --- The PbWO4 crystals ---
  Float_t aX[3] = {207.19, 183.85, 16.0} ;
  Float_t zX[3] = {82.0, 74.0, 8.0} ;
  Float_t wX[3] = {1.0, 1.0, 4.0} ;
  Float_t dX = 8.28 ;

  IlcMixture(0, "PbWO4$", aX, zX, dX, -3, wX) ;


  // --- The polysterene scintillator (CH) ---
  Float_t aP[2] = {12.011, 1.00794} ;
  Float_t zP[2] = {6.0, 1.0} ;
  Float_t wP[2] = {1.0, 1.0} ;
  Float_t dP = 1.032 ;

  IlcMixture(1, "Polystyrene$", aP, zP, dP, -2, wP) ;

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

  //CsI
  Float_t aCsI[2]={132.9 ,126.9};
  Float_t zCsI[2]={55. ,53.};
  Float_t wCsI[2]={1. ,1.};
  Float_t dCsI=4.51;
  Int_t   nCsI=-2; 

  IlcMixture(20,"CsI"  ,aCsI  ,zCsI  ,dCsI  ,nCsI  ,wCsI);
 
  // DEFINITION OF THE TRACKING MEDIA

  // for PVEC: idtmed[699->798] equivalent to fIdtmed[0->100]
  Int_t   isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ() ;
  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max() ;

  // The scintillator of the calorimeter made of PBW04                              -> idtmed[699]
  IlcMedium(0, "PbWO4$", 0, 1,
	    isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The scintillator of the CPV made of Polystyrene scintillator                   -> idtmed[700]
  IlcMedium(1, "CPV scint.   $", 1, 1,
	    isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Various Aluminium parts made of Al                                             -> idtmed[701]
  IlcMedium(2, "Al parts     $", 2, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.001, 0.001, 0, 0) ;

  // The Tywek which wraps the calorimeter crystals                                 -> idtmed[702]
  IlcMedium(3, "Tyvek wrapper$", 3, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.001, 0.001, 0, 0) ;

  // The Polystyrene foam around the calorimeter module                             -> idtmed[703]
  IlcMedium(4, "Polyst. foam $", 4, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The Titanium around the calorimeter crystal                                    -> idtmed[704]
  IlcMedium(5, "Titan. cover $", 5, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.0001, 0.0001, 0, 0) ;

  // The Silicon of the pin diode to read out the calorimeter crystal               -> idtmed[705] 
 IlcMedium(6, "Si PIN       $", 6, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.01, 0.01, 0, 0) ;

 // The thermo insulating material of the box which contains the calorimeter module -> idtmed[706]
  IlcMedium(7, "Thermo Insul.$", 7, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The Textolit which makes up the box which contains the calorimeter module      -> idtmed[707]
  IlcMedium(8, "Textolit     $", 8, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // FR4: The Plastic which makes up the frame of micromegas                        -> idtmed[708]
  IlcMedium(9, "FR4 $", 9, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.0001, 0, 0) ; 


  // The Composite Material for  micromegas                                         -> idtmed[709]
  IlcMedium(10, "CompoMat   $", 10, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Copper                                                                         -> idtmed[710]
  IlcMedium(11, "Copper     $", 11, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.0001, 0, 0) ;

  // G10: Printed Circuit material                                                  -> idtmed[711]
 
  IlcMedium(12, "G10        $", 12, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.01, 0, 0) ;

  // The Lead                                                                       -> idtmed[712]
 
  IlcMedium(13, "Lead      $", 13, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // The gas mixture: ArCo2                                                         -> idtmed[715]
 
  IlcMedium(16, "ArCo2      $", 16, 1,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.01, 0, 0) ;
 
  // Stainless steel                                                                -> idtmed[716]
  IlcMedium(17, "Steel     $", 17, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.0001, 0, 0) ;

  // Fibergalss                                                                     -> idtmed[717]
  IlcMedium(18, "Fiberglass$", 18, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Cables in air                                                                  -> idtmed[718]
  IlcMedium(19, "Cables    $", 19, 0,
	     isxfld, sxmgmx, 10.0, 0.1, 0.1, 0.1, 0.1, 0, 0) ;

  // Air                                                                            -> idtmed[798] 
  IlcMedium(99, "Air          $", 99, 0,
	     isxfld, sxmgmx, 10.0, 1.0, 0.1, 0.1, 10.0, 0, 0) ;

  //CsI
   IlcMedium(20, "CsI", 20, 1, isxfld, sxmgmx, 10., 0.1, 0.1, 0.001, 0.001, 0, 0);


}

//_____________________________________________________________________________
void IlcPVEC::Init()
{
}

//____________________________________________________________________________
void IlcPVEC::Digits2Raw()
{
// convert digits of the current event to raw data
  IlcError("method not implemented");
}

//____________________________________________________________________________
void IlcPVEC::Hits2SDigits()  
{ 
// create summable digits

  IlcPVECSDigitizer PVECDigitizer(fLoader->GetRunLoader()->GetFileName().Data()) ;
  PVECDigitizer.SetEventRange(0, -1) ; // do all the events
 
  PVECDigitizer.Digitize("all") ; 
}


//____________________________________________________________________________
IlcLoader* IlcPVEC::MakeLoader(const char* topfoldername)
{
//different behaviour than standard (singleton getter)
// --> to be discussed and made eventually coherent
 fLoader = new IlcPVECLoader(GetName(),topfoldername);
 return fLoader;
}

//____________________________________________________________________________
void IlcPVEC::SetTreeAddress()
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
       if (fHits == 0x0) fHits= new TClonesArray("IlcPVECHit",1000);
       //IlcInfo(Form("<%s> Setting Hits Address",GetName()));
       branch->SetAddress(&fHits);
     }
  }
}

//____________________________________________________________________________ 	 
Bool_t IlcPVEC::Raw2SDigits(IlcRawReader* /*rawReader*/) 	 
{ 	 
	  	 
  IlcError("method not implemented");
  return 0;
  
}
