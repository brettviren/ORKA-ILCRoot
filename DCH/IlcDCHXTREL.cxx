#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#endif
#include <TProfile.h>
#include <TMath.h>
#include <fstream>
#include <string>
#include "IlcDCHXTREL.h"

ClassImp(IlcDCHXTREL)

IlcDCHXTREL::IlcDCHXTREL(const char *XTFname ){
  trxtf=0x0;
  trxt=0x0;
  trdata=0x0;
  radius =  new TVectorF(0);
  integrTime =  new TVectorF(0);
  integrDiffL =  new TVectorF(0);
  integrDiffT =  new TVectorF(0);
  coeffDiffL =  new TVectorF(0);
  coeffDiffT =  new TVectorF(0);
  Xxtx=0x0;
  Xxttime=0x0;
  XDiffL=0x0;
  XDiffT=0x0;
  XcoeffDiffL=0x0;
  XcoeffDiffT=0x0;
  xtData=0x0;
  firsttime = 0.;
  mindrifttime = 0.;
  maxdrifttime = 0.;
  Volts = 0.;
  Emax = 0.;
  Emin = 0.;
  Econst = 0.;
  E=0x0;
  Vdrift=0x0;
  DiffL=0x0;
  DiffT=0x0;
  filename=0x0;
  if (XTFname==0x0 || XTFname=="") xtfilename="XTtr_fromSigmaDiff.root";
  else xtfilename=XTFname;
  xtnbin = 0;
  curentr = -1;
  drifttime = 0.;

  Dpos = 0.;
  Dminpos = 0.;
  Dfirstpos = 0.;
  Ddrifttime = 0.;
  Dmindrifttime = 0.;
//  Dfirsttime = 0.;
  Dcurentr = -1;

  x = new Float_t [3];
  OpenXT();
}


IlcDCHXTREL::~IlcDCHXTREL(){
  if(trxtf!=0) { if (trxtf->IsOpen()) {trxtf->Close();} }
  delete[] x;
}

void IlcDCHXTREL::OpenXT(){
  trxtf = new TFile(xtfilename.Data(),"read");
  if (!trxtf->IsOpen()) trxtf->Open(xtfilename.Data());
  trxt = (TTree *) trxtf->Get("XTrelation");
  xtData= new XTDataVlGas();
  trxt->SetBranchAddress("XTData",&xtData);
//  IlcDCHXTDataVlGas *xtData = (IlcDCHXTDataVlGas *) trxt->GetBranch("XTData");
  trxt->GetEntry(0);
  Rwire = xtData->GetRwire();
  InnerRtube = xtData->GetInnerRtube();
  Volts = xtData->GetVolts();
  Econst = Volts/TMath::Log( InnerRtube/Rwire );
  Emax = Econst/Rwire;
  Emin = Econst/InnerRtube;
  pos = Rwire;  //r wire
  Xxtx = xtData->GetDist();
  Xxttime = xtData->GetTime();
  XDiffL = xtData->GetDiffL();
  XDiffT = xtData->GetDiffT();
  firsttime = Xxttime[0];
  mindrifttime = 0.;
  xtnbin = xtData->GetNpoints();

  Dpos = 0.;
  Dfirstpos = Xxtx[0];
  Dmindrifttime = 0.;
  Dminpos = 0.;
  maxdrifttime = Xxttime[xtnbin-1];
}


Float_t* IlcDCHXTREL::time2dist(Float_t time){
  
  if ( time <= firsttime ) {
    x[0] = Rwire;
    x[1] = XDiffL[0];
    x[2] = XDiffT[0];
    return x;
  }
  if ( time <= mindrifttime && curentr != -1){
    curentr = -1;
    drifttime = 0.;
  }
  while (time>drifttime && curentr != (xtnbin-1)){
    curentr++;
    minpos = pos;
    mindrifttime = drifttime;
    drifttime = Xxttime[curentr];
    pos = Xxtx[curentr];
    x[1] = XDiffL[curentr];
    x[2] = XDiffT[curentr];
  }
  if (curentr == xtnbin-1){
    if( time>drifttime ) {
      if (drifttime==mindrifttime) x[0] = pos;
      else x[0] = (pos-minpos)/(drifttime-mindrifttime)*(time-drifttime) + pos; //linear extrapolation afer last point
    }
    else x[0] = (pos-minpos)/(drifttime-mindrifttime)*(time-mindrifttime) + minpos; //linear interpolation
//    else x=(pos+minpos)/2;

    x[0] = (x[0] > InnerRtube) ? InnerRtube : x[0];
  }
  else{
    x[0] = (pos-minpos)/(drifttime-mindrifttime)*(time-mindrifttime) + minpos; //linear interpolation
//    x = (pos+minpos)/2;
  }
//cout<<curentr<<" "<<xtnbin<<" "<<"time "<<time<<" t1 "<<mindrifttime<<" t2 "<<drifttime<<" x "<<x<<" x1 "<<minpos<<" x2 "<<pos<<endl;
  x[0] = (x[0] < Rwire) ? Rwire : x[0];
  return x;

}

Float_t* IlcDCHXTREL::dist2time(Float_t dist){
  
  dist = dist+Rwire;
  
  if ( dist <= Dfirstpos ) {
    x[0] = 0.;
    x[1] = XDiffL[0];
    x[2] = XDiffT[0];
    return x;
  }
//   if ( dist <= Dminpos && Dcurentr != -1){
//     Dcurentr = -1;
//     Dpos = 0.;
//   }
  
  int index=TMath::BinarySearch(xtnbin,Xxtx,dist)+1;
  if(index>xtnbin-1) index=xtnbin-1;

  Dcurentr=index;
  Dpos = Xxtx[Dcurentr];

//   while (dist>Dpos && Dcurentr != (xtnbin-1)){
//     Dcurentr++;
//     Dpos = Xxtx[Dcurentr];
//   }

  if(Dcurentr>0){
    Dminpos = Xxtx[Dcurentr-1];
    Dmindrifttime = Xxttime[Dcurentr-1];
  }
  Ddrifttime = Xxttime[Dcurentr];
  x[1] = XDiffL[Dcurentr];
  x[2] = XDiffT[Dcurentr];
  
  if (Dcurentr == xtnbin-1){
    if( dist>Dpos ) {
      if (Dpos==Dminpos) x[0] = Ddrifttime;
      else x[0] = (Ddrifttime-Dmindrifttime)/(Dpos-Dminpos)*(dist-Dpos) + Ddrifttime; //linear extrapolation afer last point
    }
    else x[0] = (Ddrifttime-Dmindrifttime)/(Dpos-Dminpos)*(dist-Dminpos) + Dmindrifttime; //linear interpolation
//    else x=(pos+minpos)/2;
    x[0] = (x[0] > maxdrifttime) ? maxdrifttime : x[0];
  }else if(Dcurentr==0||fabs(Dpos-Dminpos)<1e-10){
    x[0] = Ddrifttime;
  }else{
    x[0] = (Ddrifttime-Dmindrifttime)/(Dpos-Dminpos)*(dist-Dminpos) + Dmindrifttime; //linear interpolation
//    x = (pos+minpos)/2;
  }
//cout<<curentr<<" "<<xtnbin<<" "<<"time "<<time<<" t1 "<<mindrifttime<<" t2 "<<drifttime<<" x "<<x<<" x1 "<<minpos<<" x2 "<<pos<<endl;

  x[0] = (x[0] > 0.) ? x[0] : 0.;

  return x;

}
