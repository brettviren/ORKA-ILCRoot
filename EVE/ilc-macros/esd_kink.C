// $Id: esd_kink.C 55699 2012-04-11 10:01:56Z hristov $
// Main authors: Paraskevi Ganoti: 2009

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TVector3.h>
#include <TEveVector.h>
#include <TEveTrackPropagator.h>
#include <TEveVSDStructs.h>
#include <TEveManager.h>

#include <IlcExternalTrackParam.h>
#include <IlcPID.h>
#include <IlcESDEvent.h>
#include <IlcESDtrack.h>
#include <IlcESDkink.h>
#include <IlcEveKink.h>
#include <IlcEveEventManager.h>
#endif

void esd_kink_init_rectrackMother(TEveRecTrack& rt, const IlcExternalTrackParam* tp)
{
  Double_t pbuf[3], vbuf[3];

  rt.fSign = tp->GetSign();
  tp->GetXYZ(vbuf);     rt.fV.Set(vbuf);
  tp->GetPxPyPz(pbuf);  rt.fP.Set(pbuf);
  
  rt.fBeta = 1; // ep/TMath::Sqrt(ep*ep + mc*mc);
}

void esd_kink_init_rectrackDaughter(TEveRecTrack& rt, const IlcExternalTrackParam* tp, TEveVector* svt,TEveVector* spt)
{
  rt.fSign = tp->GetSign();
  rt.fV.Set(*svt);
  rt.fP.Set(*spt);
  
  rt.fBeta = 1; // ep/TMath::Sqrt(ep*ep + mc*mc);
}

IlcEveKink* esd_make_kink(TEveTrackPropagator* rnrStyleMoth,TEveTrackPropagator* rnrStyleDaugh, IlcESDtrack* moth, IlcESDtrack* daug, IlcESDkink* kink, Int_t i)
{
  TEveRecTrack  rcMoth;
  TEveRecTrack  rcDaug;
  TEveRecKink   rcKink;
  
  const TVector3 p1(kink->GetMotherP());
  rcKink.fPMother.Set(p1);
  const TVector3 p2(kink->GetDaughterP());
  rcKink.fPDaughter.Set(p2);

  const TVector3 r1(kink->GetPosition());
  rcKink.fVKink.Set(r1);
  
  for (Int_t j=0; j<3; ++j) rcKink.fKinkAngle[j]=kink->GetAngle(j);
  
  Double_t r[3], r2[3];

  moth->GetTPCInnerParam()->GetXYZ(r2);  rcKink.fVMother.Set(r2);
  daug->GetOuterParam()->GetXYZ(r);  rcKink.fVDaughter.Set(r);

  esd_kink_init_rectrackMother(rcMoth, (moth->GetTPCInnerParam()));  
  rcMoth.fIndex = moth->GetID();
  
  esd_kink_init_rectrackDaughter(rcDaug, daug->GetOuterParam(), &rcKink.fVKink, &rcKink.fPDaughter);
  rcDaug.fIndex = daug->GetID();
  
  IlcEveKink* myKink = new IlcEveKink(&rcMoth, &rcDaug, &rcKink, rnrStyleMoth,rnrStyleDaugh);
  
  myKink->SetElementName(Form("ESDkink %d  \n", i));
  myKink->SetESDKinkIndex(i);
 
  for (Int_t j=0; j<3; ++j) myKink->SetKinkAngle(j, kink->GetAngle(j));
  Double_t daugProbability[10];
  Double_t daugP = 0.0;
  daug->GetESDpid(daugProbability);
  daugP = daug->P();

  // ****** Tentative particle type "concentrations"
  Double_t c[5]={0.01, 0.01, 0.85, 0.10, 0.05};
  IlcPID::SetPriors(c);

  IlcPID daugPid(daugProbability);

  Int_t   daugMostProbPdg =  0;

  switch (daugPid.GetMostProbable()){
  case 0:
    daugMostProbPdg =   11; break;
  case 1:
    daugMostProbPdg =   13; break;
  case 2:
    daugMostProbPdg =  211; break;
  default :
    daugMostProbPdg =  13; break;
  }

  Float_t daugMaxProbPid  = daugPid.GetProbability(daugPid.GetMostProbable());

  myKink->SetMaxProbPdgPid(daugMostProbPdg,daugMaxProbPid);//????????????

  return myKink;
} 


IlcEveKinkList* esd_kink()
{
  IlcESDEvent* esd = IlcEveEventManager::AssertESD();
  IlcEveKinkList* cont = new IlcEveKinkList("ESD kink");
  cont->SetMainColor(3); // green
  TEveTrackPropagator* rnrStyleMoth = cont->GetPropagatorMoth();
  rnrStyleMoth->SetMagField( 0.1*esd->GetMagneticField() );
  TEveTrackPropagator* rnrStyleDaugh = cont->GetPropagatorDaugh();
  rnrStyleDaugh->SetMagField( 0.1*esd->GetMagneticField() ); 
  rnrStyleDaugh->SetMaxR(520);
  gEve->AddElement(cont);

  Int_t count = 0;
//   for (Int_t n=0; n<esd->GetNumberOfKinks(); ++n) 
//   {
//     IlcESDkink *kink = esd->GetKink(n);  //???????????
//     printf("kink number = %d,  label of mother = %d , label of daug = %d --", n, kink->GetLabel(0), kink->GetLabel(1));
//   }   To be investigated...
    for (Int_t n=0; n<esd->GetNumberOfTracks(); ++n)
    { 
         IlcESDtrack* mtrack = esd->GetTrack(n);
         if(mtrack->GetKinkIndex(0)<0){
    
         IlcESDkink *kink = new IlcESDkink;
    
         kink=esd->GetKink(TMath::Abs(mtrack->GetKinkIndex(0))-1);
      
         for (Int_t m=0; m<esd->GetNumberOfTracks(); ++m)
         { 
              IlcESDtrack * dtrack = esd->GetTrack(m);

              if((dtrack->GetKinkIndex(0)>0)&&(dtrack->GetKinkIndex(0)==TMath::Abs(mtrack->GetKinkIndex(0)))) {
              IlcESDtrack* mothTr = esd->GetTrack(n);
              IlcESDtrack* daugTr = esd->GetTrack(m);
      
              IlcEveKink* myKink = esd_make_kink(rnrStyleMoth, rnrStyleDaugh, mothTr, daugTr, kink, (TMath::Abs(mtrack->GetKinkIndex(0))-1));
              if (myKink)
              {
                 gEve->AddElement(myKink, cont);
                 ++count;
              }
              }
         }  // inner track loop
    
 
         }  //mother kink index <0
    } // Outer track loop

  cont->SetTitle("test");

  cont->MakeKinks();
  gEve->Redraw3D();

  return cont;
}
