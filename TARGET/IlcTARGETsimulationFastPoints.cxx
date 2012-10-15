/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                          *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcTARGETsimulationFastPoints.cxx,v 1.2 2008/05/07 13:40:55 cgatto Exp $ */
//////////////////////////////////////////////////////////
// implements fast simulation                           //
//                                                      //
//                                                      //
//////////////////////////////////////////////////////////


#include <TRandom.h>

#include "IlcLog.h"
#include "IlcTARGET.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETmodule.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETgeomSPD.h"
#include "IlcRun.h"
#include "IlcTARGETsimulationFastPoints.h"
#include "IlcTARGETParam.h"


ClassImp(IlcTARGETsimulationFastPoints)



IlcTARGETsimulationFastPoints::IlcTARGETsimulationFastPoints():
  fExactPoints(false)
{
  //constructor
//   fSigmaRPhi[0] = fSigmaRPhi[1] = 12e-4;
//   fSigmaRPhi[2] = fSigmaRPhi[3] = 38e-4;
//   fSigmaRPhi[4] = fSigmaRPhi[5] = 20e-4;
//   fSigmaZ[0] = fSigmaZ[1] = 120e-4;        // resolution for 425 micron pixels
//   fSigmaZ[2] = fSigmaZ[3] = 28e-4;
//   fSigmaZ[4] = fSigmaZ[5] = 830e-4;
//   fSigmaDe[0] = fSigmaDe[1] = 0.72e-6;
//   fSigmaDe[2] = fSigmaDe[3] = 0.90e-6;
//   fSigmaDe[4] = fSigmaDe[5] =  5e-6;
//   fThrDe[0] = fThrDe[1] = 7.2e-6;
//   fThrDe[2] = fThrDe[3] = 2.70e-6;
//   fThrDe[4] = fThrDe[5] = 10e-6;
  
        // resolution for 5 micron pixels

IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
IlcTARGETgeom *gm = ilcTARGET->GetTARGETgeom2();

// if (gm->GetDetVersion() !=3 ) cout <<"in IlcTARGETsimulationFastPoints: Minor Version: "<<gm->GetDetVersion()<<endl;

IlcTARGETParam *Param = new IlcTARGETParam(ilcTARGET->GetMinorVersion());

// if (gm->GetDetVersion() == 3) Param=new IlcTARGETParam(gm->GetDetVersion());
// else if (gm->GetDetVersion() == 2) Param=new IlcTARGETParam(gm->GetDetVersion());
// else Param=new IlcTARGETParam(gm->GetDetVersion());

  Int_t nlayers=Param->GetNlayers();
  Int_t nendcaps=Param->GetNendcaps();
  Int_t nlayersStrip=Param->GetNlayersStrip();
  Int_t nendcapsStrip=Param->GetNendcapsStrip();
  fSigmaRPhi.Set(nlayers+nendcaps+nlayersStrip+nendcapsStrip);
  fSigmaZ.Set(nlayers+nendcaps+nlayersStrip+nendcapsStrip);
  fThrDe.Set(nlayers+nendcaps+nlayersStrip+nendcapsStrip);
  fSigmaDe.Set(nlayers+nendcaps+nlayersStrip+nendcapsStrip);



  for (Int_t i=0;i<nlayers;i++) fSigmaRPhi[i] = Param->GetSigmaRPhi(i); 
  for (Int_t i=0;i<nendcaps;i++) fSigmaRPhi[i+nlayers] = Param->GetSigmaRPhi(i+nlayers); 
  for (Int_t i=0;i<nlayersStrip;i++) fSigmaRPhi[i+nlayers+nendcaps] = Param->GetSigmaRPhi(i+nlayers+nendcaps); 
  for (Int_t i=0;i<nendcapsStrip;i++) fSigmaRPhi[i+nlayers+nendcaps+nlayersStrip] = Param->GetSigmaRPhi(i+nlayers+nendcaps+nlayersStrip); 

  for (Int_t i=0;i<nlayers;i++) fSigmaZ[i] = Param->GetSigmaZ(i); 
  for (Int_t i=0;i<nendcaps;i++) fSigmaZ[i+nlayers] = Param->GetSigmaZ(i+nlayers); 
  for (Int_t i=0;i<nlayersStrip;i++) fSigmaZ[i+nlayers+nendcaps] = Param->GetSigmaZ(i+nlayers+nendcaps); 
  for (Int_t i=0;i<nendcapsStrip;i++) fSigmaZ[i+nlayers+nendcaps+nlayersStrip] = Param->GetSigmaZ(i+nlayers+nendcaps+nlayersStrip); 

  for (Int_t i=0;i<nlayers;i++) fSigmaDe[i] = Param->GetSigmaDe(i); 
  for (Int_t i=0;i<nendcaps;i++) fSigmaDe[i+nlayers] = Param->GetSigmaDe(i+nlayers);
  for (Int_t i=0;i<nlayersStrip;i++) fSigmaDe[i+nlayers+nendcaps] = Param->GetSigmaDe(i+nlayers+nendcaps); 
  for (Int_t i=0;i<nendcapsStrip;i++) fSigmaDe[i+nlayers+nendcaps+nlayersStrip] = Param->GetSigmaDe(i+nlayers+nendcaps+nlayersStrip); 
 
  for (Int_t i=0;i<nlayers;i++) fThrDe[i] = Param->GetThrDe(i); 
  for (Int_t i=0;i<nendcaps;i++) fThrDe[i+nlayers] = Param->GetThrDe(i+nlayers); 
  for (Int_t i=0;i<nlayersStrip;i++) fThrDe[i+nlayers+nendcaps] = Param->GetThrDe(i+nlayers+nendcaps); 
  for (Int_t i=0;i<nendcapsStrip;i++) fThrDe[i+nlayers+nendcaps+nlayersStrip] = Param->GetThrDe(i+nlayers+nendcaps+nlayersStrip); 

  delete Param;

  fHitx1=0.0;
  fHity1=0.0;
  fHitz1=0.0;
  
}

//-------------------------------------------------------------
void IlcTARGETsimulationFastPoints::CreateFastRecPoints(Int_t module, TClonesArray* recp){
    // Fast points simulator
    

    
    IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");

    CreateFastRecPoints((IlcTARGETmodule *)(ilcTARGET->GetModule(module)),
			module,gRandom,recp);
}
//-------------------------------------------------------------
Bool_t IlcTARGETsimulationFastPoints::CreateFastRecPoints(IlcTARGETmodule *mod,
						     Int_t module,
						     TRandom *random,
						     TClonesArray* recp) {
  // Fast points simulator 
  

  

  TClonesArray &pt=*recp;
  IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
  IlcTARGETgeom *gm = ilcTARGET->GetTARGETgeom2();
 
  const Float_t kdEdXtoQ = 2.778e+8; 

  Int_t lay,lad,det;
  Bool_t IsHits=kFALSE;
  gm->GetModuleId(module,lay,lad,det);
  Int_t ind=(lad-1)*gm->GetNdetectors(lay)+(det-1);
  Int_t lyr=(lay-1);
  //cout <<"Fast recpoints per il modulo: "<<module<<endl;
  //cout <<"ladder="<<lad-1<<endl;
  //cout <<"det="<<det-1<<endl;
  //cout <<"lyr="<<lyr<<endl;
  
  Int_t ihit,numofhits;
  Float_t locals[3];
  Float_t globals[3];
  Double_t sigmarphi=0., sigmaz=0., sigmade=0., thrde=0.;
  Float_t deltaXl,deltaZl,deltaDe;

  Int_t hitlay, hitlad, hitdet, hitstatus;
  Float_t hitpx, hitpy, hitpz, hitdestep=0;

  Int_t   hitstatus1, hittrack1;
  Float_t hitx1, hity1, hitz1,hitt1;
  Float_t hitx2, hity2, hitz2;
  Float_t xMg,yMg,zMg;
  Int_t irecp=0;
  numofhits = mod->GetNhits();
  if (numofhits) { if (IlcLog::GetGlobalDebugLevel()>0) cout <<"numero del modulo: "<<module<<endl; 
  IsHits= kTRUE;}
  else IsHits=kFALSE;
  IlcDebug(1, Form(" number of hits = %d in module %d ", numofhits, module));
  
  //printf("numofhits %d \n",numofhits);
  for(ihit=0;ihit<numofhits;ihit++){
    IlcTARGEThit *hit=mod->GetHit(ihit);
    IlcDebug(1, Form("Status0 = %d",hit->GetTrackStatus0()));
IlcDebug(1, Form("Status = %d",hit->GetTrackStatus()));
IlcDebug(1, Form("**********************************"));
  if ((hit->GetTrackStatus0()==66)) {
   hit->GetPositionG0(hitx1,hity1,hitz1,hitt1);
   fHitx1=hitx1;
   fHity1=hity1;
   fHitz1=hitz1;
   hitdestep=hit->GetIonization();
  }else{
    hitdestep+=hit->GetIonization();

  }

  if (((hit->GetTrackStatus())!=65)) {
      hit->GetPositionG(hitx2,hity2,hitz2); 
    }
    else { continue;}
    
    IlcDebug(1, Form(" Global position: xg = %f,  yg = %f, zg = %f", hitx1, hity1, hitz1));
    
    hitstatus1 = hit->GetTrackStatus();
    
    IlcDebug(1, Form(" Track status = %d", hitstatus1));
    
    
    
    hittrack1 = hit->GetTrack();
    
    mod->MedianHitG(module,fHitx1,fHity1,fHitz1,hitx2,hity2,hitz2,xMg,yMg,zMg);
    //    mod->MedianHit(module,hitx1,hity1,hitz1,hitstatus1,xMg,yMg,zMg,flag);
    IlcDebug(1, Form("Info index=%d hit xyz: %f %f %f status %d xyzMg %f %f %f",module,hitx2,hity2,hitz2,hitstatus1,xMg,yMg,zMg));
    //flag=(((hit->GetTrackStatus0()&0x0002)!=0)&&((hit->GetTrackStatus()&0x0004)!=0));
    
    IlcDebug(1, Form(" Global median position: xMg = %f, yMg  = %f, zMg = %f", xMg, yMg, zMg));
    
    //if (flag==1) {
       IlcDebug(1, Form(" Energy deposited in the current step = %f", hitdestep));
       if (IlcLog::GetGlobalDebugLevel()>0) cout << "hitdestep:" <<hitdestep<<endl;
      if (hitdestep > 0) {
	hit->GetDetectorID(hitlay,hitlad,hitdet);
	
	IlcDebug(1, Form(" This hit belongs to layer %d, ladder %d, detector %d ", hitlay,hitlad,hitdet));
	
	hit->GetMomentumG(hitpx,hitpy,hitpz);  
	
	IlcDebug(1, Form(" Global momentum: px = %f, py  = %f, pz = %f", hitpx,hitpy,hitpz));
	          
	hitstatus = hitstatus1;
		// Transform to the module local frame
	globals[0] = xMg; 
	globals[1] = yMg;
	globals[2] = zMg;
        
	gm->GtoL(hitlay,hitlad,hitdet,globals,locals);
	IlcDebug(1, Form(" Local median position: xMl = %f,  yMl = %f, zMl = %f", locals[0], locals[1], locals[2]));
	// Retrieve sigma values for position and energy, and energy
	// threshold
	sigmarphi = SigmaRPhi(hitlay);
	sigmaz = SigmaZ(hitlay);
	sigmade = SigmaDe(hitlay);
	thrde = ThrDe(hitlay);
       	deltaXl = fExactPoints?0:random->Gaus(0,sigmarphi);
	deltaZl = fExactPoints?0:random->Gaus(0,sigmaz);
	deltaDe = fExactPoints?0:random->Gaus(0,sigmade);
	 
	IlcDebug(1, Form(" Deltas from the gaussian smearing: deltaXl = %f, deltaZl  = %f, deltaDe = %f", deltaXl, deltaZl, deltaDe));
	
	// Apply energy threshold and trasform back to global reference
	// system
	if ( (hitdestep+deltaDe) > thrde ){
	
	  IlcDebug(1, Form(" E + dE = %f is greater than Ethresold = %f : ", hitdestep+deltaDe, thrde)); 
          //cout << "fastrecpoint: local X: " << locals[0] << " local Z: " << locals[2] << endl;
	
	  locals[0] += deltaXl;

	  locals[2] += deltaZl;
	  IlcTARGETRecPoint rp(gm);
	  rp.SetLabel(hit->GetTrack(),0);
	  //		    rp.fTracks[0]=mod->GetHitTrackIndex(ihit);
	  rp.SetLabel(-3,1);
	  rp.SetLabel(-3,2);
	  
	  IlcDebug(1, Form(" xMl+deltaXl = %f, zMl+deltaZl = %f before SetXZ", locals[0], locals[2]));
          if (IlcLog::GetGlobalDebugLevel()>0) cout <<"Layer: " << lyr <<"Module: "<<module<<" ---- Hit[0]: " <<locals[0]<<" ---- Hit[1]: " <<locals[2]<<endl;

	  rp.SetXZ(module,locals[0],locals[2]);
	  
	  IlcDebug(1, Form(" xMl+deltaXl = %f, zMl+deltaZl = %f after SetXZ", locals[0], locals[2]));
          //printf("module = %d, xMl+deltaXl = %f, zMl+deltaZl = %f after SetXZ\n",module, locals[0], locals[2]);
	  
	  rp.SetdEdX(hitdestep+deltaDe);
	  rp.SetQ(kdEdXtoQ*(hitdestep+deltaDe));  // number of e
	  rp.SetSigmaDetLocX2(sigmarphi*sigmarphi);
	  rp.SetSigmaZ2(sigmaz*sigmaz);
	  rp.SetDetectorIndex(ind);
	  rp.SetLayer(lyr);
          rp.SetModule(module);
          rp.SetDetector(det-1);
	  new (pt[irecp]) IlcTARGETRecPoint(rp);
	  irecp++;
	} // end if ( (hitdestep+deltaDe)
	
	else IlcDebug(1, Form(" E + dE = %f is less than Ethresold = %f : ", hitdestep+deltaDe, thrde)); 
	
      } // end if (hitdestep > 0)
   // } // end if (flag!=1)
  } // end for ihit
 return IsHits;
}
//_______________________________________________________________________
void IlcTARGETsimulationFastPoints::SetSigmaRPhi(Int_t lay,Double_t srphi)
{
  // set sigmas in rphi
  


	fSigmaRPhi[lay]=srphi;
}
//_______________________________________________________________________
void IlcTARGETsimulationFastPoints::SetSigmaZ(Int_t lay,Double_t sz)
{
  // set sigmas in z
  
 

	fSigmaZ[lay]=sz;

}
//_______________________________________________________________________
void IlcTARGETsimulationFastPoints::SetSigmaDe(Int_t lay,Double_t sde)
{
  // set sigmas in energy
  


	fSigmaDe[lay]=sde;

}
//_______________________________________________________________________
void IlcTARGETsimulationFastPoints::SetThrDe(Int_t lay,Double_t thrde)
{
  // set energy thersholds
  


	fThrDe[lay]=thrde;

}

