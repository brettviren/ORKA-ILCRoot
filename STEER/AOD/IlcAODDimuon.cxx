// Infrastructure for Large Collider Experiment, All rights reserved.         *
// Author: P. Cortese, Universita' del Piemonte Orientale in Alessandria and
// INFN of Torino - Italy
//
// The class defines a dimuon pair object from two IlcAODTrack objects.
// IlcAODDimuon objects are supposed to be added to the IlcAODEvent structure
// during analysis. They would then allow to calculate the dimuon-related
// kinematic variables with a minimal disk occupancy.
// The payload of the class has been reduced to two pointers to the two
// tracks. An instance of this class has also to be added to the IlcAODEvent 
// structure to provide additional information that is specific to MUON and 
// therefore has not been included into the AOD header.
// Two transient data members are not stored on file as they can be recomputed
// at runtime.
//

#include "IlcAODDimuon.h"
#include "TLorentzVector.h"
#define IlcAODDimuon_CXX

ClassImp(IlcAODDimuon)

//______________________________________________________________________________
IlcAODDimuon::IlcAODDimuon():IlcVParticle(),fP(0),fMProton(0.93827231)
{
  // default constructor
  fMu[0]=0;
  fMu[1]=0;
}

//______________________________________________________________________________
IlcAODDimuon::IlcAODDimuon(const IlcAODDimuon& dimu):
  IlcVParticle(dimu),
  fP(0),fMProton(0.93827231)
{
  // copy constructor
  fMu[0]=dimu.Mu(0);
  fMu[1]=dimu.Mu(1);
}

//______________________________________________________________________________
IlcAODDimuon &IlcAODDimuon::operator=(const IlcAODDimuon& dimu)
{
  // assignment operator
  if(&dimu != this){
    delete fP;
    fP=0;
    fMProton=0.93827231;
    fMu[0]=dimu.Mu(0);
    fMu[1]=dimu.Mu(1);
  }
  return *this;
}

//______________________________________________________________________________
IlcAODDimuon::IlcAODDimuon(TObject *mu0, TObject *mu1):
  fP(0),fMProton(0.93827231)
{
  // Creates a dimuon pair from two tracks

  fMu[0]=mu0;
  fMu[1]=mu1;
}

//______________________________________________________________________________
IlcAODDimuon::~IlcAODDimuon()
{
  // destructor
  delete fP;
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Px() const {
  // Px of the dimuon
  if(CheckPointers())return -999999999;
  return ((IlcAODTrack*)fMu[0].GetObject())->Px()+
         ((IlcAODTrack*)fMu[1].GetObject())->Px();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Py() const {
  // Py of the dimuon
  if(CheckPointers())return -999999999;
  return ((IlcAODTrack*)fMu[0].GetObject())->Py()+
         ((IlcAODTrack*)fMu[1].GetObject())->Py();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Pz() const {
  // Pz of the dimuon
  if(CheckPointers())return -999999999;
  return ((IlcAODTrack*)fMu[0].GetObject())->Pz()+
         ((IlcAODTrack*)fMu[1].GetObject())->Pz();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Pt() const {
  // Pt of the dimuon
  if(CheckPointers())return -999999999;
  Double_t px=Px();
  Double_t py=Py();
  return TMath::Sqrt(px*px+py*py);
}

//______________________________________________________________________________
Double_t IlcAODDimuon::E() const 
{
    // Dimuon energy
  
  if(CheckPointers())return -999999999;

  return ((IlcAODTrack*)fMu[0].GetObject())->E()+ ((IlcAODTrack*)fMu[1].GetObject())->E();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::P() const {
  // Dimuon momentum
  if(CheckPointers())return -999999999;
  return TLV()->P();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::M() const {
  // Dimuon invariant mass
  if(CheckPointers())return -999999999;
  return TLV()->M();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Eta() const {
  // Dimuon pseudorapidity
  if(CheckPointers())return -999999999;
  return TLV()->Eta();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Phi() const {
  // Dimuon asimuthal angle
  if(CheckPointers())return -999999999;
  return TLV()->Phi();
}


//______________________________________________________________________________
Double_t IlcAODDimuon::Theta() const {
  // Dimuon polar angle
  if(CheckPointers())return -999999999;
  return TLV()->Theta();
}

//______________________________________________________________________________
Double_t IlcAODDimuon::Y() const {
  // Dimuon rapidity
  if(CheckPointers())return -999999999;
  return TLV()->Rapidity();
}

//______________________________________________________________________________
Short_t IlcAODDimuon::Charge() const {
  // Dimuon charge
  if(CheckPointers())return -999;
  return ((IlcAODTrack*)fMu[0].GetObject())->Charge()+
         ((IlcAODTrack*)fMu[1].GetObject())->Charge();
}

//______________________________________________________________________________
Int_t IlcAODDimuon::CheckPointers() const{
  // Checks if the track pointers have been initialized
  if(fMu[0]==0||fMu[1]==0){
    printf("Dimuon not initialized\n");
    return -999;
  }
  if((fMu[0].GetObject())==0||(fMu[1].GetObject())==0){
    printf("Can not get objects\n");
    return -999;
  }
  return 0;
}

//______________________________________________________________________________
void IlcAODDimuon::SetMu(Int_t imu, IlcAODTrack *mu){
  // Assign a track pointer
  if (imu==0||imu==1){
    delete fP;
    fP=0;
    fMu[imu]=mu;
  }
}

//______________________________________________________________________________
void IlcAODDimuon::SetMuons(IlcAODTrack *mu0, IlcAODTrack *mu1){
  // Assign the track pointers
  fMu[0]=mu0;
  fMu[1]=mu1;
  delete fP;
  fP=0;
}

//______________________________________________________________________________
Double_t IlcAODDimuon::XF() {
  // Dimuon Feynman x

  if(CheckPointers())return -999999999;

  //Double_t ebeam=((IlcAODEventInfo*)fEi.GetObject())->EBeam();
  Double_t ebeam = 3500.; // temporary
  if(ebeam<=0){
    printf("IlcAODDimuon::xf: can not compute xf with EBeam=%f\n",ebeam);
    return -999999999;
  }
  Double_t mDimu=M();
  Double_t pMax=TMath::Sqrt(ebeam*ebeam-mDimu*mDimu);
  return Pz()/pMax;
}

//______________________________________________________________________________
Double_t IlcAODDimuon::CostCS(){
  // Calculation the Collins-Soper angle (adapted from code by R. Arnaldi)
  if(CheckPointers())return -999999999;
  Double_t ebeam=3500.;  //temporary
  if(ebeam<=0){
    printf("Can not compute costCS with EBeam=%f\n",ebeam);
    return -999999999;
  }
  Double_t mp=fMProton;
  Double_t pbeam=TMath::Sqrt(ebeam*ebeam-mp*mp);
  Double_t pla10=((IlcAODTrack*)fMu[0].GetObject())->Px();
  Double_t pla11=((IlcAODTrack*)fMu[0].GetObject())->Py();
  Double_t pla12=((IlcAODTrack*)fMu[0].GetObject())->Pz();
  Double_t e1=((IlcAODTrack*)fMu[0].GetObject())->E();
  Double_t mu1Charge=((IlcAODTrack*)fMu[0].GetObject())->Charge();
  Double_t pla20=((IlcAODTrack*)fMu[1].GetObject())->Px();
  Double_t pla21=((IlcAODTrack*)fMu[1].GetObject())->Py();
  Double_t pla22=((IlcAODTrack*)fMu[1].GetObject())->Pz();
  Double_t e2=((IlcAODTrack*)fMu[1].GetObject())->E();
  Double_t mu2Charge=((IlcAODTrack*)fMu[1].GetObject())->Charge();

  // Fill the Lorentz vector for projectile and target
  // For the moment we do not consider the crossing angle
  // Projectile runs towards the MUON arm
  TLorentzVector pProjCM(0.,0.,-pbeam,ebeam); // projectile
  TLorentzVector pTargCM(0.,0., pbeam,ebeam); // target
  //
  // --- Get the muons parameters in the CM frame
  //
  TLorentzVector pMu1CM(pla10,pla11,pla12,e1);
  TLorentzVector pMu2CM(pla20,pla21,pla22,e2);
  //
  // --- Obtain the dimuon parameters in the CM frame
  //
  TLorentzVector pDimuCM=pMu1CM+pMu2CM;
  //
  // --- Translate the dimuon parameters in the dimuon rest frame
  //
  TVector3 beta=(-1./pDimuCM.E())*pDimuCM.Vect();
  TLorentzVector pMu1Dimu=pMu1CM;
  TLorentzVector pMu2Dimu=pMu2CM;
  TLorentzVector pProjDimu=pProjCM;
  TLorentzVector pTargDimu=pTargCM;
  pMu1Dimu.Boost(beta);
  pMu2Dimu.Boost(beta);
  pProjDimu.Boost(beta);
  pTargDimu.Boost(beta);
  //
  // --- Determine the z axis for the CS angle 
  //
  TVector3 zaxisCS=(((pProjDimu.Vect()).Unit())-((pTargDimu.Vect()).Unit())).Unit();
  //
  // --- Determine the CS angle (angle between mu+ and the z axis defined above)
  //
  Double_t cost;
  if(mu1Charge > 0) {
    cost = zaxisCS.Dot((pMu1Dimu.Vect()).Unit());
    // Theta CS is not properly defined for Like-Sign muons
    if(mu2Charge > 0 && cost<0) cost=-cost;
  } else { 
    // Theta CS is not properly defined for Like-Sign muons
    cost = zaxisCS.Dot((pMu2Dimu.Vect()).Unit());
    if(mu2Charge < 0 && cost<0) cost=-cost;
  }
  return cost;
}

//______________________________________________________________________________
Double_t IlcAODDimuon::CostHe(){
  // Calculation the Helicity polarization angle (adapted from code by R. Arnaldi)
  if(CheckPointers())return -999999999;
  Double_t ebeam=3500; //temporary
  if(ebeam<=0){
    printf("Can not compute costCS with EBeam=%f\n",ebeam);
    return -999999999;
  }
  Double_t pla10=((IlcAODTrack*)fMu[0].GetObject())->Px();
  Double_t pla11=((IlcAODTrack*)fMu[0].GetObject())->Py();
  Double_t pla12=((IlcAODTrack*)fMu[0].GetObject())->Pz();
  Double_t e1=((IlcAODTrack*)fMu[0].GetObject())->E();
  Double_t mu1Charge=((IlcAODTrack*)fMu[0].GetObject())->Charge();
  Double_t pla20=((IlcAODTrack*)fMu[1].GetObject())->Px();
  Double_t pla21=((IlcAODTrack*)fMu[1].GetObject())->Py();
  Double_t pla22=((IlcAODTrack*)fMu[1].GetObject())->Pz();
  Double_t e2=((IlcAODTrack*)fMu[1].GetObject())->E();
  Double_t mu2Charge=((IlcAODTrack*)fMu[1].GetObject())->Charge();
  //
  // --- Get the muons parameters in the CM frame
  //
  TLorentzVector pMu1CM(pla10,pla11,pla12,e1);
  TLorentzVector pMu2CM(pla20,pla21,pla22,e2);
  //
  // --- Obtain the dimuon parameters in the CM frame
  //
  TLorentzVector pDimuCM=pMu1CM+pMu2CM;
  //
  // --- Translate the dimuon parameters in the dimuon rest frame
  //
  TVector3 beta=(-1./pDimuCM.E())*pDimuCM.Vect();
  TLorentzVector pMu1Dimu=pMu1CM;
  TLorentzVector pMu2Dimu=pMu2CM;
  pMu1Dimu.Boost(beta);
  pMu2Dimu.Boost(beta);
  //
  // --- Determine the z axis for the calculation of the polarization angle
  // (i.e. the direction of the dimuon in the CM system)
  //
  TVector3 zaxis;
  zaxis=(pDimuCM.Vect()).Unit();
  //
  // --- Calculation of the polarization angle (Helicity)
  // (angle between mu+ and the z axis defined above)
  //
  Double_t cost;
  if(mu1Charge > 0) {
    cost = zaxis.Dot((pMu1Dimu.Vect()).Unit());
    // Theta Helicity is not properly defined for Like-Sign muons
    if(mu2Charge > 0 && cost<0) cost=-cost;
  } else { 
    cost = zaxis.Dot((pMu2Dimu.Vect()).Unit());
    // Theta Helicity is not properly defined for Like-Sign muons
    if(mu2Charge < 0 && cost<0) cost=-cost;
  }  
  return cost;
}

//________________________________________________________________________
Double_t IlcAODDimuon::PhiCS(){
  // Cosinus of the Collins-Soper polar decay angle
  if(CheckPointers())return -999999999;
  Double_t ebeam=3500.;  //temporary
  if(ebeam<=0){
    printf("Can not compute phiCS with EBeam=%f\n",ebeam);
    return -999999999;
  }
  Double_t mp=fMProton;
  Double_t pbeam=TMath::Sqrt(ebeam*ebeam-mp*mp);
  Double_t pla10=((IlcAODTrack*)fMu[0].GetObject())->Px();
  Double_t pla11=((IlcAODTrack*)fMu[0].GetObject())->Py();
  Double_t pla12=((IlcAODTrack*)fMu[0].GetObject())->Pz();
  Double_t e1=((IlcAODTrack*)fMu[0].GetObject())->E();
  Double_t mu1Charge=((IlcAODTrack*)fMu[0].GetObject())->Charge();
  Double_t pla20=((IlcAODTrack*)fMu[1].GetObject())->Px();
  Double_t pla21=((IlcAODTrack*)fMu[1].GetObject())->Py();
  Double_t pla22=((IlcAODTrack*)fMu[1].GetObject())->Pz();
  Double_t e2=((IlcAODTrack*)fMu[1].GetObject())->E();

  // Fill the Lorentz vector for projectile and target
  // For the moment we do not consider the crossing angle
  // Projectile runs towards the MUON arm
  TLorentzVector pProjCM(0.,0.,-pbeam,ebeam); // projectile
  TLorentzVector pTargCM(0.,0., pbeam,ebeam); // target
  //
  // --- Get the muons parameters in the CM frame
  //
  TLorentzVector pMu1CM(pla10,pla11,pla12,e1);
  TLorentzVector pMu2CM(pla20,pla21,pla22,e2);
  //
  // --- Obtain the dimuon parameters in the CM frame
  //
  TLorentzVector pDimuCM=pMu1CM+pMu2CM;
  //
  // --- Translate the dimuon parameters in the dimuon rest frame
  //
  TVector3 beta=(-1./pDimuCM.E())*pDimuCM.Vect();
  TLorentzVector pMu1Dimu=pMu1CM;
  TLorentzVector pMu2Dimu=pMu2CM;
  TLorentzVector pProjDimu=pProjCM;
  TLorentzVector pTargDimu=pTargCM;
  pMu1Dimu.Boost(beta);
  pMu2Dimu.Boost(beta);
  pProjDimu.Boost(beta);
  pTargDimu.Boost(beta);
  //
  // --- Determine the z axis for the CS angle 
  //
  TVector3 zaxisCS=(((pProjDimu.Vect()).Unit())-((pTargDimu.Vect()).Unit())).Unit();
  //
  // --- Determine the CS angle (angle between mu+ and the z axis defined above)
  //
   TVector3 yaxisCS=(((pProjDimu.Vect()).Unit()).Cross((pTargDimu.Vect()).Unit())).Unit();
   TVector3 xaxisCS=(yaxisCS.Cross(zaxisCS)).Unit();
 
   Double_t phi;
   if(mu1Charge>0) phi = TMath::ATan2((pMu1Dimu.Vect()).Dot(yaxisCS),((pMu1Dimu.Vect()).Dot(xaxisCS)));
   else phi = TMath::ATan2((pMu2Dimu.Vect()).Dot(yaxisCS),((pMu2Dimu.Vect()).Dot(xaxisCS)));
     
   return phi;
}

//______________________________________________________________________________
Double_t IlcAODDimuon::PhiHe(){
  // Calculation the Helicity aimuthal angle (adapted from code by R. Arnaldi)
  if(CheckPointers())return -999999999;
  Double_t ebeam=3500; //temporary
  if(ebeam<=0){
    printf("Can not compute phiHE with EBeam=%f\n",ebeam);
    return -999999999;
  }
  Double_t pbeam=TMath::Sqrt(ebeam*ebeam-fMProton*fMProton);
  Double_t pla10=((IlcAODTrack*)fMu[0].GetObject())->Px();
  Double_t pla11=((IlcAODTrack*)fMu[0].GetObject())->Py();
  Double_t pla12=((IlcAODTrack*)fMu[0].GetObject())->Pz();
  Double_t e1=((IlcAODTrack*)fMu[0].GetObject())->E();
  Double_t mu1Charge=((IlcAODTrack*)fMu[0].GetObject())->Charge();
  Double_t pla20=((IlcAODTrack*)fMu[1].GetObject())->Px();
  Double_t pla21=((IlcAODTrack*)fMu[1].GetObject())->Py();
  Double_t pla22=((IlcAODTrack*)fMu[1].GetObject())->Pz();
  Double_t e2=((IlcAODTrack*)fMu[1].GetObject())->E();

  // Fill the Lorentz vector for projectile and target
  // For the moment we consider no crossing angle
  // Projectile runs towards the MUON arm
  TLorentzVector pProjCM(0.,0.,-pbeam,ebeam); // projectile
  TLorentzVector pTargCM(0.,0., pbeam,ebeam); // target
  //
  // --- Get the muons parameters in the CM frame
  //
  TLorentzVector pMu1CM(pla10,pla11,pla12,e1);
  TLorentzVector pMu2CM(pla20,pla21,pla22,e2);
  //
  // --- Obtain the dimuon parameters in the CM frame
  //
  TLorentzVector pDimuCM=pMu1CM+pMu2CM;
  //
  // --- Translate the muon parameters in the dimuon rest frame
  // 
  TVector3 zaxis=(pDimuCM.Vect()).Unit(); 
  //
  // --- Translate the dimuon parameters in the dimuon rest frame
  //
  TVector3 beta=(-1./pDimuCM.E())*pDimuCM.Vect();
  TLorentzVector pMu1Dimu=pMu1CM;
  TLorentzVector pMu2Dimu=pMu2CM;
  pMu1Dimu.Boost(beta);
  pMu2Dimu.Boost(beta);
  
  TLorentzVector pProjDimu=pProjCM;
  TLorentzVector pTargDimu=pTargCM; 
  pProjDimu.Boost(beta);
  pTargDimu.Boost(beta);

  TVector3 yaxis=((pProjDimu.Vect()).Cross(pTargDimu.Vect())).Unit();
  TVector3 xaxis=(yaxis.Cross(zaxis)).Unit();
  //
  // --- Calculation of the azimuthal angle (Helicity)
  //
   Double_t phi;
   if(mu1Charge>0) phi = TMath::ATan2((pMu1Dimu.Vect()).Dot(yaxis),(pMu1Dimu.Vect()).Dot(xaxis));
   else phi = TMath::ATan2((pMu2Dimu.Vect()).Dot(yaxis),(pMu2Dimu.Vect()).Dot(xaxis));
   
   return phi;
}

//______________________________________________________________________________
Int_t IlcAODDimuon::AnyPt(){
  // Test if the two muons match two trigger tracks
  if(CheckPointers())return 0;
  return (((IlcAODTrack*)fMu[0].GetObject())->MatchTrigger())&&
         (((IlcAODTrack*)fMu[1].GetObject())->MatchTrigger());
}

//______________________________________________________________________________
Int_t IlcAODDimuon::LowPt(){
  // Test if the two muons match two trigger tracks with a "Low Pt" cut
  if(CheckPointers())return 0;
  return (((IlcAODTrack*)fMu[0].GetObject())->MatchTriggerLowPt())&&
         (((IlcAODTrack*)fMu[1].GetObject())->MatchTriggerLowPt());
}

//______________________________________________________________________________
Int_t IlcAODDimuon::HighPt(){
  // Test if the two muons match two trigger tracks with a "High Pt" cut
  if(CheckPointers())return 0;
  return (((IlcAODTrack*)fMu[0].GetObject())->MatchTriggerHighPt())&&
         (((IlcAODTrack*)fMu[1].GetObject())->MatchTriggerHighPt());
}

//______________________________________________________________________________
Double_t IlcAODDimuon::MaxChi2Match(){
  // Maximum matching Chi2 between track and trigger track
  if(CheckPointers())return -999999999;
  return TMath::Max((((IlcAODTrack*)fMu[0].GetObject())->GetChi2MatchTrigger()),
                    (((IlcAODTrack*)fMu[1].GetObject())->GetChi2MatchTrigger()));
}

//______________________________________________________________________________
TLorentzVector* IlcAODDimuon::TLV() const
{
  delete fP;
  fP = new TLorentzVector(Px(),Py(),Pz(),E());
  return fP;  
}
