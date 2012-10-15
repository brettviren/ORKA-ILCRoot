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

/* $Id: IlcRSTACKPIDv0.cxx 53738 2011-12-20 16:49:27Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.15  2007/03/06 06:57:05  kharlov
 * DP:calculation of distance to CPV done in TSM
 *
 * Revision 1.14  2006/09/07 18:31:08  kharlov
 * Effective c++ corrections (T.Pocheptsov)
 *
 * Revision 1.13  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version v0 of the RSTACK particle identifier 
// Particle identification based on the 
//     - CPV information, 
//     - Preshower information (in MIXT or GPS2 geometries)
//     - shower width.
//
// CPV or Preshower clusters should be closer in RSTACK plane than fCpvEmcDistance (in cm).
// This parameter can be set by method SetCpvtoEmcDistanceCut(Float_t cut)  
//
// One can set desirable ID method by the function SetIdentificationMethod(option).
// Presently the following options can be used together or separately :
//     - "disp": use dispersion cut on shower width 
//               (width can be set by method SetDispersionCut(Float_t cut)
//     - "ell" : use cut on the axis of the ellipse, drawn around shower 
//       (this cut can be changed by SetShowerProfileCut(char* formula), 
//        where formula - any function of two variables f(lambda[0],lambda[1]).
//        Shower is considered as EM if f() > 0 )
// One can visualize current cuts calling method PlotDispersionCuts().    
//
// use case:
//  root [0] IlcRSTACKPIDv0 * p1 = new IlcRSTACKPIDv0("gilc.root")
//  Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//  root [1] p1->SetIdentificationMethod("disp ellipse")
//  root [2] p1->ExecuteTask()
//  root [3] IlcRSTACKPIDv0 * p2 = new IlcRSTACKPIDv0("gilc1.root","ts1")
//  Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//                // reading headers from file gilc1.root and TrackSegments 
//                // with title "ts1"
//  root [4] p2->SetRecParticlesBranch("rp1")
//                // set file name for the branch RecParticles
//  root [5] p2->ExecuteTask("deb all time")
//                // available options
//                // "deb" - prints # of reconstructed particles
//                // "deb all" -  prints # and list of RecParticles
//                // "time" - prints benchmarking results
//                  
//*-- Author: Yves Schutz (SUBATECH)  & Gines Martinez (SUBATECH) & 
//            Dmitri Peressounko (SUBATECH & Kurchatov Institute)
//            Completely redesined by Dmitri Peressounko, March 2001

// --- ROOT system ---
#include "TTree.h"
#include "TF2.h"
#include "TFormula.h"
#include "TCanvas.h"
#include "TClonesArray.h"

#include "TBenchmark.h"
// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcRSTACKPIDv0.h"
#include "IlcRSTACKEmcRecPoint.h"
#include "IlcRSTACKTrackSegment.h"
#include "IlcRSTACKRecParticle.h"
#include "IlcRSTACKGeometry.h"

ClassImp( IlcRSTACKPIDv0) 

//____________________________________________________________________________
IlcRSTACKPIDv0::IlcRSTACKPIDv0():
  fIDOptions("dis time"),
  fClusterizer(0),
  fTSMaker(0),
  fFormula(0),
  fDispersion(0.f),
  fCpvEmcDistance(0.f),
  fTimeGate(2.e-9f)
{ 
  // default ctor
}

//____________________________________________________________________________
IlcRSTACKPIDv0::IlcRSTACKPIDv0(IlcRSTACKGeometry *geom) : 
  IlcRSTACKPID(geom),
  fIDOptions("dis time"),
  fClusterizer(0),
  fTSMaker(0),
  fFormula(new TFormula("LambdaCuts","(x>1)*(x<2.5)*(y>0)*(y<x)")),
  fDispersion(2.f),
  fCpvEmcDistance(3.f),
  fTimeGate(2.e-9f)
{ 
  //ctor
}

//____________________________________________________________________________
IlcRSTACKPIDv0::IlcRSTACKPIDv0(const IlcRSTACKPIDv0 & rhs) :
  IlcRSTACKPID(rhs),
  fIDOptions(rhs.fIDOptions),
  fClusterizer(rhs.fClusterizer),
  fTSMaker(rhs.fTSMaker),
  fFormula(rhs.fFormula),
  fDispersion(rhs.fDispersion),
  fCpvEmcDistance(rhs.fCpvEmcDistance),
  fTimeGate(rhs.fTimeGate)
{
  //Copy ctor, the same as compiler-generated, possibly wrong if
  //someone implements dtor correctly.
}
  
//____________________________________________________________________________
IlcRSTACKPIDv0 & IlcRSTACKPIDv0::operator = (const IlcRSTACKPIDv0 & rhs)
{
  //Copy-assignment, emulates compiler generated, possibly wrong.
  IlcRSTACKPID::operator = (rhs);
  if (this != &rhs) {
    fIDOptions = rhs.fIDOptions;
    fClusterizer = rhs.fClusterizer;
    fTSMaker = rhs.fTSMaker;
    fFormula = rhs.fFormula;
    fDispersion = rhs.fDispersion;
    fCpvEmcDistance = rhs.fCpvEmcDistance;
    fTimeGate = rhs.fTimeGate;
  }
  else {
    IlcFatal("Self assignment!");
  }
  return *this;
}

//____________________________________________________________________________
IlcRSTACKPIDv0::~IlcRSTACKPIDv0()
{
  //Empty dtor, fFormula leaks 
}

//DP
////____________________________________________________________________________
//Float_t  IlcRSTACKPIDv0::GetDistance(IlcRSTACKEmcRecPoint * emc,IlcRSTACKRecPoint * cpv, Option_t *  Axis)const
//{
//  // Calculates the distance between the EMC RecPoint and the PPSD RecPoint
// 
//  const IlcRSTACKGeometry * geom = IlcRSTACKLoader::GetRSTACKGeometry() ; 
//  TVector3 vecEmc ;
//  TVector3 vecCpv ;
//  
//  emc->GetLocalPosition(vecEmc) ;
//  cpv->GetLocalPosition(vecCpv) ; 
//  if(emc->GetRSTACKMod() == cpv->GetRSTACKMod()){ 
//    
//    // Correct to difference in CPV and EMC position due to different distance to center.
//    // we assume, that particle moves from center
//    Float_t dCPV = geom->GetIPtoOuterCoverDistance();
//    Float_t dEMC = geom->GetIPtoCrystalSurface() ;
//    dEMC         = dEMC / dCPV ;
//    vecCpv = dEMC * vecCpv  - vecEmc ; 
//    if (Axis == "X") return vecCpv.X();
//    if (Axis == "Y") return vecCpv.Y();
//    if (Axis == "Z") return vecCpv.Z();
//    if (Axis == "R") return vecCpv.Mag();
//  } 
// 
//  return 100000000 ;
//}

//____________________________________________________________________________
void  IlcRSTACKPIDv0::TrackSegments2RecParticles(Option_t * option) 
{
  //Steering method

  if(strstr(option,"tim"))
    gBenchmark->Start("RSTACKPID");
  
  if(strstr(option,"print")) {
    Print() ; 
    return ; 
  }

  IlcInfo(Form("%d emc clusters, %d track segments", 
	       fEMCRecPoints->GetEntriesFast(), 
	       fTrackSegments->GetEntriesFast())) ;

  MakeRecParticles() ;
    
  if(strstr(option,"deb"))
    PrintRecParticles(option) ;

  if(strstr(option,"tim")){
    gBenchmark->Stop("RSTACKPID");
    IlcInfo(Form("took %f seconds for PID", 
		 gBenchmark->GetCpuTime("RSTACKPID"))); 
  }
  
}

//____________________________________________________________________________
void  IlcRSTACKPIDv0::MakeRecParticles()
{
  // Reconstructs the particles from the tracksegments

  fRecParticles->Clear();
  
  TIter next(fTrackSegments) ; 
  IlcRSTACKTrackSegment * ts ; 
  Int_t index = 0 ; 
  IlcRSTACKRecParticle * rp ; 
  
  Bool_t ellips = fIDOptions.Contains("ell",TString::kIgnoreCase ) ;
  Bool_t disp   = fIDOptions.Contains("dis",TString::kIgnoreCase ) ;
  Bool_t time   = fIDOptions.Contains("tim",TString::kIgnoreCase ) ;
  
  while ( (ts = (IlcRSTACKTrackSegment *)next()) ) {
    
    new( (*fRecParticles)[index] ) IlcRSTACKRecParticle() ;
    rp = (IlcRSTACKRecParticle *)fRecParticles->At(index) ; 
    rp->SetTrackSegment(index) ;
    rp->SetIndexInList(index) ;
    
    IlcRSTACKEmcRecPoint * emc = 0 ;
    if(ts->GetEmcIndex()>=0)
      emc = (IlcRSTACKEmcRecPoint *) fEMCRecPoints->At(ts->GetEmcIndex()) ;
    else
      IlcFatal(Form("ts->GetEmcIndex() return illegal index %d",ts->GetEmcIndex()));
    
    IlcRSTACKRecPoint    * cpv = 0 ;
    if(ts->GetCpvIndex()>=0)
      cpv = (IlcRSTACKRecPoint *)   fCPVRecPoints->At(ts->GetCpvIndex()) ;
    
    //set momentum and energy first
    Float_t    e = emc->GetEnergy() ;
    TVector3 dir = GetMomentumDirection(emc,cpv) ; 
    dir.SetMag(e) ;

    rp->SetMomentum(dir.X(),dir.Y(),dir.Z(),e) ;
    rp->SetCalcMass(0);

    //now set type (reconstructed) of the particle    
    Int_t showerprofile = 0;  // 0 narrow and 1 wide
    
    if(ellips){
      Float_t lambda[2] ;
      emc->GetElipsAxis(lambda) ;
      if(fFormula->Eval(lambda[0],lambda[1]) <= 0 )
	showerprofile = 1 ;  // not narrow
    }
    
    if(disp)
      if(emc->GetDispersion() > fDispersion )
	showerprofile = 1 ;  // not narrow
    
    Int_t slow = 0 ;
    if(time)
      if(emc->GetTime() > fTimeGate )
	slow = 0 ; 
        
    // Looking at the CPV detector
    Int_t cpvdetector= 0 ;  //1 hit and 0 no hit     
    if(cpv)
      if(ts->GetCpvDistance("R") < fCpvEmcDistance) 
	cpvdetector = 1 ;  
    
    Int_t type = showerprofile + 2 * slow  + 4 * cpvdetector ;
    rp->SetType(type) ; 
    rp->SetProductionVertex(0,0,0,0);
    rp->SetFirstMother(-1);
    rp->SetLastMother(-1);
    rp->SetFirstDaughter(-1);
    rp->SetLastDaughter(-1);
    rp->SetPolarisation(0,0,0);
    index++ ; 
  }
  
}

//____________________________________________________________________________
void  IlcRSTACKPIDv0:: Print(const Option_t *) const
{
  // Print the parameters used for the particle type identification
  TString message ; 
  message  = "=============== IlcRSTACKPIDv0 ================\n" ;
  message += "Making PID\n" ;
  message += "with parameters:\n"  ;
  message += "    Maximal EMC - CPV  distance (cm) %f\n" ;
  IlcInfo(Form( message.Data(),  
       fCpvEmcDistance ));

  if(fIDOptions.Contains("dis",TString::kIgnoreCase ))
    IlcInfo(Form("                    dispersion cut %f",  fDispersion )) ;
  if(fIDOptions.Contains("ell",TString::kIgnoreCase ))
    IlcInfo(Form("             Eliptic cuts function: %s",  
		 fFormula->GetTitle() )) ;
  if(fIDOptions.Contains("tim",TString::kIgnoreCase ))
    IlcInfo(Form("             Time Gate used: %f",  fTimeGate)) ;
}

//____________________________________________________________________________
void  IlcRSTACKPIDv0::SetShowerProfileCut(const char * formula)
{
  //set shape of the cut on the axis of ellipce, drown around shouer
  //shower considered "narrow" if Formula(lambda[0],lambda[1]) > 0.
  if(fFormula) 
    delete fFormula; 
  fFormula = new TFormula("Lambda Cut",formula) ;
}

//____________________________________________________________________________
void  IlcRSTACKPIDv0::PlotDispersionCuts()const
{
  // produces a plot of the dispersion cut
  TCanvas*  lambdas = new TCanvas("lambdas","Cuts on the ellipse axis",200,10,700,500);
 
  if(fIDOptions.Contains("ell",TString::kIgnoreCase ) ){
    TF2 * ell = new TF2("Elliptic Cuts",fFormula->GetName(),0,3,0,3) ;
    ell->SetMinimum(0.0000001) ;
    ell->SetMaximum(0.001) ;
    ell->SetLineStyle(1) ;
    ell->SetLineWidth(2) ;
    ell->Draw() ;
  }
  
  if( fIDOptions.Contains("dis",TString::kIgnoreCase ) ){
    TF2 * dsp = new TF2("dispersion","(y<x)*(x*x+y*y < [0]*[0])",0,3,0,3) ;
    dsp->SetParameter(0,fDispersion) ;
    dsp->SetMinimum(0.0000001) ;
    dsp->SetMaximum(0.001) ;
    dsp->SetLineStyle(1) ;
    dsp->SetLineColor(2) ;
    dsp->SetLineWidth(2) ;
    dsp->SetNpx(200) ;
    dsp->SetNpy(200) ;
    if(fIDOptions.Contains("ell",TString::kIgnoreCase ) )
      dsp->Draw("same") ;
    else
      dsp->Draw() ;
  }
  lambdas->Update();
}

//____________________________________________________________________________
TVector3 IlcRSTACKPIDv0::GetMomentumDirection(IlcRSTACKEmcRecPoint * emc, IlcRSTACKRecPoint * )const 
{ 
  // Calculates the momentum direction:
  //   1. if only a EMC RecPoint, direction is given by IP and this RecPoint
  //   2. if a EMC RecPoint and CPV RecPoint, direction is given by the line through the 2 recpoints 
  //  However because of the poor position resolution of PPSD the direction is always taken as if we were 
  //  in case 1.

  TVector3 dir(0,0,0) ; 
  
  TVector3 emcglobalpos ;
  TMatrix  dummy ;
  
  emc->GetGlobalPosition(emcglobalpos, dummy) ;
  
 
  // The following commented code becomes valid once the PPSD provides 
  // a reasonable position resolution, at least as good as EMC ! 
  //   TVector3 ppsdlglobalpos ;
  //   TVector3 ppsduglobalpos ;
  //   if( fPpsdLowRecPoint ){ // certainly a photon that has concerted
  //     fPpsdLowRecPoint->GetGlobalPosition(ppsdlglobalpos, mdummy) ; 
  //     dir = emcglobalpos -  ppsdlglobalpos ; 
  //     if( fPpsdUpRecPoint ){ // not looks like a charged       
  //        fPpsdUpRecPoint->GetGlobalPosition(ppsduglobalpos, mdummy) ; 
  //        dir = ( dir +  emcglobalpos -  ppsduglobalpos ) * 0.5 ; 
  //      }
  //   }
  //   else { // looks like a neutral
  //    dir = emcglobalpos ;  
  //  }
  
  dir = emcglobalpos ;  
  dir.SetZ( -dir.Z() ) ;   // why ?  
  dir.SetMag(1.) ;

  // One can not access MC information in the reconstruction!!
  // PLEASE FIT IT, EITHER BY TAKING 0,0,0 OR ACCESSING THE
  // VERTEX DIAMOND FROM CDB GRP FOLDER.
  //account correction to the position of IP
  //  Float_t xo,yo,zo ; //Coordinates of the origin
  //  gIlc->Generator()->GetOrigin(xo,yo,zo) ;
  //  TVector3 origin(xo,yo,zo);
  //  dir = dir - origin ;

  return dir ;  
}
//____________________________________________________________________________
void IlcRSTACKPIDv0::PrintRecParticles(Option_t * option)
{
  // Print table of reconstructed particles

  TString message ; 
  message = "Found %d RecParticles\n" ; 
  IlcInfo(Form(message.Data(), 
	       fRecParticles->GetEntriesFast() )) ;   

  if(strstr(option,"all")) {  // printing found TS
    IlcInfo("  PARTICLE   Index"  ) ; 
   
    Int_t index ;
    for (index = 0 ; index < fRecParticles->GetEntries() ; index++) {
      IlcRSTACKRecParticle * rp = (IlcRSTACKRecParticle * ) fRecParticles->At(index) ;       
      
      Text_t particle[100];
      switch(rp->GetType()) {
      case  IlcRSTACKFastRecParticle::kNEUTRALEMFAST:
	strncpy( particle, "NEUTRAL EM FAST",100);
	break;
      case  IlcRSTACKFastRecParticle::kNEUTRALHAFAST:
	strncpy(particle, "NEUTRAL HA FAST",100);
	break;
      case  IlcRSTACKFastRecParticle::kNEUTRALEMSLOW:
	strncpy(particle, "NEUTRAL EM SLOW",100);
	break ;
      case  IlcRSTACKFastRecParticle::kNEUTRALHASLOW: 
	strncpy(particle, "NEUTRAL HA SLOW",100);
	break ;
      case  IlcRSTACKFastRecParticle::kCHARGEDEMFAST:
	strncpy(particle, "CHARGED EM FAST",100);
	break ;
      case  IlcRSTACKFastRecParticle::kCHARGEDHAFAST:
	strncpy(particle, "CHARGED HA FAST",100);
	break ;	
      case  IlcRSTACKFastRecParticle::kCHARGEDEMSLOW:
	strncpy(particle, "CHARGEDEMSLOW",100);
	break ;
      case  IlcRSTACKFastRecParticle::kCHARGEDHASLOW:
	strncpy(particle, "CHARGED HA SLOW",100);
	break ; 
      }
      
      //    Int_t * primaries; 
      //    Int_t nprimaries;
      //    primaries = rp->GetPrimaries(nprimaries);
      
      IlcInfo(Form("          %s     %d",  
		   particle, rp->GetIndexInList())) ;
    }
  }  
}
