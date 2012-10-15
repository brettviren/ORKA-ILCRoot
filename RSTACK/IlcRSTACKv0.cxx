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


//_________________________________________________________________________
// Implementation version v0 of RSTACK Manager class 
// An object of this class does not produce hits nor digits
// It is the one to use if you do not want to produce outputs in TREEH or TREED
//                  


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
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACKv0.h"
#include "IlcRun.h"
#include "IlcLog.h"
#include "IlcGeomManager.h"

ClassImp(IlcRSTACKv0)

//____________________________________________________________________________
IlcRSTACKv0::IlcRSTACKv0(const char *name, const char *title):
  IlcRSTACK(name,title)
{
  // ctor : title is used to identify the layout
  GetGeometry() ; 
}


//____________________________________________________________________________
TList* IlcRSTACKv0::BuildGeometry2()
{
  // Build the RSTACK geometry for the ROOT display

  TList *Nodes = new TList();       //List of geometry nodes

  new TMaterial("void","Vacuum",0,0,0);  //Everything is void
  TBRIK *brik = new TBRIK("S_ilc","ilc volume","void",2000,2000,3000);
  brik->SetVisibility(0);
  TNode *top = new TNode("ilc","ilc","S_ilc");


  IlcRSTACKGeometry * geom = GetGeometry() ; 

  Double_t RSTACKRmin = geom->GetRSTACKRmin();
  Double_t RSTACKRmax = geom->GetRSTACKRmax();
  Double_t RSTACKLength = geom->GetRSTACKLength();
  Int_t    RSTACKNSectorPhi = geom->GetRSTACKNSectorPhi();
  
  Double_t RSTACKparam[3] = {RSTACKRmin, RSTACKRmax, RSTACKLength/2.};

  TTUBS *tubs = new TTUBS("sRSTACK","sRSTACK","void",RSTACKparam[0],RSTACKparam[1],RSTACKparam[2], 0., 360.);
  tubs->SetNumberOfDivisions(RSTACKNSectorPhi);
  top->cd();
  TNode *nNode = new TNode("RSTACK","RSTACK","sRSTACK",0.,0.,0.,"");
  nNode->SetLineColor(6);
  Nodes->Add(nNode);

  return Nodes;
}



//____________________________________________________________________________
void IlcRSTACKv0::CreateGeometry()
{
  // Create the RSTACK geometry for Geant

  IlcRSTACKv0 *RSTACKtmp = dynamic_cast<IlcRSTACKv0*>(gIlc->GetModule("RSTACK")) ;

  if ( RSTACKtmp == NULL ) {
    
    fprintf(stderr, "RSTACK detector not found!\n") ;
    return;
    
  }

  // Create a RSTACK module.
  TGeoMedium *Air=gGeoManager->GetMedium("RSTACK_Air");
  Int_t ScintId=gGeoManager->GetMedium("RSTACK_Scint")->GetId();

  TGeoVolume *ilc = gGeoManager->GetVolume("ILCM");

  IlcRSTACKGeometry * geom = GetGeometry() ; 

  Double_t RSTACKRmin = geom->GetRSTACKRmin();
  Double_t RSTACKRmax = geom->GetRSTACKRmax();
//   Double_t RSTACKThick = RSTACKRmax-RSTACKRmin;
  Double_t RSTACKLength = geom->GetRSTACKLength();
  Int_t    RSTACKNSectorPhi = geom->GetRSTACKNSectorPhi();
  Double_t RSTACKTileScintThickness = geom->GetRSTACKTileScintThickness(); //cm
  Int_t RSTACKNTiles = geom->GetRSTACKNTilesInSector();

  Double_t RSTACKparam[3] = {RSTACKRmin, RSTACKRmax, RSTACKLength/2.};


  TGeoTube *RSTACKtube = new TGeoTube(RSTACKparam[0], RSTACKparam[1], RSTACKparam[2]);
  TGeoVolume *VolRSTACKtube = new TGeoVolume("RSTACK_TUBE", RSTACKtube, Air);

  Double_t CellAngleDegPhi=360./RSTACKNSectorPhi;
  VolRSTACKtube->Divide("RSTACK_BSECTOR", 2, RSTACKNSectorPhi, 0., CellAngleDegPhi);

  TGeoVolume *VolRSTACKSectorPhi = gGeoManager->GetVolume("RSTACK_BSECTOR");
  TString RSTACKTileName("RSTACK_BSCITILES");
  
  Double_t RSTACKTileParam[5];
  TGeoVolume *VolRSTACKTile = gGeoManager->Volume(RSTACKTileName.Data(), "tubs", ScintId, RSTACKTileParam, 0);
  for(Int_t idx=0; idx<RSTACKNTiles; idx++){
    RSTACKTileParam[0] = RSTACKRmin+idx*RSTACKTileScintThickness;
    RSTACKTileParam[1] = RSTACKTileParam[0]+RSTACKTileScintThickness;
    RSTACKTileParam[2] = RSTACKLength/2.;
    RSTACKTileParam[3] = -CellAngleDegPhi/2.;
    RSTACKTileParam[4] = CellAngleDegPhi/2.;
    gGeoManager->Node(VolRSTACKTile->GetName(), idx+1, VolRSTACKSectorPhi->GetName(), 0., 0., 0., 0, 1, RSTACKTileParam, 5);
  }



  ilc->AddNode(VolRSTACKtube, 1);

}

//____________________________________________________________________________
void IlcRSTACKv0::CreateGeometryforSupport()
{
  // Create the RSTACK support geometry for GEANT

  //not yet implemented

}

//_____________________________________________________________________________
void IlcRSTACKv0::AddAlignableVolumes() const
{

  IlcWarning("method not yet implemented for ORKA RSTACK");
  return; //FIXME

  //
  // Create entries for alignable volumes associating the symbolic volume
  // name with the corresponding volume path. Needs to be syncronized with
  // eventual changes in the geometry
  // Alignable volumes are:
  // 1) RSTACK modules as a whole
  // 2) ...

}

//____________________________________________________________________________
Float_t IlcRSTACKv0::ZMin(void) const
{
  // Overall dimension of the RSTACK (min)

  IlcRSTACKGeometry * geom = GetGeometry() ; 

  return -geom->GetRSTACKLength()/2.;
}

//____________________________________________________________________________
Float_t IlcRSTACKv0::ZMax(void) const
{
  // Overall dimension of the RSTACK (max)

  IlcRSTACKGeometry * geom = GetGeometry() ; 

  return  geom->GetRSTACKLength()/2.;
}

//____________________________________________________________________________
void IlcRSTACKv0::Init(void)
{
  // Just prints an information message
  
  Int_t i;

  if(IlcLog::GetGlobalDebugLevel()>0) {
    TString st ; 
    for(i=0;i<35;i++) 
      st += "*";
    Info("Init", "%s", st.Data()) ;  
    // Here the RSTACK initialisation code (if any!)
    
    IlcRSTACKGeometry * geom = GetGeometry() ; 

    if (geom!=0)  
      Info("Init", "IlcRSTACK%s: RSTACK geometry intialized for %s", Version().Data(), geom->GetName()) ;
    else
      Info("Init", "IlcRSTACK%s: RSTACK geometry initialization failed !", Version().Data()) ;       

    Info("Init", "%s", st.Data()) ;  
  }
}
