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


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  PHOS tender, recalibrate PHOS clusters                                   //
//  and do track matching                                                    //
//  Author : Dmitri Peressounko (RRC KI)                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TROOT.h"
#include "TH2.h"
#include "TFile.h"

#include <IlcLog.h>
#include <IlcESDEvent.h>
#include <IlcAnalysisManager.h>
#include <IlcTender.h>
#include <IlcCDBManager.h>
#include "IlcMagF.h"
#include "TGeoGlobalMagField.h"

#include "IlcESDCaloCluster.h"
#include "IlcPHOSTenderSupply.h"
#include "IlcPHOSCalibData.h"
#include "IlcPHOSGeometry.h"
#include "IlcPHOSEsdCluster.h"
#include "IlcOADBContainer.h"

ClassImp(IlcPHOSTenderSupply)

IlcPHOSTenderSupply::IlcPHOSTenderSupply() :
  IlcTenderSupply()
  ,fOCDBpass("local://OCDB")
  ,fNonlinearityVersion("Default")
  ,fPHOSGeo(0x0)
  ,fRunNumber(0)
  ,fRecoPass(-1)  //to be defined
  ,fUsePrivateBadMap(0)
  ,fUsePrivateCalib(0)
  ,fPHOSCalibData(0x0)
{
	//
	// default ctor
	//
   for(Int_t i=0;i<10;i++)fNonlinearityParams[i]=0. ;
   for(Int_t mod=0;mod<5;mod++)fPHOSBadMap[mod]=0x0 ;
}

//_____________________________________________________
IlcPHOSTenderSupply::IlcPHOSTenderSupply(const char *name, const IlcTender *tender) :
  IlcTenderSupply(name,tender)
  ,fOCDBpass("alien:///ilc/cern.ch/user/p/prsnko/PHOSrecalibrations/")
  ,fNonlinearityVersion("Default")
  ,fPHOSGeo(0x0)
  ,fRunNumber(0)
  ,fRecoPass(-1)  //to be defined
  ,fUsePrivateBadMap(0)
  ,fUsePrivateCalib(0)
  ,fPHOSCalibData(0x0)
{
	//
	// named ctor
	//
   for(Int_t i=0;i<10;i++)fNonlinearityParams[i]=0. ;
   for(Int_t mod=0;mod<5;mod++)fPHOSBadMap[mod]=0x0 ;
}

//_____________________________________________________
IlcPHOSTenderSupply::~IlcPHOSTenderSupply()
{
  //Destructor
  if(fPHOSCalibData)
    delete fPHOSCalibData;
  fPHOSCalibData=0x0 ;
}

//_____________________________________________________
void IlcPHOSTenderSupply::InitTender()
{
  //
  // Initialise PHOS tender
  //
  IlcESDEvent *event=fTender->GetEvent();
  if (!event) return;
	      
  fRunNumber=event->GetRunNumber();	      
  if(fRecoPass<0){ //not defined yet
    // read if from filename.
    IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
    TTree * t = mgr->GetTree();
    if(t){  
      TFile * f = t->GetCurrentFile() ;
      if(f){  
        TString fname(f->GetName());
        if(fname.Contains("pass1"))
	   fRecoPass=1;
        else 
	  if(fname.Contains("pass2"))
	   fRecoPass=2;
          else 
	    if(fname.Contains("pass3")) 
  	      fRecoPass=3;
      }
    }
    if(fRecoPass<0){
      IlcError("Can not find pass number from file name, set it manually");
    }
  }
   
  //Init geometry 
  if(!fPHOSGeo){
    IlcOADBContainer geomContainer("phosGeo");
    geomContainer.InitFromFile("$ILC_ROOT/OADB/PHOS/PHOSGeometry.root","PHOSRotationMatrixes");
    TObjArray *matrixes = (TObjArray*)geomContainer.GetObject(fRunNumber,"PHOSRotationMatrixes");
    fPHOSGeo =  IlcPHOSGeometry::GetInstance("IHEP") ;
    for(Int_t mod=0; mod<5; mod++) {
      if(!matrixes->At(mod)) continue;
      fPHOSGeo->SetMisalMatrix(((TGeoHMatrix*)matrixes->At(mod)),mod) ;
      printf(".........Adding Matrix(%d), geo=%p\n",mod,fPHOSGeo) ;
    }
  }
  
  //Init Bad channels map
  if(!fUsePrivateBadMap){
   IlcOADBContainer badmapContainer(Form("phosBadMap"));
    badmapContainer.InitFromFile("$ILC_ROOT/OADB/PHOS/PHOSBadMaps.root","phosBadMap");
    TObjArray *maps = (TObjArray*)badmapContainer.GetObject(fRunNumber,"phosBadMap");
    if(!maps){
      IlcError(Form("Can not read Bad map for run %d. \n You may choose to use your map with ForceUsingBadMap()\n",fRunNumber)) ;    
    }
    else{
      IlcInfo(Form("Setting PHOS bad map with name %s \n",maps->GetName())) ;
      for(Int_t mod=0; mod<5;mod++){
        if(fPHOSBadMap[mod]) 
          delete fPHOSBadMap[mod] ;
        TH2I * h = (TH2I*)maps->At(mod) ;      
	if(h)
          fPHOSBadMap[mod]=new TH2I(*h) ;
      }
    }    
  } 

  if(!fUsePrivateCalib){
    //Init recalibration
    //Check the pass1-pass2-pass3 reconstruction
    IlcOADBContainer calibContainer("phosRecalibration");
    calibContainer.InitFromFile("$ILC_ROOT/OADB/PHOS/PHOSCalibrations.root","phosRecalibration");
    TObjArray *recalib = (TObjArray*)calibContainer.GetObject(fRunNumber,"PHOSRecalibration");
    if(!recalib){
      IlcFatal(Form("Can not read calibrations for run %d\n. You may choose your specific calibration with ForceUsingCalibration()\n",fRunNumber)) ;
    }
    else{
      fPHOSCalibData = (IlcPHOSCalibData*)recalib->At(fRecoPass-1) ;
      if(!fPHOSCalibData) {
        IlcFatal(Form("Can not find calibration for run %d, pass %d \n",fRunNumber, fRecoPass)) ;
      }
    }
  }
  
}

//_____________________________________________________
void IlcPHOSTenderSupply::ProcessEvent()
{
  //Choose PHOS clusters and recalibrate them
  //that it recalculate energy, position and distance 
  //to closest track extrapolation	

  IlcESDEvent *event=fTender->GetEvent();
  if (!event) return;
	      
  fRunNumber=event->GetRunNumber();	      

  if(!fPHOSCalibData || fTender->RunChanged()){
    InitTender();
    
  }


  const IlcESDVertex *esdVertex = event->GetPrimaryVertex();
  IlcESDCaloCells * cells = event->GetPHOSCells() ;
  TVector3 vertex(esdVertex->GetX(),esdVertex->GetY(),esdVertex->GetZ());
  if(vertex.Mag()>99.) //vertex not defined?
    vertex.SetXYZ(0.,0.,0.) ;

  //For re-calibration
  const Double_t logWeight=4.5 ;  

  Int_t multClust = event->GetNumberOfCaloClusters();
  for (Int_t i=0; i<multClust; i++) {
    IlcESDCaloCluster *clu = event->GetCaloCluster(i);
    if ( !clu->IsPHOS()) continue;
    
    Float_t  position[3];
    clu->GetPosition(position);
    TVector3 global(position) ;
    Int_t relId[4] ;
    fPHOSGeo->GlobalPos2RelId(global,relId) ;
    Int_t mod  = relId[0] ;
    Int_t cellX = relId[2];
    Int_t cellZ = relId[3] ;
    if ( !IsGoodChannel(mod,cellX,cellZ) ) {
      clu->SetE(0.) ;
      continue ;
    }  
   //Apply re-Calibreation
    IlcPHOSEsdCluster cluPHOS1(*clu);
    cluPHOS1.Recalibrate(fPHOSCalibData,cells); // modify the cell energies
    cluPHOS1.EvalAll(logWeight,vertex);         // recalculate the cluster parameters
    cluPHOS1.SetE(CorrectNonlinearity(cluPHOS1.E()));// Users's nonlinearity
    
    Float_t  xyz[3];
    cluPHOS1.GetPosition(xyz);
    clu->SetPosition(xyz);                       //rec.point position in MARS
    clu->SetE(cluPHOS1.E());                           //total or core particle energy
    clu->SetDispersion(cluPHOS1.GetDispersion());  //cluster dispersion
    //    ec->SetPID(rp->GetPID()) ;            //array of particle identification
    clu->SetM02(cluPHOS1.GetM02()) ;               //second moment M2x
    clu->SetM20(cluPHOS1.GetM20()) ;               //second moment M2z
    Double_t dx=0.,dz=0. ;
    fPHOSGeo->GlobalPos2RelId(global,relId) ;
    TVector3 locPos;
    fPHOSGeo->Global2Local(locPos,global,mod) ;

    Double_t pttrack=0.;
    Int_t charge=0;
    FindTrackMatching(mod,&locPos,dx,dz,pttrack,charge) ;
    Double_t r=TestCPV(dx, dz, pttrack,charge) ;
    clu->SetTrackDistance(dx,dz); 
    
    clu->SetEmcCpvDistance(r);    
    clu->SetChi2(TestLambda(clu->E(),clu->GetM20(),clu->GetM02()));                     //not yet implemented
    clu->SetTOF(cluPHOS1.GetTOF());       

  }


}
//___________________________________________________________________________________________________
void IlcPHOSTenderSupply::FindTrackMatching(Int_t mod,TVector3 *locpos,
					    Double_t &dx, Double_t &dz,
					    Double_t &pt,Int_t &charge){
  //Find track with closest extrapolation to cluster
  
  IlcESDEvent *event= fTender->GetEvent();
  Double_t  magF = event->GetMagneticField();
  Double_t magSign = 1.0;
  if(magF<0)magSign = -1.0;
  
  if (!TGeoGlobalMagField::Instance()->GetField()) {
    IlcMagF* field = new IlcMagF("Maps","Maps", magSign, magSign, IlcMagF::k5kG);
    TGeoGlobalMagField::Instance()->SetField(field);
  }

  // *** Start the matching
  Int_t nt=event->GetNumberOfTracks();
  //Calculate actual distance to PHOS module
  TVector3 globaPos ;
  fPHOSGeo->Local2Global(mod, 0.,0., globaPos) ;
  const Double_t rPHOS = globaPos.Pt() ; //Distance to center of  PHOS module
  const Double_t kYmax = 72.+10. ; //Size of the module (with some reserve) in phi direction
  const Double_t kZmax = 64.+10. ; //Size of the module (with some reserve) in z direction
  const Double_t kAlpha0=330./180.*TMath::Pi() ; //First PHOS module angular direction
  const Double_t kAlpha= 20./180.*TMath::Pi() ; //PHOS module angular size
  Double_t minDistance = 1.e6;


  Double_t gposTrack[3] ; 

  Double_t bz = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->SolenoidField();
  bz = TMath::Sign(0.5*kAlmost0Field,bz) + bz;

  Double_t b[3]; 
  for (Int_t i=0; i<nt; i++) {
    IlcESDtrack *esdTrack=event->GetTrack(i);

    // Skip the tracks having "wrong" status (has to be checked/tuned)
    ULong_t status = esdTrack->GetStatus();
    if ((status & IlcESDtrack::kTPCout)   == 0) continue;
    //     if ((status & IlcESDtrack::kTRDout)   == 0) continue;
    //     if ((status & IlcESDtrack::kTRDrefit) == 1) continue;
    
    //Continue extrapolation from TPC outer surface
    const IlcExternalTrackParam *outerParam=esdTrack->GetOuterParam();
    if (!outerParam) continue;
    IlcExternalTrackParam t(*outerParam);
    
    t.GetBxByBz(b) ;
    //Direction to the current PHOS module
    Double_t phiMod=kAlpha0-kAlpha*mod ;
    if(!t.Rotate(phiMod))
      continue ;
    
    Double_t y;                       // Some tracks do not reach the PHOS
    if (!t.GetYAt(rPHOS,bz,y)) continue; //    because of the bending
    
    Double_t z; 
    if(!t.GetZAt(rPHOS,bz,z))
      continue ;
    if (TMath::Abs(z) > kZmax) 
      continue; // Some tracks miss the PHOS in Z
    if(TMath::Abs(y) < kYmax){
      t.PropagateToBxByBz(rPHOS,b);        // Propagate to the matching module
      //t.CorrectForMaterial(...); // Correct for the TOF material, if needed
      t.GetXYZ(gposTrack) ;
      TVector3 globalPositionTr(gposTrack) ;
      TVector3 localPositionTr ;
      fPHOSGeo->Global2Local(localPositionTr,globalPositionTr,mod) ;
      Double_t ddx = locpos->X()-localPositionTr.X();
      Double_t ddz = locpos->Z()-localPositionTr.Z();
      Double_t d2 = ddx*ddx + ddz*ddz;
      if(d2 < minDistance) {
	dx = ddx ;
	dz = ddz ;
	minDistance=d2 ;
	pt=esdTrack->Pt() ;
	charge=esdTrack->Charge() ;
      }
    }
  } //Scanned all tracks
  
}
//____________________________________________________________
Float_t IlcPHOSTenderSupply::CorrectNonlinearity(Float_t en){

  //For backward compatibility, if no RecoParameters found
  if(fNonlinearityVersion=="Default"){
    return 0.0241+1.0504*en+0.000249*en*en ;
  }

  if(fNonlinearityVersion=="NoCorrection"){
    return en ;
  }
  if(fNonlinearityVersion=="Gustavo2005"){
    return fNonlinearityParams[0]+fNonlinearityParams[1]*en + fNonlinearityParams[2]*en*en ;
  }
  if(fNonlinearityVersion=="Henrik2010"){
    return en*(fNonlinearityParams[0]+fNonlinearityParams[1]*TMath::Exp(-en*fNonlinearityParams[2]))*(1.+fNonlinearityParams[3]*TMath::Exp(-en*fNonlinearityParams[4]))*(1.+fNonlinearityParams[6]/(en*en+fNonlinearityParams[5])) ;
  }

  return en ;
}
//_____________________________________________________________________________
Double_t IlcPHOSTenderSupply::TestLambda(Double_t pt,Double_t l1,Double_t l2){
  
  Double_t l2Mean  = 1.53126+9.50835e+06/(1.+1.08728e+07*pt+1.73420e+06*pt*pt) ;
  Double_t l1Mean  = 1.12365+0.123770*TMath::Exp(-pt*0.246551)+5.30000e-03*pt ;
  Double_t l2Sigma = 6.48260e-02+7.60261e+10/(1.+1.53012e+11*pt+5.01265e+05*pt*pt)+9.00000e-03*pt;
  Double_t l1Sigma = 4.44719e-04+6.99839e-01/(1.+1.22497e+00*pt+6.78604e-07*pt*pt)+9.00000e-03*pt;
  Double_t c=-0.35-0.550*TMath::Exp(-0.390730*pt) ;
  Double_t R2=0.5*(l1-l1Mean)*(l1-l1Mean)/l1Sigma/l1Sigma + 
              0.5*(l2-l2Mean)*(l2-l2Mean)/l2Sigma/l2Sigma +
              0.5*c*(l1-l1Mean)*(l2-l2Mean)/l1Sigma/l2Sigma ;
  return (R2<2.5*2.5) ;
  
}
//____________________________________________________________________________
Double_t IlcPHOSTenderSupply::TestCPV(Double_t dx, Double_t dz, Double_t pt, Int_t charge){
  //Parameterization of LHC10h period
  //_true if neutral_
  
  Double_t meanX=0;
  Double_t meanZ=0.;
  Double_t sx=TMath::Min(5.4,2.59719e+02*TMath::Exp(-pt/1.02053e-01)+
              6.58365e-01*5.91917e-01*5.91917e-01/((pt-9.61306e-01)*(pt-9.61306e-01)+5.91917e-01*5.91917e-01)+1.59219);
  Double_t sz=TMath::Min(2.75,4.90341e+02*1.91456e-02*1.91456e-02/(pt*pt+1.91456e-02*1.91456e-02)+1.60) ;
  IlcESDEvent *event=fTender->GetEvent();
  Double_t mf = event->GetMagneticField(); //Positive for ++ and negative for --

  if(mf<0.){ //field --
    meanZ = -0.468318 ;
    if(charge>0)
      meanX=TMath::Min(7.3, 3.89994*1.20679*1.20679/(pt*pt+1.20679*1.20679)+0.249029+2.49088e+07*TMath::Exp(-pt*3.33650e+01)) ;
    else
      meanX=-TMath::Min(7.7,3.86040*0.912499*0.912499/(pt*pt+0.912499*0.912499)+1.23114+4.48277e+05*TMath::Exp(-pt*2.57070e+01)) ;
  }
  else{ //Field ++
    meanZ= -0.468318;
    if(charge>0)
      meanX=-TMath::Min(8.0,3.86040*1.31357*1.31357/(pt*pt+1.31357*1.31357)+0.880579+7.56199e+06*TMath::Exp(-pt*3.08451e+01)) ;
    else
      meanX= TMath::Min(6.85, 3.89994*1.16240*1.16240/(pt*pt+1.16240*1.16240)-0.120787+2.20275e+05*TMath::Exp(-pt*2.40913e+01)) ;     
  }

  Double_t rz=(dz-meanZ)/sz ;
  Double_t rx=(dx-meanX)/sx ;
  return TMath::Sqrt(rx*rx+rz*rz) ;
}

//________________________________________________________________________
Bool_t IlcPHOSTenderSupply::IsGoodChannel(Int_t mod, Int_t ix, Int_t iz)
{
  //Check if this channel belogs to the good ones
  
  if(mod>4 || mod<1){
//    IlcError(Form("No bad map for PHOS module %d ",mod)) ;
    return kTRUE ;
  }
  if(!fPHOSBadMap[mod]){
//     IlcError(Form("No Bad map for PHOS module %d",mod)) ;
     return kTRUE ;
  }
  if(fPHOSBadMap[mod]->GetBinContent(ix,iz)>0)
    return kFALSE ;
  else
    return kTRUE ;
}
//________________________________________________________________________
void IlcPHOSTenderSupply::ForceUsingBadMap(const char * filename){
  //Read TH2I histograms with bad maps from local or alien file 
  TFile * fbm = TFile::Open(filename) ;
  if(!fbm || !fbm->IsOpen()){
    IlcError(Form("Can not open BadMaps file %s \n",filename)) ;
    return ;
  }
  gROOT->cd() ;
  char key[55] ;
  for(Int_t mod=1;mod<4; mod++){
    snprintf(key,55,"PHOS_BadMap_mod%d",mod) ;
    TH2I * h = (TH2I*)fbm->Get(key) ;
    if(h)
       fPHOSBadMap[mod] = new TH2I(*h) ;
  }
  fbm->Close() ;
  fUsePrivateBadMap=kTRUE ;
}
//________________________________________________________________________
void IlcPHOSTenderSupply::ForceUsingCalibration(const char * filename){
  //Read PHOS recalibration parameters from the file.
  //We assume that file contains single entry: IlcPHOSCalibData
  TFile * fc = TFile::Open(filename) ;
  if(!fc || !fc->IsOpen()){
    IlcFatal(Form("Can not open Calibration file %s \n",filename)) ;
    return ;
  }
  fPHOSCalibData = (IlcPHOSCalibData*)fc->Get("PHOSCalibration") ;
  fc->Close() ;
  fUsePrivateCalib=kTRUE; 
}





