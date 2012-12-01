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

//-------------------------------------------------------------------------
//               Implementation of the V0 vertexer class
//                  reads tracks writes out V0 vertices
//                      fills the ESD with the V0s       
//     Origin: Iouri Belikov, IPHC, Strasbourg, Jouri.Belikov@cern.ch
//-------------------------------------------------------------------------


#include "IlcESDEvent.h"
#include "IlcESDv0.h"
#include "IlcV0vertexer.h"

ClassImp(IlcV0vertexer)


//A set of very loose cuts 
Double_t IlcV0vertexer::fgChi2max=33.; //max chi2
Double_t IlcV0vertexer::fgDNmin=0.05;  //min imp parameter for the 1st daughter
Double_t IlcV0vertexer::fgDPmin=0.05;  //min imp parameter for the 2nd daughter
Double_t IlcV0vertexer::fgDCAmax=1.5;  //max DCA between the daughter tracks
Double_t IlcV0vertexer::fgCPAmin=0.9;  //min cosine of V0's pointing angle
Double_t IlcV0vertexer::fgRmin=0.2;    //min radius of the fiducial volume
Double_t IlcV0vertexer::fgRmax=200.;   //max radius of the fiducial volume

#ifdef WIN32
IlcV0vertexer::IlcV0vertexer() :
  TObject(),
  fChi2max(fgChi2max), 
  fDNmin(fgDNmin),
  fDPmin(fgDPmin),
  fDCAmax(fgDCAmax),
  fCPAmin(fgCPAmin), 
  fRmin(fgRmin),
  fRmax(fgRmax) 
{
}
  void IlcV0vertexer::SetCuts(const Double_t cuts[7]) {
  fChi2max=cuts[0]; 
  fDNmin=cuts[1];   fDPmin=cuts[2];
  fDCAmax=cuts[3];  fCPAmin=cuts[4];
  fRmin=cuts[5];    fRmax=cuts[6]; 
}
#endif

Int_t IlcV0vertexer::Tracks2V0vertices(IlcESDEvent *event) {
  //--------------------------------------------------------------------
  //This function reconstructs V0 vertices
  //--------------------------------------------------------------------

   const IlcESDVertex *vtxT3D=event->GetPrimaryVertex();

   Double_t xPrimaryVertex=vtxT3D->GetXv();
   Double_t yPrimaryVertex=vtxT3D->GetYv();
   Double_t zPrimaryVertex=vtxT3D->GetZv();

   Int_t nentr=event->GetNumberOfTracks();
   Double_t b=event->GetMagneticField();

   if (nentr<2) return 0; 

   TArrayI neg(nentr);
   TArrayI pos(nentr);

   Int_t nneg=0, npos=0, nvtx=0;

   Int_t i;
   for (i=0; i<nentr; i++) {
     IlcESDtrack *esdTrack=event->GetTrack(i);
     ULong_t status=esdTrack->GetStatus();

     //if ((status&IlcESDtrack::kITSrefit)==0)//not to accept the ITS SA tracks
        if ((status&IlcESDtrack::kTPCrefit)==0) continue;

     Double_t d=esdTrack->GetD(xPrimaryVertex,yPrimaryVertex,b);
     if (TMath::Abs(d)<fDPmin) continue;
     if (TMath::Abs(d)>fRmax) continue;

     if (esdTrack->GetSign() < 0.) neg[nneg++]=i;
     else pos[npos++]=i;
   }   


   for (i=0; i<nneg; i++) {
      Int_t nidx=neg[i];
      IlcESDtrack *ntrk=event->GetTrack(nidx);

      for (Int_t k=0; k<npos; k++) {
         Int_t pidx=pos[k];
	 IlcESDtrack *ptrk=event->GetTrack(pidx);

         if (TMath::Abs(ntrk->GetD(xPrimaryVertex,yPrimaryVertex,b))<fDNmin)
	   if (TMath::Abs(ptrk->GetD(xPrimaryVertex,yPrimaryVertex,b))<fDNmin) continue;

         Double_t xn, xp, dca=ntrk->GetDCA(ptrk,b,xn,xp);
         if (dca > fDCAmax) continue;
         if ((xn+xp) > 2*fRmax) continue;
         if ((xn+xp) < 2*fRmin) continue;
   
         IlcExternalTrackParam nt(*ntrk), pt(*ptrk);
         Bool_t corrected=kFALSE;
         if ((nt.GetX() > 3.) && (xn < 3.)) {
	   //correct for the beam pipe material
           corrected=kTRUE;
         }
         if ((pt.GetX() > 3.) && (xp < 3.)) {
	   //correct for the beam pipe material
           corrected=kTRUE;
         }
         if (corrected) {
	   dca=nt.GetDCA(&pt,b,xn,xp);
           if (dca > fDCAmax) continue;
           if ((xn+xp) > 2*fRmax) continue;
           if ((xn+xp) < 2*fRmin) continue;
	 }

         nt.PropagateTo(xn,b); pt.PropagateTo(xp,b);

         IlcESDv0 vertex(nt,nidx,pt,pidx);
         if (vertex.GetChi2V0() > fChi2max) continue;
	 
         Double_t x=vertex.Xv(), y=vertex.Yv();
         Double_t r2=x*x + y*y;
         if (r2 < fRmin*fRmin) continue;
         if (r2 > fRmax*fRmax) continue;

	 Float_t cpa=vertex.GetV0CosineOfPointingAngle(xPrimaryVertex,yPrimaryVertex,zPrimaryVertex);
         const Double_t pThr=1.5;
         Double_t pv0=vertex.P();
         if (pv0<pThr) {
           //Below the threshold "pThr", try a momentum dependent cos(PA) cut 
           const Double_t bend=0.03; // approximate Xi bending angle
           const Double_t qt=0.211;  // max Lambda pT in Omega decay
           const Double_t cpaThr=TMath::Cos(TMath::ATan(qt/pThr) + bend);
           Double_t 
           cpaCut=(fCPAmin/cpaThr)*TMath::Cos(TMath::ATan(qt/pv0) + bend); 
           if (cpa < cpaCut) continue;
         } else
	 if (cpa < fCPAmin) continue;

	 vertex.SetDcaV0Daughters(dca);
         vertex.SetV0CosineOfPointingAngle(cpa);
         vertex.ChangeMassHypothesis(kK0Short);

         event->AddV0(&vertex);

         nvtx++;
      }
   }

   Info("Tracks2V0vertices","Number of reconstructed V0 vertices: %d",nvtx);

   return nvtx;
}














