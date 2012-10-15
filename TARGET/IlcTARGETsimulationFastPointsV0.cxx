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
 ***********************************************************************/

/* $Id: IlcTARGETsimulationFastPointsV0.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */
/////////////////////////////////////////////////////////
//  fast simulation V0                                 //
//                                                     //
//                                                     //
/////////////////////////////////////////////////////////

#include "IlcTARGET.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETmodule.h"
#include "IlcTARGETsimulationFastPointsV0.h"
#include "IlcTARGETstatistics.h"

ClassImp(IlcTARGETsimulationFastPointsV0)

IlcTARGETsimulationFastPointsV0::IlcTARGETsimulationFastPointsV0()
{
  // default constructor
  fSx = 0;
  fSz = 0;
}
IlcTARGETsimulationFastPointsV0::IlcTARGETsimulationFastPointsV0(const char *dataType){
  //constructor
  Info("IlcTARGETsimulationFastPointsV0","Standard constructor %s",dataType);
  fSx = new IlcTARGETstatistics(2);
  fSz = new IlcTARGETstatistics(2);
}
//______________________________________________________________________
IlcTARGETsimulationFastPointsV0::IlcTARGETsimulationFastPointsV0(const IlcTARGETsimulationFastPointsV0 &/*rec*/):IlcTARGETsimulation(/*rec*/){
    // Copy constructor. 

  Error("Copy constructor","Copy constructor not allowed");
  
}
//______________________________________________________________________
IlcTARGETsimulationFastPointsV0& IlcTARGETsimulationFastPointsV0::operator=(const IlcTARGETsimulationFastPointsV0& /*source*/){
    // Assignment operator. This is a function which is not allowed to be
    // done.
    Error("operator=","Assignment operator not allowed\n");
    return *this; 
}

//----------------------------------------------------------
IlcTARGETsimulationFastPointsV0::~IlcTARGETsimulationFastPointsV0()
{
  //destructor
  if(fSx) delete fSx;
  if(fSz) delete fSz;

}

//-------------------------------------------------------------
Bool_t IlcTARGETsimulationFastPointsV0::CreateFastRecPoints(IlcTARGETmodule *mod,Int_t module,TRandom *rndm, TClonesArray* recp) {
  // Fast points simulator for all of the TARGET.

  Int_t   nhit,h,trk,ifirst;
  Float_t x,y,z,t,e;// local coordinate (cm) and time of flight, and dedx.
  Float_t x1,y1,z1;
  IlcTARGEThit *hit;

  if(rndm!=0) module=0; // fix unsued parameter warnings.
  fSx->Reset(); // Start out with things clearly zeroed
  fSz->Reset(); // Start out with things clearly zeroed
  e = 0.; // Start out with things clearly zeroed
  Double_t weight=1.;
  nhit = mod->GetNhits();
  ifirst = 1;
  for(h=0;h<nhit;h++){
    hit = mod->GetHit(h);
    hit->GetPositionL(x,y,z,t);
    if(ifirst) {x1=x;y1=y;z1=z;}
    e += hit->GetIonization();
    trk = hit->GetTrack();
    fSx->AddValue((Double_t)x,weight);
    fSz->AddValue((Double_t)z,weight);
    ifirst = 0;
    if(hit->StatusExiting()||  // leaving volume
       hit->StatusDisappeared()|| // interacted/decayed...
       hit->StatusStop() // dropped below E cuts.
       ){ // exiting track, write out RecPoint.
      //      if(fSz->GetRMS()>1.E-1) {
      //	TParticle *part = hit->GetParticle();
      //	printf("idpart %d energy %f \n",part->GetPdgCode(),part->Energy());
      //	printf("diffx=%e diffy=%e diffz=%e\n",x-x1,y-y1,z-z1);
      //      }
      switch (mod->GetLayer()){
      case 1: case 2:  // SPDs
	AddSPD(e,mod,trk,recp);
	break;
      case 3: case 4:  // SDDs
	AddSDD(e,mod,trk,recp);
	break;
      case 5: case 6:  // SSDs
	AddSSD(e,mod,trk,recp);
	break;
      } // end switch
      fSx->Reset();
      fSz->Reset();
      e = 0.;
      ifirst = 1;
      continue;
    }// end if
  } // end for h
return kTRUE;
}
//_______________________________________________________________________
void IlcTARGETsimulationFastPointsV0::AddSPD(Float_t &e,
					  IlcTARGETmodule* mod,Int_t trackNumber,TClonesArray* recp){
  //
  TClonesArray &pt=*recp;
  const Float_t kmicronTocm = 1.0e-4;
  //  const Float_t kdEdXtoQ = ;
  const Float_t kRMSx = 12.0*kmicronTocm; // microns->cm TARGET TDR Table 1.3
  const Float_t kRMSz = 70.0*kmicronTocm; // microns->cm TARGET TDR Table 1.3
  Float_t a1,a2; // general float.
  
  IlcTARGETgeom* gm = mod->GetTARGET()->GetTARGETgeom();

  IlcTARGETRecPoint rpSPD(gm);

  rpSPD.SetLabel(trackNumber,0);
  rpSPD.SetLabel(-3,1);
  rpSPD.SetLabel(-3,2);
  rpSPD.SetXZ(mod->GetIndex(),fSx->GetMean(),fSz->GetMean());
  rpSPD.SetdEdX(e);
  rpSPD.SetQ(1.0);
  a1 = fSx->GetRMS(); a1 *= a1; a1 += kRMSx*kRMSx;
  //  if(a1>1.E5) printf("addSPD: layer=%d track #%d dedx=%e sigmaX2= %e ",
  //		    mod->GetLayer(),trackNumber,e,a1);
  rpSPD.SetSigmaDetLocX2(a1);
  a2 = fSz->GetRMS(); a2 *= a2; a2 += kRMSz*kRMSz;
  //  if(a1>1.E5) printf(" sigmaZ2= %e\n",a2);
  rpSPD.SetSigmaZ2(a2);
  rpSPD.SetDetectorIndex(mod->GetIndex());
  rpSPD.SetLayer(mod->GetLayer());
  //(mod->GetTARGET())->AddRecPoint(rpSPD);
  new (pt[fNrecp]) IlcTARGETRecPoint(rpSPD);
  fNrecp++;
}
//_______________________________________________________________________
void IlcTARGETsimulationFastPointsV0::AddSDD(Float_t &e,
					  IlcTARGETmodule* mod,Int_t trackNumber,TClonesArray* recp){
  //
  TClonesArray &pt=*recp;
  const Float_t kmicronTocm = 1.0e-4;
  const Float_t kdEdXtoQ = 2.778e+8; 
  const Float_t kRMSx = 38.0*kmicronTocm; // microns->cm TARGET TDR Table 1.3
  const Float_t kRMSz = 28.0*kmicronTocm; // microns->cm TARGET TDR Table 1.3
  Float_t a1,a2; // general float.
  IlcTARGETgeom* gm = mod->GetTARGET()->GetTARGETgeom();

  IlcTARGETRecPoint rpSDD(gm);

  rpSDD.SetLabel(trackNumber,0);
  rpSDD.SetLabel(-3,1);
  rpSDD.SetLabel(-3,2);
  rpSDD.SetXZ(mod->GetIndex(),fSx->GetMean(),fSz->GetMean());
  rpSDD.SetdEdX(e);
  rpSDD.SetQ(kdEdXtoQ*e);
  a1 = fSx->GetRMS(); a1 *= a1; a1 += kRMSx*kRMSx;
  //  if(a1>1.E5) printf("addSDD: layer=%d track #%d dedx=%e sigmaX2= %e ",
  //		    mod->GetLayer(),trackNumber,e,a1);
  rpSDD.SetSigmaDetLocX2(a1);
  a2 = fSz->GetRMS(); a2 *= a2; a2 += kRMSz*kRMSz;
  //  if(a1>1.E5) printf(" sigmaZ2= %e\n",a2);
  rpSDD.SetSigmaZ2(a2);
  rpSDD.SetDetectorIndex(mod->GetIndex());
  rpSDD.SetLayer(mod->GetLayer());
  new (pt[fNrecp]) IlcTARGETRecPoint(rpSDD);
  fNrecp++;

}
//_______________________________________________________________________
void IlcTARGETsimulationFastPointsV0::AddSSD(Float_t &e,
					  IlcTARGETmodule* mod,Int_t trackNumber,TClonesArray* recp){
  // 
  TClonesArray &pt=*recp;
  const Float_t kmicronTocm = 1.0e-4;
  const Float_t kdEdXtoQ = 2.778e+8;
  const Float_t kRMSx = 20.0*kmicronTocm;  // microns->cm TARGET TDR Table 1.3
  const Float_t kRMSz = 830.0*kmicronTocm; // microns->cm TARGET TDR Table 1.3
  Float_t a1,a2; // general float.

  IlcTARGETgeom* gm = mod->GetTARGET()->GetTARGETgeom();
 
  IlcTARGETRecPoint rpSSD(gm);

  rpSSD.SetLabel(trackNumber,0);
  rpSSD.SetLabel(-3,1);
  rpSSD.SetLabel(-3,2);
  rpSSD.SetXZ(mod->GetIndex(),fSx->GetMean(),fSz->GetMean());
  rpSSD.SetdEdX(e);
  rpSSD.SetQ(kdEdXtoQ*e);
  a1 = fSx->GetRMS(); a1 *= a1; a1 += kRMSx*kRMSx;
  //  if(a1>1.E5) printf("addSSD: layer=%d track #%d dedx=%e sigmaX2= %e ",
  //		    mod->GetLayer(),trackNumber,e,a1);
  rpSSD.SetSigmaDetLocX2(a1);
  a2 = fSz->GetRMS(); a2 *= a2; a2 += kRMSz*kRMSz;
  //  if(a1>1.E5) printf(" sigmaZ2= %e RMSx=%e RMSz=%e\n",a2,fSx->GetRMS(),fSz->GetRMS());
  rpSSD.SetSigmaZ2(a2);
  rpSSD.SetDetectorIndex(mod->GetIndex());
  rpSSD.SetLayer(mod->GetLayer());
  new (pt[fNrecp]) IlcTARGETRecPoint(rpSSD);
  fNrecp++;


}
//_______________________________________________________________________

