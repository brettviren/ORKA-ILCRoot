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
/* $Id: IlcPVECv0.cxx 47780 2011-02-24 15:37:59Z kharlov $ */


//_________________________________________________________________________
// Implementation version v0 of PVEC Manager class 
// An object of this class does not produce hits nor digits
// It is the one to use if you do not want to produce outputs in TREEH or TREED
//                  
//*-- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC KI & SUBATECH)


// --- ROOT system ---

#include <TFolder.h>
#include <TGeometry.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TTree.h>
#include <TVirtualMC.h>
#include <TGeoPhysicalNode.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TVector3.h>
#include <TGeoTube.h>
#include <TBRIK.h>
#include <TTUBS.h>
#include <TNode.h>

// --- Standard library ---

#include <string.h>
#include <stdlib.h>

// --- IlcRoot header files ---

#include "IlcConst.h"
#include "IlcPVECGeometry.h"
#include "IlcPVECLoader.h"
#include "IlcPVECv0.h"
#include "IlcRun.h"
#include "IlcLog.h"
#include "IlcGeomManager.h"

ClassImp(IlcPVECv0)

//____________________________________________________________________________
IlcPVECv0::IlcPVECv0(const char *name, const char *title):
  IlcPVEC(name,title)
{
  // ctor : title is used to identify the layout
  GetGeometry() ; 
}


//____________________________________________________________________________
TList* IlcPVECv0::BuildGeometry2()
{
  // Build the PVEC geometry for the ROOT display

  TList *Nodes = new TList();       //List of geometry nodes

  new TMaterial("void","Vacuum",0,0,0);  //Everything is void
  TBRIK *brik = new TBRIK("S_ilc","ilc volume","void",2000,2000,3000);
  brik->SetVisibility(0);
  TNode *top = new TNode("ilc","ilc","S_ilc");


  IlcPVECGeometry * geom = GetGeometry() ; 

  Double_t PVECRmin = geom->GetPVECRmin();
  Double_t PVECRmax = geom->GetPVECRmax();
  Double_t PVECLength = geom->GetPVECLength();
  Int_t PVECNSlicePhi = geom->GetPVECNSlicePhi();
  Double_t zEC=geom->GetPVECZPosition();

  Double_t PVECparam[3] = {PVECRmin, PVECRmax, PVECLength/2.};

  TTUBS *tubs = new TTUBS("sPVEC","sPVEC","void",PVECparam[0],PVECparam[1],PVECparam[2], 0., 360.);
  tubs->SetNumberOfDivisions(PVECNSlicePhi);
  top->cd();

  TNode *nNode1 = new TNode("PVEC1","PVEC1","sPVEC",0.,0.,-(zEC+PVECLength/2.),"");
  nNode1->SetLineColor(7);
  Nodes->Add(nNode1);

  TNode *nNode2 = new TNode("PVEC2","PVEC2","sPVEC",0.,0.,(zEC+PVECLength/2.),"");
  nNode2->SetLineColor(7);
  Nodes->Add(nNode2);

  return Nodes;
}



//____________________________________________________________________________
void IlcPVECv0::CreateGeometry()
{
  // Create the PVEC geometry for Geant

  IlcPVECv0 *PVECtmp = dynamic_cast<IlcPVECv0*>(gIlc->GetModule("PVEC")) ;

  if ( PVECtmp == NULL ) {
    
    fprintf(stderr, "PVEC detector not found!\n") ;
    return;
    
  }

  // Create a PVEC module.
  TGeoMedium *Air=gGeoManager->GetMedium("PVEC_Air          $");
  Int_t CsI_Id=gGeoManager->GetMedium("PVEC_CsI")->GetId();

  TGeoVolume *ilc = gGeoManager->GetVolume("ILCM");

  IlcPVECGeometry * geom = GetGeometry() ; 

  Double_t PVECRmin = geom->GetPVECRmin();
  Double_t PVECRmax = geom->GetPVECRmax();
  Double_t PVECLength = geom->GetPVECLength();
  const Int_t nLay = geom->GetPVECNLayers();
#ifdef WIN32
  Double_t *LayerThickness		= (Double_t *)malloc(nLay);
  Int_t *NXtalPhi		= (Int_t *)malloc(nLay);
#else
  Double_t LayerThickness[nLay];
  Int_t NXtalPhi[nLay];
#endif 

  for(Int_t iLay=0; iLay<nLay; iLay++){
    LayerThickness[iLay] = geom->GetPVECLayerThickness(iLay+1);
    NXtalPhi[iLay] = geom->GetPVECNXtalPhi(iLay+1);
  }
  Double_t XTalEnvelopThickness = geom->GetPVECXTalEnvelopThickness();
  Double_t zEC=geom->GetPVECZPosition();
  
  Double_t PVECparam[3] = {PVECRmin, PVECRmax, PVECLength/2.};


  TGeoTube *PVECtube = new TGeoTube(PVECparam[0], PVECparam[1], PVECparam[2]);
  TGeoVolume *VolPVECtube = new TGeoVolume("PVEC_TUBE", PVECtube, Air);

#ifdef WIN32
	TGeoTube **PVECLayer		= (TGeoTube **)malloc(nLay);
	TGeoVolume **VolPVECLayer	= (TGeoVolume **)malloc(nLay);
#else
  TGeoTube *PVECLayer[nLay];
  TGeoVolume *VolPVECLayer[nLay]; 
#endif 

  
  PVECparam[1] = PVECparam[0];
  for(Int_t iLay=0; iLay<nLay; iLay++){
    PVECparam[0] += (iLay>0) ? LayerThickness[iLay-1]+2.*XTalEnvelopThickness : 0.;
    PVECparam[1] += LayerThickness[iLay]+2.*XTalEnvelopThickness;
    PVECLayer[iLay] = new TGeoTube(PVECparam[0], PVECparam[1], PVECparam[2]);
    VolPVECLayer[iLay] = new TGeoVolume(Form("PVEC_LAY_%d", iLay+1), PVECLayer[iLay], Air);
    VolPVECLayer[iLay]->Divide("PVEC_CsIXtal", 2, NXtalPhi[iLay], 0., 360./NXtalPhi[iLay],CsI_Id);
  
    VolPVECtube->AddNode(VolPVECLayer[iLay],iLay+1);
  }



  TGeoRotation rot1("rot1", 0., 0., 0.);
  TGeoTranslation trans1(0., 0., (zEC+PVECLength/2.));
  TGeoCombiTrans *mat1 = new TGeoCombiTrans(trans1, rot1);
  mat1->RegisterYourself();

  TGeoRotation rot2("rot2", 90., 0.,90.,-90.,180.,0.);
  TGeoTranslation trans2(0., 0., -(zEC+PVECLength/2.));
  TGeoCombiTrans *mat2 = new TGeoCombiTrans(trans2, rot2);
  mat2->RegisterYourself();

  ilc->AddNode(VolPVECtube, 1,mat1);
  ilc->AddNode(VolPVECtube, 2,mat2);

#ifdef WIN32
		//free(LayerThickness);
		//free(NXtalPhi);
		//free(PVECLayer);
		//free(VolPVECLayer);
		//delete [] LayerThickness;
		//delete [] NXtalPhi;
		//delete [] PVECLayer;
		//delete [] VolPVECLayer;
#endif 


}

//____________________________________________________________________________
void IlcPVECv0::CreateGeometryforSupport()
{
  // Create the PVEC support geometry for GEANT

  //not yet implemented

}

//_____________________________________________________________________________
void IlcPVECv0::AddAlignableVolumes() const
{

  IlcWarning("method not yet implemented for ORKA PVEC");
  return; //FIXME

  //
  // Create entries for alignable volumes associating the symbolic volume
  // name with the corresponding volume path. Needs to be syncronized with
  // eventual changes in the geometry
  // Alignable volumes are:
  // 1) PVEC modules as a whole
  // 2) ...

}

//____________________________________________________________________________
Float_t IlcPVECv0::ZMin(void) const
{
  // Overall dimension of the PVEC (min)

  IlcPVECGeometry * geom = GetGeometry() ; 

  return -geom->GetPVECLength()/2.;
}

//____________________________________________________________________________
Float_t IlcPVECv0::ZMax(void) const
{
  // Overall dimension of the PVEC (max)

  IlcPVECGeometry * geom = GetGeometry() ; 

  return  geom->GetPVECLength()/2.;
}

//____________________________________________________________________________
void IlcPVECv0::Init(void)
{
  // Just prints an information message
  
  Int_t i;

  if(IlcLog::GetGlobalDebugLevel()>0) {
    TString st ; 
    for(i=0;i<35;i++) 
      st += "*";
    Info("Init", "%s", st.Data()) ;  
    // Here the PVEC initialisation code (if any!)
    
    IlcPVECGeometry * geom = GetGeometry() ; 

    if (geom!=0)  
      Info("Init", "IlcPVEC%s: PVEC geometry intialized for %s", Version().Data(), geom->GetName()) ;
    else
      Info("Init", "IlcPVEC%s: PVEC geometry initialization failed !", Version().Data()) ;       

    Info("Init", "%s", st.Data()) ;  
  }
}
