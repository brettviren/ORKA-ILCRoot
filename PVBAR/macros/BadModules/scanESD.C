#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TH3.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "IlcCDBManager.h"
#include "IlcGeomManager.h"
#include "IlcRunLoader.h"
#include "IlcESDEvent.h"
#include "IlcESDCaloCluster.h"
#include "IlcPVBARGeometry.h"
#endif
void scanESD(Int_t irun=7727)
{
  //This macro scans ESD and fills 3D histogram:
  //spectrum of clusters with center in each cristall
  //this histogram can be used e.g. in bad modules selection
  //Author: D.Peressounko Dmitri.Peressounko@cern.ch

  //Uncomment the following if misalignement should be applied
  IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  //   IlcCDBManager::Instance()->SetDefaultStorage("local://./");
  //   IlcCDBManager::Instance()->SetSpecificStorage("PVBAR/*","local:///data/prsnko/");
  IlcCDBManager::Instance()->SetRun(irun) ;

  //The final histo
  TH3D * hEsd = new TH3D("hEsd","Energy of all clusters",64,0.,64.,56,0.,56.,2000,0.,10.) ;

  //Reading/creation of Geometry
  IlcGeomManager::LoadGeometry("geometry.root");
  IlcPVBARGeometry *PVBARgeom = IlcPVBARGeometry::GetInstance("ORKA","") ;

  //Create list of ESD files
  TChain * chain = new TChain("esdTree") ;
  char filename[255] ;
  for(Int_t sec=1; sec<=17; sec++){
    sprintf(filename,"Seq_%d0/IlcESDs.root/esdTree",sec) ;
    chain->AddFile(filename);
  }
  
  IlcESDEvent *event = new IlcESDEvent();
  event->ReadFromTree(chain);
  
  for(Int_t iEvent=1; iEvent<chain->GetEntries(); iEvent++){
     if(iEvent%10000==0){
       printf("Event %d \n",iEvent) ;
     }
    chain->GetEvent(iEvent);
    Int_t multClu = event->GetNumberOfCaloClusters();
    for (Int_t i0=0; i0<multClu; i0++) {
      IlcESDCaloCluster * clu1 = event->GetCaloCluster(i0);
      Float_t xyz[3] = {0,0,0};
      clu1->GetPosition(xyz);   //Gloabal position in ILC system
      Int_t absId ;
      PVBARgeom->RelPosToAbsId(3,xyz[0],xyz[2],absId) ; //Here we assume that data taken with 3 module
      Int_t relid[4] ;
      PVBARgeom->AbsToRelNumbering(absId,relid) ;
      hEsd->Fill(relid[2]*1.-0.5, relid[3]*1.-0.5,clu1->E()) ;
    }   
  }
  
  TFile *ff = new TFile("scan.root","recreate") ;
  hEsd->Write() ;
  ff->Close() ;

}
