/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
// Implementation of the interface for THBTprocessor
// Author: Piotr Krzysztof Skowronski <Piotr.Skowronski@cern.ch>
//////////////////////////////////////////////////////////////////////////////////
//Wrapper class for "hbt processor" after burner
//The origibal code is written in fortran by Lanny Ray
//and is put in the directory $ILC_ROOT/HBTprocessor
//Detailed description is on the top of the file hbt_event_processor.f
//
//This class can be used ONLY with IlcGenCocktailAfterBurner wrapper generator
//i.e. (in Config.C)
// ....
// IlcGenCocktailAfterBurner = gener = new IlcGenCocktailAfterBurner();
// gener->SetPhiRange(0, 360); //Set global parameters
// gener->SetThetaRange(35., 145.); 
// IlcGenHIJINGpara *hijing = new IlcGenHIJINGpara(10000); //Add some generator
// hijing->SetMomentumRange(0, 999);   
// gener->AddGenerator(hijing,"HIJING PARAMETRIZATION",1); 
//
// IlcGenHBTprocessor *hbtp = new IlcGenHBTprocessor(); //create object
// hbtp->SetRefControl(2); //set parameters
// hbtp->SetSwitch1D(1);
// hbtp->SetR0(6);//fm - radius
// hbtp->SetLambda(0.7);//chaocity parameter
// gener->AddAfterBurner(hbtp,"HBT PROCESSOR",1); //add to the main generator
// 
// gener->Init();
//
//CAUTIONS: 
//         A)  HBT PROCESSOR NEEDS MORE THAN ONE EVENT TO WORK
//             AS MORE AS IT BETTER WORKS
//         B)  IT IS ABLE TO "ADD" CORRELATIONS ONLY UP TO TWO PARTICLE TYPES AT ONES
//
//
// Artificial particle dennsity enhancment feature
// HBT Processor is unable to process correctly low multiplicity particles
// even if the high statistics (more events) is supplied.
// For that reason was implemented artificial multiplicity enhancement 
// - see also comments in HBT Processor source file (HBTP/hbt_event_processor.f)
//   or web page (http://ilcsoft.cern.ch/people/skowron/hbtprocessor/hbteventprocessor.html) 
//   concerning trk_accep or  SetTrackRejectionFactor method in this class
// Fortran is cheated by masking several events a single one. Number is defined by fEventMerge
// variable. In case it is equal to 1, there is no masking and the feature is off.
// But, for example if it is 5, and we have 100 events, number of events passed to fortran is 100/5=20
// When fortran asks about multiplcity of event, let sey, 1 - multiplicity of events 5 to 9 is summed up 
// and returned. 
//
//
//////////////////////////////////////////////////////////////////////////////////

// 11.11.2001 Piotr Skowronski
// Setting seed (date) in RNG in the constructor

// 09.10.2001 Piotr Skowronski
// 
// Theta - Eta cohernecy facilities added:
//    IlcGenerator::SetThetaRange method overriden
//    Static methods added
//    EtaToTheta
//    ThetaToEta 
//    DegreesToRadians
//    RadiansToDegrees
//
// Class description comments put on proper place

// 27.09.2001 Piotr Skowronski
// removing of redefinition of defaults velues 
// in method's implementation. 
//  
// 

#include "IlcGenHBTprocessor.h"

#include <TParticle.h>
#include "THBTprocessor.h"

#include "IlcStack.h"
#include "IlcMC.h"
#include "IlcRun.h"
#include "IlcGenCocktailAfterBurner.h"
#include "IlcLog.h"



ClassImp(IlcGenHBTprocessor)

Int_t IlcGenHBTprocessor::fgDebug = 0;
static TRandom* gIlcRandom;//RNG to be used by the fortran code

IlcGenCocktailAfterBurner*  GetGenerator();
/*******************************************************************/

IlcGenHBTprocessor::IlcGenHBTprocessor(): 
  IlcGenerator(), 
  fHBTprocessor(0x0),
  fHbtPStatCodes(0x0),
  fNPDGCodes(0),
  fTrackRejectionFactor(0),
  fReferenceControl(0),
  fPrintFull(0),
  fPrintSectorData(0),
  fNPidTypes(0),
  fNevents(0),
  fSwitch1d(0),
  fSwitch3d(0),
  fSwitchType(0),
  fSwitchCoherence(0),
  fSwitchCoulomb(0),
  fSwitchFermiBose(0),
  fEventLineCounter(0),
  fMaxit(0),
  fIrand(0),
  fLambda(0),
  fR1d(0),
  fRside(0),
  fRout(0),
  fRlong(0),
  fRperp(0),
  fRparallel(0),
  fR0(0),
  fQ0(0),
  fDeltap(0),
  fDelchi(0),
  fNPtBins(0),
  fNPhiBins(0),
  fNEtaBins(0),
  fN1dFine(0),
  fN1dCoarse(0),
  fN1dTotal(0),
  fN3dFine(0),
  fN3dCoarse(0),
  fN3dTotal(0),
  fN3dFineProject(0),
  fNPxBins(0),
  fNPyBins(0),
  fNPzBins(0),
  fNSectors(0),
  fPtBinSize(0),
  fPhiBinSize(0),
  fEtaBinSize(0),
  fEtaMin(0),
  fEtaMax(0),
  fBinsize1dFine(0),
  fBinsize1dCoarse(0),
  fQmid1d(0),
  fQmax1d(0),
  fBinsize3dFine(0),
  fBinsize3dCoarse(0),
  fQmid3d(0),
  fQmax3d(0),
  fPxMin(0),
  fPxMax(0),
  fDelpx(0),
  fPyMin(0),
  fPyMax(0),
  fDelpy(0),
  fPzMin(0),
  fPzMax(0),
  fDelpz(0),
  fEventMerge(1),
  fActiveStack(0)
{
  //
  // Standard constructor
  // Sets default veues of all parameters

  SetName("IlcGenHBTprocessor");
  SetTitle("IlcGenHBTprocessor");
  
  gIlcRandom = fRandom;
  fHBTprocessor = new THBTprocessor();

  fNPDGCodes = 0;
  DefineParticles();

  SetTrackRejectionFactor();
  SetRefControl();
  SetPIDs();
  SetNPIDtypes();
  SetDeltap();
  SetMaxIterations();
  SetDelChi();
  SetIRand();
  SetLambda();
  SetR1d() ;
  SetRSide();
  SetROut() ;
  SetRLong() ;
  SetRPerp();
  SetRParallel();
  SetR0();
  SetQ0();
  SetSwitch1D();
  SetSwitch3D();
  SetSwitchType();
  SetSwitchCoherence();
  SetSwitchCoulomb();
  SetSwitchFermiBose();
  //SetMomentumRange();
  SetPtRange();
  SetPxRange();
  SetPyRange(); 
  SetPzRange(); 
  SetPhiRange(); 
  SetEtaRange();  
  SetNPtBins();  
  SetNPhiBins();  
  SetNEtaBins();
  SetNPxBins(); 
  SetNPyBins();
  SetNPzBins(); 
  SetNBins1DFineMesh();
  SetBinSize1DFineMesh();
  SetNBins1DCoarseMesh();
  SetBinSize1DCoarseMesh();
  SetNBins3DFineMesh();
  SetBinSize3DFineMesh();
  SetNBins3DCoarseMesh();
  SetBinSize3DCoarseMesh();
  SetNBins3DFineProjectMesh();
}

/*******************************************************************/


/*******************************************************************/

IlcGenHBTprocessor::~IlcGenHBTprocessor()
{
//destructor
  CleanStatusCodes();
  if (fHBTprocessor) delete fHBTprocessor; //delete generator
  
}

/*******************************************************************/

void IlcGenHBTprocessor::InitStatusCodes()
{
 //creates and inits status codes array to zero
  IlcGenCocktailAfterBurner *cab = GetGenerator();

  if(!cab) {
    Fatal("InitStatusCodes()","Can not find IlcGenCocktailAfterBurner generator");
    return;
  }

  Int_t nev = cab->GetNumberOfEvents();

  fHbtPStatCodes = new Int_t* [nev];
  for( Int_t i =0; i<nev;i++)
  { 
    Int_t nprim = cab->GetStack(i)->GetNprimary();
    fHbtPStatCodes[i] = new Int_t[nprim];
    for (Int_t k =0 ;k<nprim;k++)
      fHbtPStatCodes[i][k] =0;
    
  }
  
}
/*******************************************************************/

void IlcGenHBTprocessor::CleanStatusCodes()
{
 //Cleans up status codes
  if (fHbtPStatCodes)
  {
    for (Int_t i =0; i<GetGenerator()->GetNumberOfEvents(); i++)
      delete [] fHbtPStatCodes[i];  
    delete [] fHbtPStatCodes;
    fHbtPStatCodes = 0;
  }

}
/*******************************************************************/

void IlcGenHBTprocessor::Init()
  {  
  //sets up parameters in generator
   
   THBTprocessor *thbtp = fHBTprocessor;
   

   thbtp->SetTrackRejectionFactor(fTrackRejectionFactor);
   thbtp->SetRefControl(fReferenceControl);
   
   if ((fPid[0] == fPid[1]) || (fPid[0] == 0) || (fPid[1] == 0))
    {
       if (fPid[0] == 0)
         thbtp->SetPIDs(IdFromPDG(fPid[1]) ,0);
       else
         thbtp->SetPIDs(IdFromPDG(fPid[0]) ,0);
       thbtp->SetNPIDtypes(1);
       
       if (fSwitchType !=1)
          Warning("IlcGenHBTprocessor::Init","\nThere is only one particle type set,\n\
                   and Switch_Type differnt then 1,\n which does not make sense.\n\
                   Setting it to 1.\n");
                   
       SetSwitchType(1);
    }
   else
    {
       thbtp->SetPIDs(IdFromPDG(fPid[0]) ,IdFromPDG(fPid[1]));
       SetNPIDtypes(2);
       thbtp->SetSwitchType(fSwitchType); 
    }
   
   thbtp->SetMaxIterations(fMaxit);
   thbtp->SetDelChi(fDelchi);
   thbtp->SetIRand(fIrand);
   thbtp->SetLambda(fLambda);
   thbtp->SetR1d(fR1d);
   thbtp->SetRSide(fRside);
   thbtp->SetROut(fRout);
   thbtp->SetRLong(fRlong);
   thbtp->SetRPerp(fRperp);
   thbtp->SetRParallel(fRparallel);
   thbtp->SetR0(fR0);
   thbtp->SetQ0(fQ0);
   thbtp->SetSwitch1D(fSwitch1d);
   thbtp->SetSwitch3D(fSwitch3d);
   thbtp->SetSwitchType(fSwitchType);
   thbtp->SetSwitchCoherence(fSwitchCoherence);
   thbtp->SetSwitchCoulomb(fSwitchCoulomb);
   thbtp->SetSwitchFermiBose(fSwitchFermiBose);
   thbtp->SetPtRange(fPtMin,fPtMax);
   thbtp->SetPxRange(fPxMin,fPxMax);
   thbtp->SetPyRange(fPyMin,fPyMax);
   thbtp->SetPzRange(fPzMin,fPzMax);
   thbtp->SetPhiRange(fPhiMin*180./((Float_t)TMath::Pi())+180.0, //casting is because if fPhiMin = 180.0 then
                      fPhiMax*180./((Float_t)TMath::Pi())+180.0);//TMath::Pi() != TMath::Pi()*fPhiMin/180.0,
   thbtp->SetEtaRange(fEtaMin,fEtaMax);
   thbtp->SetNPtBins(fNPtBins);
   thbtp->SetNPhiBins(fNPhiBins);
   thbtp->SetNEtaBins(fNEtaBins);
   thbtp->SetNPxBins(fNPxBins);
   thbtp->SetNPyBins(fNPyBins);
   thbtp->SetNPzBins(fNPzBins);
   thbtp->SetNBins1DFineMesh(fN1dFine);
   thbtp->SetBinSize1DFineMesh(fBinsize1dFine);
   thbtp->SetNBins1DCoarseMesh(fN1dCoarse);
   thbtp->SetBinSize1DCoarseMesh(fBinsize1dCoarse);
   thbtp->SetNBins3DFineMesh(fN3dFine);
   thbtp->SetBinSize3DFineMesh(fBinsize3dFine);
   thbtp->SetNBins3DCoarseMesh(fN3dCoarse);
   thbtp->SetBinSize3DCoarseMesh(fBinsize3dCoarse);
   thbtp->SetNBins3DFineProjectMesh(fN3dFineProject);
   
   thbtp->SetPrintFull(fPrintFull);
       
 }
/*******************************************************************/
  
void IlcGenHBTprocessor::Generate()
 {
 //starts processig 
   IlcGenCocktailAfterBurner* cab = GetGenerator();
   if (cab == 0x0)
    {
      Fatal("Generate()","IlcGenHBTprocessor needs IlcGenCocktailAfterBurner to be main generator");
      return;
    }
   if (cab->GetNumberOfEvents() <2)
    {
      Fatal("Generate()",
            "HBT Processor needs more than 1 event to work properly,\
             but there is only %d set", cab->GetNumberOfEvents());
    }
  
  
   fHBTprocessor->Initialize(); //reset all fields of common blocks 
                                   //in case there are many HBT processors
                  	               //run one after one (i.e. in AliCocktailAfterBurner)
                                   //between Init() called and Generate there might 
   Init();                         //be different instance running - be sure that we have our settings
   
   InitStatusCodes(); //Init status codes
   
   fHBTprocessor->GenerateEvent(); //Generates event
   
   CleanStatusCodes(); //Clean Status codes - thet are not needed anymore
 }
 
/*******************************************************************/


/*******************************************************************/
void IlcGenHBTprocessor::GetParticles(TClonesArray * particles) const
 {
 //practically dumm
   if (!particles)
    {
      Error("GetParticles","Particles has to be initialized");
      return;
    } 
   fHBTprocessor->ImportParticles(particles);
 }

/*******************************************************************/

Int_t IlcGenHBTprocessor::GetHbtPStatusCode(Int_t part) const
{
//returns the status code of the given particle in the active event
//see SetActiveEvent in the bottom of IlcGenHBTprocessor.cxx
//and in AliCocktailAfterBurner
 Int_t ev, idx;
 GetTrackEventIndex(part,ev,idx);
 if ( (ev<0) || (idx<0) )
  {
    Error("GetHbtPStatusCode","GetTrackEventIndex returned error");
    return 0;
  }
 return fHbtPStatCodes[ev][idx];
  
}

/*******************************************************************/
void  IlcGenHBTprocessor::SetHbtPStatusCode(Int_t hbtstatcode, Int_t part)
{
 //Sets the given status code to given particle number (part) in the active event
 Int_t ev, idx;
 GetTrackEventIndex(part,ev,idx);
 if ( (ev<0) || (idx<0) )
  {
    Error("GetHbtPStatusCode","GetTrackEventIndex returned error");
    return;
  }
 else fHbtPStatCodes[ev][idx] = hbtstatcode;
}

/*******************************************************************/

void IlcGenHBTprocessor::SetTrackRejectionFactor(Float_t trf) //def 1.0
  {
   //Sets the Track Rejection Factor
   //variates in range 0.0 <-> 1.0
   //Describes the factor of particles rejected from the output.
   //Used only in case of low muliplicity particles e.g. lambdas.
   //Processor generates addisional particles and builds the 
   //correletions on such a statistics.
   //At the end these particels are left in the event according 
   //to this factor: 1==all particles are left
   //                0==all are removed 
   
    fTrackRejectionFactor=trf;
    fHBTprocessor->SetTrackRejectionFactor(trf);
  }
/*******************************************************************/

void IlcGenHBTprocessor::SetRefControl(Int_t rc) //default 2
 {
 //Sets the Refernce Control Switch
 //switch wether read reference histograms from file =1
 //              compute from input events =2 - default
   fReferenceControl=rc;
   fHBTprocessor->SetRefControl(rc);
 }
/*******************************************************************/

void IlcGenHBTprocessor::SetPIDs(Int_t pid1,Int_t pid2)
  {
   //default pi+ pi-
   //Sets PDG Codes of particles to be processed, default \\Pi^{+} and \\Pi^{-}
   //This method accepts PDG codes which is
   //in opposite to THBBProcessor which accepts GEANT PID
    if ( (pid1 == 0) && (pid2 == 0) )
    {
      Error("IlcGenHBTprocessor::SetPIDs","Sensless Particle Types setting: 0 0, Ignoring\n");
    }
    
    fPid[0]=pid1;
    fPid[1]=pid2;
    
    if(pid1 == pid2)
       {
        fHBTprocessor->SetPIDs(IdFromPDG(pid1) ,0);
        SetNPIDtypes(1);
        SetSwitchType(1);
       }
    else
       { 
        fHBTprocessor->SetPIDs(IdFromPDG(pid1) ,IdFromPDG(pid2));
        SetNPIDtypes(2);
       }
  }
/*******************************************************************/

void IlcGenHBTprocessor::SetNPIDtypes(Int_t npidt)
  {
    //Number ofparticle types to be processed - default 2
    //see IlcGenHBTprocessor::SetPIDs
    
    fNPidTypes = npidt;
    fHBTprocessor->SetNPIDtypes(npidt);
  }
/*******************************************************************/

void IlcGenHBTprocessor::SetDeltap(Float_t deltp) 
  {
  //default = 0.1 GeV
  //maximum range for random momentum shifts in GeV/c;
  //px,py,pz independent; Default = 0.1 GeV/c.
    fDeltap=deltp;
    fHBTprocessor->SetDeltap(deltp); 
  }
/*******************************************************************/

void IlcGenHBTprocessor::SetMaxIterations(Int_t maxiter) 
  { 
  //maximum # allowed iterations thru all the 
  //tracks for each event. Default = 50.
  //If maxit=0, then calculate the correlations 
  //for the input set of events without doing the
  //track adjustment procedure.
    
    fMaxit=maxiter;
    fHBTprocessor->SetMaxIterations(maxiter);
  }

/*******************************************************************/
void IlcGenHBTprocessor::SetDelChi(Float_t dc)
  {
    //min % change in total chi-square for which
    //the track adjustment iterations may stop,
    //Default = 0.1%.
    
    fDelchi=dc;
    fHBTprocessor->SetDelChi(dc);
  }
/*******************************************************************/

void IlcGenHBTprocessor::SetIRand(Int_t irnd) 
  { 
    //if fact dummy - used only for compatibility
    //we are using IlcRoot built in RNG
    //dummy in fact since we are using ilcroot build-in RNG
    //Sets renaodom number generator
    fIrand=irnd;
    fHBTprocessor->SetIRand(irnd);
  }
/*******************************************************************/
      
void IlcGenHBTprocessor::SetLambda(Float_t lam) 
  { 
  //default = 0.6
  // Sets Chaoticity Parameter
    fLambda = lam;
    fHBTprocessor->SetLambda(lam);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetR1d(Float_t r) 
  {
    //default 7.0
    //Sets Spherical source model radius (fm)
    fR1d = r;
    fHBTprocessor->SetR1d(r);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetRSide(Float_t rs) 
  {
   //default rs = 6.0
   //Rside,Rout,Rlong  = Non-spherical Bertsch-Pratt source model (fm)
   
    fRside = rs;
    fHBTprocessor->SetRSide(rs);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetROut(Float_t ro) 
  {
    //default ro = 7.0
    //Rside,Rout,Rlong  = Non-spherical Bertsch-Pratt source model (fm)
    fRout = ro;
    fHBTprocessor->SetROut(ro);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetRLong(Float_t rl) 
  {
    //default rl = 4.0
    //Rside,Rout,Rlong  = Non-spherical Bertsch-Pratt source model (fm)
    fRlong = rl;
    fHBTprocessor->SetRLong(rl);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetRPerp(Float_t rp) 
  {
   //default rp = 6.0
   //Rperp,Rparallel,R0= Non-spherical Yano-Koonin-Podgoretski source model (fm).
    fRperp = rp;
    fHBTprocessor->SetRPerp(rp);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetRParallel(Float_t rprl) 
  { 
   //default rprl = 4.0
   //Rperp,Rparallel,R0= Non-spherical Yano-Koonin-Podgoretski source model (fm).
    fRparallel = rprl;
    fHBTprocessor->SetRParallel(rprl);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetR0(Float_t r0) 
  {
  //default r0 = 4.0
  //Rperp,Rparallel,R0= Non-spherical Yano-Koonin-Podgoretski source model (fm).
    fR0 = r0;
    fHBTprocessor->SetR0(r0);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetQ0(Float_t q0) 
  { 
  //default q0 = 9.0
  //Sets Q0                = NA35 Coulomb parameter for finite source size in (GeV/c)
  //                         if fSwitchCoulomb = 2
  //                       = Spherical Coulomb source radius in (fm) 
  //                         if switchCoulomb = 3, used to interpolate the
  //                         input Pratt/Cramer discrete Coulomb source
  //                         radii tables.
    fQ0 = q0;
    fHBTprocessor->SetQ0(q0);
  }

/*******************************************************************/
void IlcGenHBTprocessor::SetSwitch1D(Int_t s1d) 
  {
//default s1d = 3
// Sets fSwitch1d   
//                          = 0 to not compute the 1D two-body //orrelations.
//                          = 1 to compute this using Q-invariant
//                          = 2 to compute this using Q-total
//                          = 3 to compute this using Q-3-ve//tor

    fSwitch1d = s1d;
    fHBTprocessor->SetSwitch1D(s1d);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetSwitch3D(Int_t s3d) 
  {
//default s3d = 0
// Sets fSwitch3d
//                         = 0 to not compute the 3D two-body correlations.
//                         = 1 to compute this using the side-out-long form
//                         = 2 to compute this using the Yanno-Koonin-Pogoredskij form.   
     
    fSwitch3d = s3d;
    fHBTprocessor->SetSwitch3D(s3d);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetSwitchType(Int_t st)
  {
//default st = 3
//  Sets  switch_type       = 1 to fit only the like pair correlations
//                          = 2 to fit only the unlike pair correlations
//                          = 3 to fit both the like and unlike pair correl.
//See SetPIDs and Init
//If only one particle type is set, unly==1 makes sens
  
    fSwitchType = st;
    fHBTprocessor->SetSwitchType(st);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetSwitchCoherence(Int_t sc)
  {
// default  sc = 0
//        switchCoherence  = 0 for purely incoherent source (but can have
//                              lambda < 1.0)
//                          = 1 for mixed incoherent and coherent source
  
    fSwitchCoherence = sc;
    fHBTprocessor->SetSwitchCoherence(sc);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetSwitchCoulomb(Int_t scol) 
  {
//default scol = 2
//        switchCoulomb    = 0 no Coulomb correction
//                          = 1 Point source Gamow correction only
//                          = 2 NA35 finite source size correction
//                          = 3 Pratt/Cramer finite source size correction;
//                              interpolated from input tables.
    fSwitchCoulomb =scol;
    fHBTprocessor->SetSwitchCoulomb(scol);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetSwitchFermiBose(Int_t sfb)
  {
//default sfb = 1
//        switchFermiBose =  1 Boson pairs
//                          = -1 Fermion pairs

    fSwitchFermiBose = sfb;
    fHBTprocessor->SetSwitchFermiBose(sfb);
  }
/*******************************************************************/
void IlcGenHBTprocessor::SetPtRange(Float_t ptmin, Float_t ptmax)
 {
// default ptmin = 0.1, ptmax = 0.98
//Sets Pt range (GeV)
   IlcGenerator::SetPtRange(ptmin,ptmax);
   fHBTprocessor->SetPtRange(ptmin,ptmax);
 }

/*******************************************************************/
void IlcGenHBTprocessor::SetPxRange(Float_t pxmin, Float_t pxmax)
 {
//default pxmin = -1.0, pxmax = 1.0
//Sets Px range 
  fPxMin =pxmin;
  fPxMax =pxmax;
  fHBTprocessor->SetPxRange(pxmin,pxmax);
 }
/*******************************************************************/
void IlcGenHBTprocessor::SetPyRange(Float_t pymin, Float_t pymax)
 {
//default  pymin = -1.0, pymax = 1.0
//Sets Py range 
  fPyMin =pymin;
  fPyMax =pymax;
   fHBTprocessor->SetPyRange(pymin,pymax);
 }
/*******************************************************************/
void IlcGenHBTprocessor::SetPzRange(Float_t pzmin, Float_t pzmax)
 {
//default pzmin = -3.6, pzmax = 3.6
//Sets Py range
   fPzMin =pzmin;
   fPzMax =pzmax; 
   fHBTprocessor->SetPzRange(pzmin,pzmax);
 }
void IlcGenHBTprocessor::SetMomentumRange(Float_t /*pmin*/, Float_t /*pmax*/)
 {
 //default pmin=0, pmax=0
 //Do not use this method!
    MayNotUse("IlcGenHBTprocessor::SetMomentumRange Method is Dummy");
 }
 
 /*******************************************************************/
void IlcGenHBTprocessor::SetPhiRange(Float_t phimin, Float_t phimax)
 {
//
//Sets \\Phi range  
  IlcGenerator::SetPhiRange(phimin,phimax);
  
  fHBTprocessor->SetPhiRange(phimin+180.0,phimax+180.0);
 }
/*******************************************************************/
void IlcGenHBTprocessor::SetEtaRange(Float_t etamin, Float_t etamax)
 {
//default etamin = -1.5, etamax = 1.5
//Sets \\Eta range   
   fEtaMin= etamin;
   fEtaMax =etamax;
   fHBTprocessor->SetEtaRange(etamin,etamax);
   
   //set the azimothal angle range in the IlcGeneraor - 
   //to keep coherency between azimuthal angle and pseudorapidity
   //DO NOT CALL this->SetThetaRange, because it calls this method (where we are) 
   //which must cause INFINITE LOOP
   IlcGenerator::SetThetaRange(RadiansToDegrees(EtaToTheta(fEtaMin)), 
                               RadiansToDegrees(EtaToTheta(fEtaMax)));
   
 }
/*******************************************************************/
void IlcGenHBTprocessor::SetThetaRange(Float_t thetamin, Float_t thetamax)
{
  //default thetamin = 0, thetamax = 180
  //Azimuthal angle, override IlcGenerator method which uses widely (i.e. wrapper generators)
  //core fortran HBTProcessor uses Eta (pseudorapidity)
  //so these methods has to be synchronized
  
  IlcGenerator::SetThetaRange(thetamin,thetamax);
  
  SetEtaRange( ThetaToEta(fThetaMin) , ThetaToEta(fThetaMax) );

}
  
/*******************************************************************/
void IlcGenHBTprocessor::SetNPtBins(Int_t nptbin)
 {
  //default nptbin = 50
  //set number of Pt bins  
   fNPtBins= nptbin; 
   fHBTprocessor->SetNPtBins(nptbin);
 }
/*******************************************************************/
void IlcGenHBTprocessor::SetNPhiBins(Int_t nphibin)
{ 
  //default nphibin = 50
  //set number of Phi bins
  fNPhiBins=nphibin;
  fHBTprocessor->SetNPhiBins(nphibin);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetNEtaBins(Int_t netabin)
{
  //default netabin = 50
  //set number of Eta bins
  fNEtaBins = netabin;
  fHBTprocessor->SetNEtaBins(netabin);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetNPxBins(Int_t npxbin)
{
  //default  npxbin = 20
  //set number of Px bins
  fNPxBins = npxbin; 
  fHBTprocessor->SetNPxBins(npxbin);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetNPyBins(Int_t npybin)
{
  //default  npybin = 20
  //set number of Py bins
  fNPyBins = npybin;
  fHBTprocessor->SetNPyBins(npybin);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetNPzBins(Int_t npzbin)
{
  //default npzbin = 70
  //set number of Pz bins
  fNPzBins = npzbin;
  fHBTprocessor->SetNPzBins(npzbin);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetNBins1DFineMesh(Int_t n)
{
//default n = 10
//Sets the number of bins in the 1D mesh
   fN1dFine =n;
   fHBTprocessor->SetNBins1DFineMesh(n);
   
}
/*******************************************************************/
void IlcGenHBTprocessor::SetBinSize1DFineMesh(Float_t x)
{
//default x=0.01
//Sets the bin size in the 1D mesh
   fBinsize1dFine = x;
   fHBTprocessor->SetBinSize1DFineMesh(x);
}
/*******************************************************************/
      
void IlcGenHBTprocessor::SetNBins1DCoarseMesh(Int_t n)
{
//default n =2
//Sets the number of bins in the coarse 1D mesh
  fN1dCoarse =n;
  fHBTprocessor->SetNBins1DCoarseMesh(n);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetBinSize1DCoarseMesh(Float_t x)
{
//default x=0.05
//Sets the bin size in the coarse 1D mesh
  fBinsize1dCoarse =x;
  fHBTprocessor->SetBinSize1DCoarseMesh(x);
}
/*******************************************************************/
      
void IlcGenHBTprocessor::SetNBins3DFineMesh(Int_t n)
{
//default n = 8
//Sets the number of bins in the 3D mesh
  fN3dFine =n;
  fHBTprocessor->SetNBins3DFineMesh(n);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetBinSize3DFineMesh(Float_t x)
{
//default x=0.01
//Sets the bin size in the 3D mesh
  fBinsize3dFine =x;
  fHBTprocessor->SetBinSize3DFineMesh(x);
}
/*******************************************************************/
      
void IlcGenHBTprocessor::SetNBins3DCoarseMesh(Int_t n)
{
//default n = 2
//Sets the number of bins in the coarse 3D mesh

  fN3dCoarse = n;
  fHBTprocessor->SetNBins3DCoarseMesh(n);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetBinSize3DCoarseMesh(Float_t x)
{
//default x=0.08
//Sets the bin size in the coarse 3D mesh
  fBinsize3dCoarse = x;
  fHBTprocessor->SetBinSize3DCoarseMesh(x);
}
/*******************************************************************/
      
void IlcGenHBTprocessor::SetNBins3DFineProjectMesh(Int_t n )
{
//default n =3
//Sets the number of bins in the fine project mesh
  fN3dFineProject = n;
  fHBTprocessor->SetNBins3DFineProjectMesh(n);
}
/*******************************************************************/
void IlcGenHBTprocessor::SetPrintFull(Int_t flag)
{
//sets the print full flag
 fPrintFull = flag;
 fHBTprocessor->SetPrintFull(flag);
}


/*******************************************************************/

Int_t  IlcGenHBTprocessor::GetNumberOfEvents()
{
//returns number of available events
  IlcGenerator* g = gIlc->GetMCApp()->Generator();
  IlcGenCocktailAfterBurner* cab = (g)?dynamic_cast<IlcGenCocktailAfterBurner*>(g):0x0;
  if (cab == 0x0)
   {
     Fatal("GetNumberOfEvents","Master Generator is not an IlcGenCocktailAfterBurner");
     return 0;
   }
  return (Int_t)TMath::Ceil(cab->GetNumberOfEvents()/((Float_t)fEventMerge));
}

/*******************************************************************/

void IlcGenHBTprocessor::SetActiveEventNumber(Int_t n)
{
//sets the active event
 fActiveStack =  n*fEventMerge;
 IlcDebug(1,Form("Settimg active event %d passed %d",fActiveStack,n));
}
/*******************************************************************/

Int_t  IlcGenHBTprocessor::GetNumberOfTracks()
{
//returns number of tracks in active event
  IlcGenerator* g = gIlc->GetMCApp()->Generator();
  IlcGenCocktailAfterBurner* cab = (g)?dynamic_cast<IlcGenCocktailAfterBurner*>(g):0x0;
  if (cab == 0x0)
   {
     Fatal("GetNumberOfEvents","Master Generator is not an IlcGenCocktailAfterBurner");
     return 0;
   }
 Int_t n = 0;
 for (Int_t i = fActiveStack;i < fActiveStack+fEventMerge; i++) 
  { 
    if (i >= GetNumberOfEvents()) break; //protection not to overshoot nb of events
    IlcStack* stack = cab->GetStack(i);
    if (stack == 0x0) {
     Error("GetNumberOfTracks","There is no stack %d",i);
     continue;
    }
    n+=stack->GetNprimary();
  }
 return n;
}
/*******************************************************************/

void IlcGenHBTprocessor::SetNEventsToMerge(Int_t nev)
{
 //sets number of events to merge
 if (nev > 0 ) fEventMerge = nev;
}
/*******************************************************************/

TParticle* IlcGenHBTprocessor::GetTrack(Int_t n)
{ 
//returns track that hbtp thinks is n in active event
  IlcDebug(5,Form("n = %d",n));
  IlcGenerator* g = gIlc->GetMCApp()->Generator();
  IlcGenCocktailAfterBurner* cab = (g)?dynamic_cast<IlcGenCocktailAfterBurner*>(g):0x0;
  if (cab == 0x0)
   {
     Fatal("GetTrackEventIndex","Master Generator is not an IlcGenCocktailAfterBurner");
     return 0;
   }

 Int_t ev, idx;
 GetTrackEventIndex(n,ev,idx);
 IlcDebug(5,Form("Event = %d Particle = %d",ev,idx));
 if ( (ev<0) || (idx<0) )
  {
    Error("GetTrack","GetTrackEventIndex returned error");
    return 0x0;
  }
 IlcDebug(5,Form("Number of Tracks in Event(%d) = %d",ev,cab->GetStack(ev)->GetNprimary()));
 return cab->GetStack(ev)->Particle(idx);//safe - in case stack does not exist 
}
/*******************************************************************/

void IlcGenHBTprocessor::GetTrackEventIndex(Int_t n, Int_t &evno, Int_t &index) const
{
 //returns event(stack) number and particle index
  IlcGenerator* g = gIlc->GetMCApp()->Generator();
  IlcGenCocktailAfterBurner* cab = (g)?dynamic_cast<IlcGenCocktailAfterBurner*>(g):0x0;
  if (cab == 0x0)
   {
     Fatal("GetTrackEventIndex","Master Generator is not an IlcGenCocktailAfterBurner");
     return;
   }

 evno = -1;
 index = -1;
 for (Int_t i = fActiveStack;i < fActiveStack+fEventMerge; i++) 
  { 
    IlcStack* stack = cab->GetStack(i);
    if (stack == 0x0)
     {
       Error("GetTrackEventIndex","There is no stack %d",i);
       return;
     }

    Int_t ntracks = stack->GetNprimary();
    IlcDebug(10,Form("Event %d has %d tracks. n = %d",i,ntracks,n));
    
    if ( ntracks > n) 
     {
       evno = i;
       index = n;
       return ;
     }
    else 
     {  
       n-=ntracks;
       continue;
     }
  }
 Error("GetTrackEventIndex","Could not find given track");
}

/*******************************************************************/
/*******************************************************************/
/*******************************************************************/





void IlcGenHBTprocessor::DefineParticles()
{
  //
  // Load standard numbers for GEANT particles and PDG conversion
  fNPDGCodes = 0; //this is done in the constructor - but in any case ...
  
  fPDGCode[fNPDGCodes++]=-99;   //  0 = unused location
  fPDGCode[fNPDGCodes++]=22;    //  1 = photon
  fPDGCode[fNPDGCodes++]=-11;   //  2 = positron
  fPDGCode[fNPDGCodes++]=11;    //  3 = electron
  fPDGCode[fNPDGCodes++]=12;    //  4 = neutrino e
  fPDGCode[fNPDGCodes++]=-13;   //  5 = muon +
  fPDGCode[fNPDGCodes++]=13;    //  6 = muon -
  fPDGCode[fNPDGCodes++]=111;   //  7 = pi0
  fPDGCode[fNPDGCodes++]=211;   //  8 = pi+
  fPDGCode[fNPDGCodes++]=-211;  //  9 = pi-
  fPDGCode[fNPDGCodes++]=130;   // 10 = Kaon Long
  fPDGCode[fNPDGCodes++]=321;   // 11 = Kaon +
  fPDGCode[fNPDGCodes++]=-321;  // 12 = Kaon -
  fPDGCode[fNPDGCodes++]=2112;  // 13 = Neutron
  fPDGCode[fNPDGCodes++]=2212;  // 14 = Proton
  fPDGCode[fNPDGCodes++]=-2212; // 15 = Anti Proton
  fPDGCode[fNPDGCodes++]=310;   // 16 = Kaon Short
  fPDGCode[fNPDGCodes++]=221;   // 17 = Eta
  fPDGCode[fNPDGCodes++]=3122;  // 18 = Lambda
  fPDGCode[fNPDGCodes++]=3222;  // 19 = Sigma +
  fPDGCode[fNPDGCodes++]=3212;  // 20 = Sigma 0
  fPDGCode[fNPDGCodes++]=3112;  // 21 = Sigma -
  fPDGCode[fNPDGCodes++]=3322;  // 22 = Xi0
  fPDGCode[fNPDGCodes++]=3312;  // 23 = Xi-
  fPDGCode[fNPDGCodes++]=3334;  // 24 = Omega-
  fPDGCode[fNPDGCodes++]=-2112; // 25 = Anti Proton
  fPDGCode[fNPDGCodes++]=-3122; // 26 = Anti Proton
  fPDGCode[fNPDGCodes++]=-3222; // 27 = Anti Sigma -
  fPDGCode[fNPDGCodes++]=-3212; // 28 = Anti Sigma 0
  fPDGCode[fNPDGCodes++]=-3112; // 29 = Anti Sigma 0
  fPDGCode[fNPDGCodes++]=-3322; // 30 = Anti Xi 0
  fPDGCode[fNPDGCodes++]=-3312; // 31 = Anti Xi +
  fPDGCode[fNPDGCodes++]=-3334; // 32 = Anti Omega +
}  

/*******************************************************************/
Int_t IlcGenHBTprocessor::IdFromPDG(Int_t pdg) const 
{
  //
  // Return Geant3 code from PDG and pseudo ENDF code
  //
  for(Int_t i=0;i<fNPDGCodes;++i)
    if(pdg==fPDGCode[i]) return i;
  return -1;
}
Int_t IlcGenHBTprocessor::PDGFromId(Int_t id) const
{
  //
  // Return PDG code and pseudo ENDF code from Geant3 code
  //
  if(id>0 && id<fNPDGCodes) return fPDGCode[id];
  else return -1;
}
Double_t IlcGenHBTprocessor::ThetaToEta(Double_t arg)
 {
  //converts etha(azimuthal angle) to Eta (pseudorapidity). Argument in radians

   if(arg>= TMath::Pi()) return  708.0;//This number is the biggest wich not crashes exp(x)
   if(arg<= 0.0) return -708.0;//
   if(arg == TMath::Pi()/2.) return 0.0;//
   
   if (arg > 0.0) 
    { 
      return TMath::Log( TMath::Tan(arg/2.)) ;
    }
   else 
    { 
      return -TMath::Log( TMath::Tan(-arg/2.)) ;
    }
 }
                                  
/*******************************************************************/
/******      ROUTINES    USED    FOR     COMMUNUCATION      ********/
/********************     WITH      FORTRAN     ********************/
/*******************************************************************/

#ifndef WIN32
  # define hbtpran hbtpran_  
  # define ilchbtp_puttrack ilchbtp_puttrack_
  # define ilchbtp_gettrack ilchbtp_gettrack_
  # define ilchbtp_getnumberevents ilchbtp_getnumberevents_
  # define ilchbtp_getnumbertracks  ilchbtp_getnumbertracks_
  # define ilchbtp_initialize ilchbtp_initialize_
  # define ilchbtp_setactiveeventnumber ilchbtp_setactiveeventnumber_
  # define ilchbtp_setparameters ilchbtp_setparameters_
  # define type_ofCall

#else
  # define hbtpran HBTPRAN
  # define ilchbtp_puttrack ILCHBTP_PUTTRACK
  # define ilchbtp_gettrack ILCHBTP_GETTRACK
  # define ilchbtp_getnumberevents ILCHBTP_GETNUMBEREVENTS
  # define ilchbtp_getnumbertracks  ILCHBTP_GETNUMBERTRACKS
  # define ilchbtp_initialize ILCHBTP_INITIALIZE
  # define ilchbtp_setactiveeventnumber ILCHBTP_SETACTIVEEVENTNUMBER
  # define ilchbtp_setparameters ILCHBTP_SETPARAMETERS
  # define type_ofCall  _stdcall
#endif    

/*******************************************************************/

IlcGenCocktailAfterBurner*  GetGenerator()
 {
   // This function has two tasks:
   // Check if environment is OK (exist gIlc and generator)
   // Returns pointer to genrator
   //to be changed with TFolders

   if(!gIlc)
    {
      ::Error("IlcGenHBTprocessor.cxx: GetGenerator()",
              "There is NO gILC! Check what you are doing!");
      ::Fatal("IlcGenHBTprocessor.cxx: GetGenerator()",
              "Running HBT Processor without gIlc... Exiting \n");
      return 0x0;//pro forma
    }
   IlcGenerator * gen = gIlc->GetMCApp()->Generator();
   
   if (!gen) 
    {
      ::Fatal("IlcGenHBTprocessor.cxx: GetGenerator()",
               "There is no generator in gIlc, exiting\n");
      return 0x0;
    }

   //we do not sure actual type of the genetator
   //and simple casting is risky - we use ROOT machinery to do safe cast
   
   TClass* cabclass = IlcGenCocktailAfterBurner::Class(); //get IlcGenCocktailAfterBurner TClass
   TClass* genclass = gen->IsA();//get TClass of the generator we got from gilc 
   //use casting implemented in TClass
   //cast gen to cabclass
   IlcGenCocktailAfterBurner* cab=(IlcGenCocktailAfterBurner*)genclass->DynamicCast(cabclass,gen);
                                                                        
   if (cab == 0x0)//if generator that we got is not IlcGenCocktailAfterBurner or its descendant we get null
   {              //then quit with error
      ::Fatal("IlcGenHBTprocessor.cxx: GetGenerator()",
              "The main Generator is not a IlcGenCocktailAfterBurner, exiting\n");
      return 0x0;
   }
   return cab;
 }
/*******************************************************************/

IlcGenHBTprocessor* GetIlcGenHBTprocessor()
{
//returns pointer to the current instance of IlcGenHBTprocessor in
//IlcGenCocktailAfterBurner (can be more than one)
//
 IlcGenCocktailAfterBurner* gen = GetGenerator();
 IlcGenerator* g = gen->GetCurrentGenerator();
 if(g == 0x0)
  {
    ::Fatal("IlcGenHBTprocessor.cxx: GetIlcGenHBTprocessor()",
                  "Can not get the current generator. Exiting");
    return 0x0;//pro forma
  }
  
 TClass* hbtpclass = IlcGenHBTprocessor::Class(); //get IlcGenCocktailAfterBurner TClass
 TClass* gclass = g->IsA();//get TClass of the current generator we got from CAB
 IlcGenHBTprocessor* hbtp = (IlcGenHBTprocessor*)gclass->DynamicCast(hbtpclass,g);//try to cast 
 if (hbtp == 0x0)
   {
      ::Fatal("IlcGenHBTprocessor.cxx: GetIlcGenHBTprocessor()",
              "\nCurrernt generator in IlcGenCocktailAfterBurner is not a IlcGenHBTprocessor, exiting\n");
      return 0x0;
   }
 return hbtp;
}

/*******************************************************************/
extern "C" void type_ofCall ilchbtp_setparameters()
 {
   //dummy
 }

extern "C" void type_ofCall  ilchbtp_initialize()
 {
   //dummy
 }

/*******************************************************************/

extern "C" void type_ofCall ilchbtp_getnumberevents(Int_t &nev)
 {
   //passes number of events to the fortran 
   if(IlcGenHBTprocessor::GetDebug()) 
    IlcInfoGeneral("ilchbtp_getnumberevents",Form("(%d) ....",nev));

   IlcGenHBTprocessor* gen = GetIlcGenHBTprocessor();//we dont check because it is done in function
   nev = gen->GetNumberOfEvents();
    
   if(IlcGenHBTprocessor::GetDebug()>5) 
    IlcInfoGeneral("ilchbtp_getnumberevents",Form("EXITED N Ev = %d",nev));
 }

/*******************************************************************/

extern "C" void type_ofCall  ilchbtp_setactiveeventnumber(Int_t & nev)
 {
//sets active event in generator (IlcGenCocktailAfterBurner)

   if(IlcGenHBTprocessor::GetDebug()>5) 
    ::Info("IlcGenHBTpocessor.cxx: ilchbtp_setactiveeventnumber","(%d)",nev);
   if(IlcGenHBTprocessor::GetDebug()>0)
    ::Info("IlcGenHBTpocessor.cxx: ilchbtp_setactiveeventnumber","Asked for event %d",nev-1);
    
   IlcGenHBTprocessor* gen = GetIlcGenHBTprocessor();//we dont check because it is done in function
   
   gen->SetActiveEventNumber(nev - 1); //fortran numerates events from 1 to N
   
   if(IlcGenHBTprocessor::GetDebug()>5) 
    ::Info("IlcGenHBTpocessor.cxx: ilchbtp_setactiveeventnumber","EXITED returned %d",nev);
 }
/*******************************************************************/
 
extern "C" void type_ofCall  ilchbtp_getnumbertracks(Int_t &ntracks)
 {
//passes number of particles in active event to the fortran  
   if(IlcGenHBTprocessor::GetDebug()>5) 
    ::Info("IlcGenHBTpocessor.cxx: ilchbtp_getnumbertracks","(%d)",ntracks);

   IlcGenHBTprocessor* gen = GetIlcGenHBTprocessor();//we dont check because it is done in function

   ntracks = gen->GetNumberOfTracks();
   if(IlcGenHBTprocessor::GetDebug()>5)
    ::Info("IlcGenHBTpocessor.cxx: ilchbtp_getnumbertracks","EXITED Ntracks = %d",ntracks); 
 }
 
/*******************************************************************/
 
extern "C" void type_ofCall  
   ilchbtp_puttrack(Int_t & n,Int_t& flag, Float_t& px, 
                    Float_t& py, Float_t& pz, Int_t& geantpid)
 {
//sets new parameters (momenta) in track number n
// in the active event
// n - number of the track in active event
// flag - flag of the track
// px,py,pz - momenta
// geantpid - type of the particle - Geant Particle ID
 
   if(IlcGenHBTprocessor::GetDebug()>5)
    ::Info("IlcGenHBTpocessor.cxx: ilchbtp_puttrack","(%d)",n);

   IlcGenHBTprocessor* gen = GetIlcGenHBTprocessor();//we dont check because it is done in function
   
   TParticle * track = gen->GetTrack(n-1);
   if (track == 0x0)
    {
      ::Error("IlcGenHBTprocessor.cxx","Can not get track from IlcGenHBTprocessor");
      return;
    }
       
   //check to be deleted 
   if (geantpid != (gen->IdFromPDG( track->GetPdgCode() )))
    {
      ::Error("IlcGenHBTprocessor.cxx: ilchbtp_puttrack",
              "SOMETHING IS GOING BAD:\n   GEANTPIDS ARE NOT THE SAME");
    }
   
   if(IlcGenHBTprocessor::GetDebug()>0)
     if (px != track->Px()) 
       ::Info("IlcGenHBTprocessor.cxx: ilchbtp_puttrack",
              "Px diff. = %f", px - track->Px());
   
   if(IlcGenHBTprocessor::GetDebug()>3)
     ::Info("IlcGenHBTprocessor.cxx: ilchbtp_puttrack",
            "track->GetPdgCode() --> %d",track->GetPdgCode());
   
   
   
   Float_t m = track->GetMass();
   Float_t e = TMath::Sqrt(m*m+px*px+py*py+pz*pz);
   track->SetMomentum(px,py,pz,e);
   
   gen->SetHbtPStatusCode(flag,n-1);
   
   if(IlcGenHBTprocessor::GetDebug()>5) ::Info("IlcGenHBTprocessor.cxx: ilchbtp_puttrack","EXITED ");
 }

/*******************************************************************/

extern "C" void type_ofCall  
  ilchbtp_gettrack(Int_t & n,Int_t & flag, Float_t & px, 
                   Float_t & py, Float_t & pz, Int_t & geantpid)
  
 {
//passes track parameters to the fortran
// n - number of the track in active event
// flag - flag of the track
// px,py,pz - momenta
// geantpid - type of the particle - Geant Particle ID
 
   if(IlcGenHBTprocessor::GetDebug()>5) ::Info("IlcGenHBTprocessor.cxx: ilchbtp_gettrack","(%d)",n);
   IlcGenHBTprocessor* g = GetIlcGenHBTprocessor();

   TParticle * track = g->GetTrack(n-1);
   
   flag = g->GetHbtPStatusCode(n-1);

   px = (Float_t)track->Px();
   py = (Float_t)track->Py();
   pz = (Float_t)track->Pz();
  
   geantpid = g->IdFromPDG( track->GetPdgCode() );
  
   if(IlcGenHBTprocessor::GetDebug()>5) ::Info("IlcGenHBTprocessor.cxx: ilchbtp_gettrack","EXITED"); 
 }

/*******************************************************************/
extern "C" Float_t type_ofCall hbtpran(Int_t &)
{
//interface to the random number generator
  return gIlcRandom->Rndm();
}        

/*******************************************************************/


/*******************************************************************/
