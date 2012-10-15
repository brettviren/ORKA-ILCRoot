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

/* $Id: IlcBODY.cxx 49869 2011-05-18 04:49:51Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Ilc external volume                                                    //
//  This class contains the description of the Ilc external volume         //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcBODYClass.gif">
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
#include <TVirtualMC.h>
#include <TArrayI.h>

#include "IlcBODY.h"
#include "IlcMagF.h"
#include "IlcRun.h"

ClassImp(IlcBODY)
 
//_____________________________________________________________________________
IlcBODY::IlcBODY()
{
  //
  // Default constructor
  //
}
 
//_____________________________________________________________________________
IlcBODY::IlcBODY(const char *name, const char *title)
       : IlcModule(name,title)
{
  //
  // Standard constructor of the Ilc external volume
  //
  //PH  SetMarkerColor(7);
  //PH  SetMarkerStyle(2);
  //PH  SetMarkerSize(0.4);
}
 
//_____________________________________________________________________________
void IlcBODY::CreateGeometry()
{
  //
  // Create the geometry of the Ilc external body
  //
  //Begin_Html
  /*
    <img src="picts/IlcBODYTree.gif">
  */
  //End_Html
  //
  // If the ZDC is present we have an asymmetric box
  // made by a four sides polygone
  //  
  //Begin_Html
  /*
    <img src="picts/IlcBODYLarge.gif">
  */
  //End_Html
  //
  // If the ZDC is not present make just a BOX
  //
  //Begin_Html
  /*
    <img src="picts/IlcBODYSmall.gif">
  */
  //End_Html

  Float_t dILCM[10];
  Int_t *idtmed = fIdtmed->GetArray()+1;
  //
  if(gIlc->GetModule("ZDC")) {
    //
    // If the ZDC is present we have an asymmetric box
    // made by a four sides polygone
    //
    dILCM[0]=45;
    dILCM[1]=360;
    dILCM[2]=4;
    dILCM[3]=2;

    dILCM[4]=-15000;
    dILCM[5]=0;
    dILCM[6]=2500;

    dILCM[7]=15000;
    dILCM[8]=0;
    dILCM[9]=2500;
    gMC->Gsvolu("ILCM","PGON",idtmed[1],dILCM,10);
  } else if ( gIlc->GetModule("ACORDE")) {
    //
    // If the Cosmic Ray Trigger  is present we need a large box
    // 
    //
    dILCM[0]=13000.;
    dILCM[1]=5000.;
    dILCM[2]=13000.;
    gMC->Gsvolu("ILCM","BOX ",idtmed[1],dILCM,3);
      
  } else {
    //
    // If the ZDC and ACORDE are not present make just a BOX
    //
    dILCM[0]=2000;
    dILCM[1]=2000;
    dILCM[2]=3000;
    gMC->Gsvolu("ILCM","BOX ",idtmed[1],dILCM,3);
  }
}
 
//_____________________________________________________________________________
void IlcBODY::CreateMaterials()
{
// Create materials and media
  Int_t isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ();
  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max();
  
  // AIR

  Float_t aAir[4]={12.0107,14.0067,15.9994,39.948};
  Float_t zAir[4]={6.,7.,8.,18.};
  Float_t wAir[4]={0.000124,0.755267,0.231781,0.012827};
  Float_t dAir = 1.20479E-3 * 960./1014.;
  Float_t dAir1 = 1.20479E-10;
  //
  IlcMixture(1,"Vacuum  $",aAir,zAir,dAir1,4,wAir);
  IlcMixture(2,"Air     $",aAir,zAir,dAir,4,wAir);
  IlcMaterial(3,"Be      $", 9.01,4 ,1.848   ,35.30,36.70);
  //
  IlcMedium(1,"Vacuum  $",1,0,isxfld,sxmgmx,10,1,0.1,0.1,10);
  IlcMedium(2,"Air     $",2,0,isxfld,sxmgmx,10,-1,-0.1,0.1 ,-10);
  IlcMedium(3,"Be pipe $",3,0,isxfld,sxmgmx,10,0.1,0.1,0.01,0.01);
}
 
