/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
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

/* $Id: IlcTARGETgeomSSD.cxx,v 1.2 2008/11/05 01:47:14 rucco Exp $ */

////////////////////////////////////////////////////////////////////////
// This class is for the Silicon Strip Detector, SSD, specific geometry.
// It is being replaced by IlcTARGETsegmentationSSD class. This file also
// constains classes derived from IlcTARGETgeomSSD which do nothing but
// initilize this one with predefined values.
////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <stdlib.h>
#include <TShape.h>
#include <TBRIK.h>

#include "IlcRun.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETgeomSSD.h"


ClassImp(IlcTARGETgeomSSD)


IlcTARGETgeomSSD::IlcTARGETgeomSSD(){
// Default constructor
    fShapeSSD = 0;
    fNp       = 0;
    fNn       = 0;
    fLowEdgeP = 0;
    fLowEdgeN = 0;
    fAngleP   = 0.0;
    fAngleN   = 0.0;


}
//----------------------------------------------------------------------
IlcTARGETgeomSSD::IlcTARGETgeomSSD(const Double_t *box,Double_t ap,Double_t an,
			     Int_t np,Double_t *p,Int_t nn,Double_t *n){
////////////////////////////////////////////////////////////////////////
//    Standard Constructor. *box={dx,dy,dz}, ap=anode angle, an=cathode angle,
// nn= number of cathodes+1,*n= array of cathode low edges+highest edge,
// np= number of anodes+1, *p= array of anode low edges+lighest edge.
///////////////////////////////////////////////////////////////////////
    fShapeSSD = 0;
    fNp       = 0;
    fNn       = 0;
    fLowEdgeP = 0;
    fLowEdgeN = 0;
    fAngleP   = 0.0;
    fAngleN   = 0.0;

    ResetSSD(box,ap,an,np,p,nn,n);
}
//----------------------------------------------------------------------
void IlcTARGETgeomSSD::ResetSSD(const Double_t *box,Double_t ap,Double_t an,
			     Int_t np,Double_t *p,Int_t nn,Double_t *n){
////////////////////////////////////////////////////////////////////////
//    Standard Filler. *box={dx,dy,dz}, ap=anode angle, an=cathode angle,
// nn= number of cathodes+1,*n= array of cathode low edges+highest edge,
// np= number of anodes+1, *p= array of anode low edges+lighest edge.
///////////////////////////////////////////////////////////////////////
    Int_t i;

    fShapeSSD = new TBRIK("ActiveSSD","Active volume of SSD","SSD SI DET",
			  box[0],box[1],box[2]);
//    if(fLowEdgeP!=0) delete fLowEdgeP;
//    if(fLowEdgeN!=0) delete fLowEdgeN;
    fNp = np;
    fNn = nn;
    fAngleP = ap;
    fAngleN = an;
    fLowEdgeP = new Double_t[fNp];
    fLowEdgeN = new Double_t[fNn];
    for(i=0;i<fNp;i++) fLowEdgeP[i] = p[i];
    for(i=0;i<fNn;i++) fLowEdgeN[i] = n[i];
}
//______________________________________________________________________
IlcTARGETgeomSSD::~IlcTARGETgeomSSD(){
    // Destructor.

    if(fLowEdgeP) delete [] fLowEdgeP; fLowEdgeP = 0;
    if(fLowEdgeN) delete [] fLowEdgeN; fLowEdgeN = 0;
    if(fShapeSSD) delete  fShapeSSD; fShapeSSD = 0;
    fNp = 0;
    fNn = 0;
    fAngleP = 0.0;
    fAngleN = 0.0;
}
//______________________________________________________________________
IlcTARGETgeomSSD::IlcTARGETgeomSSD(const IlcTARGETgeomSSD &source) : TObject(source){
////////////////////////////////////////////////////////////////////////
//    copy  constructor
////////////////////////////////////////////////////////////////////////
    Int_t i;

    if(this == &source) return;
    this->fShapeSSD = new TBRIK(*(source.fShapeSSD));
    this->fNp = source.fNp;
    this->fNn = source.fNn;
    delete fLowEdgeP;
    delete fLowEdgeN;
    this->fAngleP = source.fAngleP;
    this->fAngleN = source.fAngleN;
    fLowEdgeP = new Double_t[fNp];
    fLowEdgeN = new Double_t[fNn];
    for(i=0;i<fNp;i++) this->fLowEdgeP[i] = source.fLowEdgeP[i];
    for(i=0;i<fNn;i++) this->fLowEdgeN[i] = source.fLowEdgeN[i];
    return;
}  

IlcTARGETgeomSSD& IlcTARGETgeomSSD::operator=(const IlcTARGETgeomSSD &source) {
////////////////////////////////////////////////////////////////////////
//    assignment operator
////////////////////////////////////////////////////////////////////////
    Int_t i;

    if(this == &source) return *this;
    this->fShapeSSD = new TBRIK(*(source.fShapeSSD));
    this->fNp = source.fNp;
    this->fNn = source.fNn;
    delete fLowEdgeP;
    delete fLowEdgeN;
    this->fAngleP = source.fAngleP;
    this->fAngleN = source.fAngleN;
    fLowEdgeP = new Double_t[fNp];
    fLowEdgeN = new Double_t[fNn];
    for(i=0;i<fNp;i++) this->fLowEdgeP[i] = source.fLowEdgeP[i];
    for(i=0;i<fNn;i++) this->fLowEdgeN[i] = source.fLowEdgeN[i];
    return *this;
}
//______________________________________________________________________
void IlcTARGETgeomSSD::Local2Det(Float_t x,Float_t z,Int_t &a,Int_t &c){
    // Given a GEANT detector local coordinate, cm, this function returns
    // the detector specific P and N side strip numbers.
    // Inputs are:
    // Float_t x   Geant detector local x coordinate in cm
    // Float_t z   Geant detector local z coordinate in cm
    // outputs are:
    // Int_t &a    Detector anode strip number (P side)
    // Int_t &c    Detector cathode strip number (N side)
    Float_t d,b;
    Int_t i;

    // project on to bonding edges.
    d = x*TMath::Cos(fAngleP)+z*TMath::Sin(fAngleP);
    b = x*TMath::Cos(fAngleN)+z*TMath::Sin(fAngleN);
    if(d<fLowEdgeP[0]) i=-1;
    else for(i=0;i<fNp;i++){
	if(fLowEdgeP[i]<d) break;
    } // end for i
    a = i;
    if(b<fLowEdgeN[0]) i=-1;
    else for(i=0;i<fNn;i++){
	if(fLowEdgeN[i]<b) break;
    } // end for i
    c = i;
    return;
}
//______________________________________________________________________
void IlcTARGETgeomSSD::Det2Local(Int_t a,Int_t c,Float_t &x,Float_t &z){
//    Float_t d,b;
//    Int_t i;
    // use IlcTARGETsegmentationSSD.

    x=a;
    z=c;
    Error("Det2Locat","Use IlcTARGETsegmentationSSD");
    return;
}
//______________________________________________________________________
void IlcTARGETgeomSSD::Print(ostream *os) const {
////////////////////////////////////////////////////////////////////////
// Standard output format for this class.
////////////////////////////////////////////////////////////////////////
    Int_t i;
#if defined __GNUC__
#if __GNUC__ > 2
    ios::fmtflags fmt;
#else
    Int_t fmt;
#endif
#else
#if defined __ICC || defined __ECC || defined __xlC__
    ios::fmtflags fmt;
#else
    Int_t fmt;
#endif
#endif

    fmt = os->setf(ios::scientific);  // set scientific floating point output
    *os << "TBRIK" << " ";
    *os << setprecision(16) << GetDx() << " ";
    *os << setprecision(16) << GetDy() << " ";
    *os << setprecision(16) << GetDz() << " ";
    *os << fNp << " " << fNn << " ";
    *os << setprecision(16) << fAngleP << " ";
    *os << setprecision(16) << fAngleN << " ";
    for(i=0;i<fNp;i++) *os << setprecision(16) << fLowEdgeP[i] << " ";
    for(i=0;i<fNn;i++) *os << setprecision(16) << fLowEdgeN[i] << " ";
    *os << endl;
    os->flags(fmt); // reset back to old formating.
    return;
}
//______________________________________________________________________
void IlcTARGETgeomSSD::Read(istream *is){
////////////////////////////////////////////////////////////////////////
// Standard input format for this class.
////////////////////////////////////////////////////////////////////////
    Float_t dx,dy,dz;
    Int_t i;
    char shp[20];

    *is >> shp;
    *is >> dx >> dy >> dz;
    if(fShapeSSD!=0) delete fShapeSSD;
    fShapeSSD = new TBRIK("ActiveSSD","Active volume of SSD","SSD SI DET",
			    dx,dy,dz);
    *is >> fNp >> fNn;
    *is >> fAngleP >> fAngleN;
    if(fLowEdgeP !=0) delete fLowEdgeP;
    if(fLowEdgeN !=0) delete fLowEdgeN;
    fLowEdgeP = new Double_t[fNp];
    fLowEdgeN = new Double_t[fNn];
    for(i=0;i<fNp;i++) *is >> fLowEdgeP[i];
    for(i=0;i<fNn;i++) *is >> fLowEdgeN[i];
    return;
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSSD &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSSD &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================

ClassImp(IlcTARGETgeomSSD175)

IlcTARGETgeomSSD175::IlcTARGETgeomSSD175() : IlcTARGETgeomSSD(){
////////////////////////////////////////////////////////////////////////
//    default constructor
////////////////////////////////////////////////////////////////////////
    const Double_t kDxyz[] ={3.6500,0.0150,2.000};//cm. (Geant 3.12 units)
    // Size of sensitive detector area x,y(thickness),z
    const Double_t kangle   = 0.0175; // angle in rad. of anode and cathodes
    const Double_t kpitch   = 0.0095;// cm anode separation.
    const Int_t   kNstrips = 768; // number of anode or cathode strips.
    Double_t *leA,*leC; // array of low edges anode and cathorde.
    Int_t i;

    leA = new Double_t[kNstrips+1];
    leC = new Double_t[kNstrips+1];
    leA[0] = -kDxyz[0];
    leA[1] = -kpitch*(0.5*kNstrips-1);
    leC[0] =  kDxyz[0];
    leC[1] =  kpitch*(0.5*kNstrips-1);
    for(i=1;i<kNstrips;i++){
	leA[i+1] = leA[i] + kpitch;
	leC[i+1] = leC[i] - kpitch;
    } // end for i
    leA[kNstrips] =  kDxyz[0];
    leC[kNstrips] = -kDxyz[0];
//    cout << "IlcTARGETgeomSSD175 default creator called: start" << endl;
    IlcTARGETgeomSSD::ResetSSD(kDxyz,kangle,-kangle,
				 kNstrips+1,leA,kNstrips+1,leC);
    delete leA;
    delete leC;
//    cout << "IlcTARGETgeomSSD175 default creator called: end" << endl;
}
//________________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETgeomSSD175 &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSSD175 &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
IlcTARGETgeomSSD& IlcTARGETgeomSSD175::operator=(const IlcTARGETgeomSSD &source) {
////////////////////////////////////////////////////////////////////////
//    assignment operator
////////////////////////////////////////////////////////////////////////
  

    if(this == &source) return *this;
    Error("IlcTARGETgeomSSD175","Not allowed to make a = with "
          "IlcTARGETgeomSSD175 Using default creater instead");

    return *this;
}
//======================================================================

ClassImp(IlcTARGETgeomSSD275and75)

IlcTARGETgeomSSD275and75::IlcTARGETgeomSSD275and75() : IlcTARGETgeomSSD(){
////////////////////////////////////////////////////////////////////////
//    default constructor
////////////////////////////////////////////////////////////////////////
}
//----------------------------------------------------------------------
IlcTARGETgeomSSD275and75::IlcTARGETgeomSSD275and75(Int_t npar,Double_t *par) : 
                                                            IlcTARGETgeomSSD(){
    // Default constructor for IlcTARGETgeomSSD with strip angles of
    // 275 miliradians and 75 miliradians. This constructor initlizes
    // IlcTARGETgeomSSD with the correct values. This is the miror image
    // of the IlcTARGETgeomSSD75and275 class.
    const Double_t kDxyz[] ={3.6500,0.0150,2.000};//cm. (Geant 3.12 units)
    // Size of sensitive detector area x,y(thickness),z
    const Double_t kangleA  = 0.0275; // angle in rad. of anode and cathodes
    const Double_t kangleC  = 0.0075; // angle in rad. of anode and cathodes
    const Double_t kpitch   = 0.0095;// cm anode separation.
    const Int_t   kNstrips = 768; // number of anode or cathode strips.
    Double_t *leA,*leC; // array of low edges anode and cathorde.
    Int_t i;

    if(npar<3){
	Error("IlcTARGETgeomSSD275and75",
	      "npar=%d<3. array par must be [3] or larger.",npar);
	return;
    } // end if
    leA = new Double_t[kNstrips+1];
    leC = new Double_t[kNstrips+1];
    leA[0] = -kDxyz[0];
    leA[1] = -kpitch*(0.5*kNstrips-1);
    leC[0] =  kDxyz[0];
    leC[1] =  kpitch*(0.5*kNstrips-1);
    for(i=1;i<kNstrips;i++){
	leA[i+1] = leA[i] + kpitch;
	leC[i+1] = leC[i] - kpitch;
    } // end for i
    leA[kNstrips] =  kDxyz[0];
    leC[kNstrips] = -kDxyz[0];
//    cout << "IlcTARGETgeomSSD275and75 default creator called: start" << endl;
    IlcTARGETgeomSSD::ResetSSD(par,kangleA,kangleC,
				 kNstrips+1,leA,kNstrips+1,leC);
    delete [] leA;
    delete [] leC;
//    cout << "IlcTARGETgeomSSD275and75 default creator called: end" << endl;
}
//________________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETgeomSSD275and75 &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSSD275and75 &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
IlcTARGETgeomSSD& IlcTARGETgeomSSD275and75::operator=(const IlcTARGETgeomSSD &source) {
////////////////////////////////////////////////////////////////////////
//    assignment operator
////////////////////////////////////////////////////////////////////////
  

    if(this == &source) return *this;
    Error("IlcTARGETgeomSSD275and75","Not allowed to make a = with "
          "IlcTARGETgeomSSD275and75 Using default creater instead");

    return *this;
}

//======================================================================

ClassImp(IlcTARGETgeomSSD75and275)

IlcTARGETgeomSSD75and275::IlcTARGETgeomSSD75and275() : IlcTARGETgeomSSD(){
////////////////////////////////////////////////////////////////////////
//    default constructor
////////////////////////////////////////////////////////////////////////
}
IlcTARGETgeomSSD75and275::IlcTARGETgeomSSD75and275(Int_t npar,Double_t *par) : 
                                                            IlcTARGETgeomSSD(){
    // Default constructor for IlcTARGETgeomSSD with strip angles of
    // 75 miliradians and 275 miliradians. This constructor initlizes
    // IlcTARGETgeomSSD with the correct values. This is the miror image
    // of the IlcTARGETgeomSSD275and75 class.
    const Double_t kDxyz[] ={3.6500,0.0150,2.000};//cm. (Geant 3.12 units)
    // Size of sensitive detector area x,y(thickness),z
    const Double_t kangleA  = 0.0075; // angle in rad. of anode and cathodes
    const Double_t kangleC  = 0.0275; // angle in rad. of anode and cathodes
    const Double_t kpitch   = 0.0095;// cm anode separation.
    const Int_t   kNstrips = 768; // number of anode or cathode strips.
    Double_t *leA,*leC; // array of low edges anode and cathorde.
    Int_t i;

    if(npar<3){
	Error("IlcTARGETgeomSSD75and275",
	      "npar=%d<3. array par must be [3] or larger.",npar);
	return;
    } // end if
    leA = new Double_t[kNstrips+1];
    leC = new Double_t[kNstrips+1];
    leA[0] = -kDxyz[0];
    leA[1] = -kpitch*(0.5*kNstrips-1);
    leC[0] =  kDxyz[0];
    leC[1] =  kpitch*(0.5*kNstrips-1);
    for(i=1;i<kNstrips;i++){
	leA[i+1] = leA[i] + kpitch;
	leC[i+1] = leC[i] - kpitch;
    } // end for i
    leA[kNstrips] =  kDxyz[0];
    leC[kNstrips] = -kDxyz[0];
//    cout << "IlcTARGETgeomSSD275and75 default creator called: start" << endl;
    IlcTARGETgeomSSD::ResetSSD(par,kangleA,kangleC,
				 kNstrips+1,leA,kNstrips+1,leC);
    delete leA;
    delete leC;
//    cout << "IlcTARGETgeomSSD275and75 default creator called: end" << endl;
}
//________________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETgeomSSD75and275 &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSSD75and275 &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
IlcTARGETgeomSSD& IlcTARGETgeomSSD75and275::operator=(const IlcTARGETgeomSSD &source) {
////////////////////////////////////////////////////////////////////////
//    assignment operator
////////////////////////////////////////////////////////////////////////
  

    if(this == &source) return *this;
    Error("IlcTARGETgeomSSD75and275","Not allowed to make a = with "
          "IlcTARGETgeomSSD75and275 Using default creater instead");

    return *this;
}

//======================================================================

//======================================================================

ClassImp(IlcTARGETgeomSSDStripEnd)

IlcTARGETgeomSSDStripEnd::IlcTARGETgeomSSDStripEnd() : IlcTARGETgeomSSD(){
////////////////////////////////////////////////////////////////////////
//    default constructor
////////////////////////////////////////////////////////////////////////
}
IlcTARGETgeomSSDStripEnd::IlcTARGETgeomSSDStripEnd(Int_t lay,IlcTARGETgeom *geom, IlcTARGETParam *Param, Int_t npar,Float_t *par) : 
                                                            IlcTARGETgeomSSD(){
    // Default constructor for IlcTARGETgeomSSD with strip angles of
    // 75 miliradians and 275 miliradians. This constructor initlizes
    // IlcTARGETgeomSSD with the correct values. This is the miror image
    // of the IlcTARGETgeomSSD275and75 class.
    Float_t pixelx,pixely,pixelz;
    const Double_t kconv=10000.;
    const Double_t kconv2=0.0001;


    geom->GetPixelDimension(lay,pixelx,pixely,pixelz);
//    cout <<"par[0]: "<<par[0]<<"par[2]: "<<par[2]<<"par[4]: "<<par[4]<<endl;   
    Double_t kDxyz[] ={par[0]+(Param->GetStereoP(lay)*par[4]*2.),par[2],par[4]};//cm. (Geant 3.12 units)
    // Size of sensitive detector area x,y(thickness),z

//if (geom->GetDetVersion() !=3 ) cout <<"in IlcTARGETgeomSSD: Minor Version: "<<geom->GetDetVersion()<<endl;

    const Double_t kangleA  = Param->GetStereoP(lay); // angle in rad. of anode and cathodes
    const Double_t kangleC  = Param->GetStereoN(lay); // angle in rad. of anode and cathodes
    
    const Double_t kpitch   = pixelx*kconv2;// cm anode separation.
//cout <<"pitch = pixelX:"<<kpitch<<endl;
    const Int_t   kNstrips = (Int_t)((kDxyz[0]*kconv*2.)/pixelx); // number of anode or cathode strips.
   //cout <<"Sono in IlcTARGETgeomSSD layer: "<<lay<<endl;
    //cout <<"Sono in IlcTARGETgeomSSD kNstrips: "<<kNstrips<<endl;

    Double_t *leA,*leC; // array of low edges anode and cathorde.
    Int_t i;
    geom->SetNPixelX(lay,kNstrips);
    geom->SetNPixelZ(lay,1);
    if(npar<3){
	Error("IlcTARGETgeomSSDStripEnd",
	      "npar=%d<3. array par must be [3] or larger.",npar);
	return;
    } // end if
    leA = new Double_t[kNstrips+1];
    leC = new Double_t[kNstrips+1];
    leA[0] = -kDxyz[0];
    leA[1] = -kpitch*(0.5*kNstrips-1);
    leC[0] =  kDxyz[0];
    leC[1] =  kpitch*(0.5*kNstrips-1);
    for(i=1;i<kNstrips;i++){
	leA[i+1] = leA[i] + kpitch;
	leC[i+1] = leC[i] - kpitch;
    } // end for i
    leA[kNstrips] =  kDxyz[0];
    leC[kNstrips] = -kDxyz[0];
    //for (Int_t k=0;k<=kNstrips;k++) cout <<"leA["<<k<<"]: "<<leA[k]<<endl;
    //for (Int_t s=0;s<=kNstrips;s++) cout <<"leC["<<s<<"]: "<<leC[s]<<endl;
//    cout << "IlcTARGETgeomSSD275and75 default creator called: start" << endl;
    Double_t par2[3];
    par2[0]=kDxyz[0];
    par2[1]=kDxyz[1];
    par2[2]=kDxyz[2];
    IlcTARGETgeomSSD::ResetSSD(par2,kangleA,kangleC,
				 kNstrips+1,leA,kNstrips+1,leC);
    delete leA;
    delete leC;
//    cout << "IlcTARGETgeomSSD275and75 default creator called: end" << endl;
}
//________________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETgeomSSDStripEnd &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSSDStripEnd &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
IlcTARGETgeomSSD& IlcTARGETgeomSSDStripEnd::operator=(const IlcTARGETgeomSSD &source) {
////////////////////////////////////////////////////////////////////////
//    assignment operator
////////////////////////////////////////////////////////////////////////
  

    if(this == &source) return *this;
    Error("IlcTARGETgeomSSD75and275","Not allowed to make a = with "
          "IlcTARGETgeomSSD75and275 Using default creater instead");

    return *this;
}

//======================================================================
//======================================================================

ClassImp(IlcTARGETgeomSSDStripLay)

IlcTARGETgeomSSDStripLay::IlcTARGETgeomSSDStripLay() : IlcTARGETgeomSSD(){
////////////////////////////////////////////////////////////////////////
//    default constructor
////////////////////////////////////////////////////////////////////////
}
IlcTARGETgeomSSDStripLay::IlcTARGETgeomSSDStripLay(Int_t lay,IlcTARGETgeom *geom, IlcTARGETParam *Param, Int_t npar,Float_t *par) : 
                                                            IlcTARGETgeomSSD(){
    // Default constructor for IlcTARGETgeomSSD with strip angles of
    // 75 miliradians and 275 miliradians. This constructor initlizes
    // IlcTARGETgeomSSD with the correct values. This is the miror image
    // of the IlcTARGETgeomSSD275and75 class.
    Float_t pixelx,pixely,pixelz;
    const Double_t kconv=10000.;
    const Double_t kconv2=0.0001;

    geom->GetPixelDimension(lay,pixelx,pixely,pixelz);
    const Double_t kDxyz[] ={par[0]+(Param->GetStereoP(lay)*par[2]*2.),par[1],par[2]};//cm. (Geant 3.12 units)
    // Size of sensitive detector area x,y(thickness),z

//if (geom->GetDetVersion() !=3 ) cout <<"2) in IlcTARGETgeomSSD: Minor Version: "<<geom->GetDetVersion()<<endl;

    
    const Double_t kangleA  = Param->GetStereoP(lay); // angle in rad. of anode and cathodes
    const Double_t kangleC  = Param->GetStereoN(lay); // angle in rad. of anode and cathodes
    
    const Double_t kpitch   = pixelx*kconv2;// cm anode separation.
//cout <<"pitch = pixelX:"<<kpitch<<endl;
    const Int_t   kNstrips = (Int_t)((kDxyz[0]*kconv*2.)/pixelx); // number of anode or cathode strips.
    //cout <<"kNstrips: "<<kNstrips<<endl;

    Double_t *leA,*leC; // array of low edges anode and cathorde.
    Int_t i;
    geom->SetNPixelX(lay,kNstrips);
    geom->SetNPixelZ(lay,1);
    if(npar<3){
	Error("IlcTARGETgeomSSDStripLay",
	      "npar=%d<3. array par must be [3] or larger.",npar);
	return;
    } // end if
    leA = new Double_t[kNstrips+1];
    leC = new Double_t[kNstrips+1];
    leA[0] = -kDxyz[0];
    leA[1] = -kpitch*(0.5*kNstrips-1);
    leC[0] =  kDxyz[0];
    leC[1] =  kpitch*(0.5*kNstrips-1);
    for(i=1;i<kNstrips;i++){
	leA[i+1] = leA[i] + kpitch;
	leC[i+1] = leC[i] - kpitch;
    } // end for i
    leA[kNstrips] =  kDxyz[0];
    leC[kNstrips] = -kDxyz[0];
    //for (Int_t k=0;k<=kNstrips;k++) cout <<"leA["<<k<<"]: "<<leA[k]<<endl;
    //for (Int_t s=0;s<=kNstrips;s++) cout <<"leC["<<s<<"]: "<<leC[s]<<endl;
//    cout << "IlcTARGETgeomSSD275and75 default creator called: start" << endl;
    Double_t par2[3];
    
    par2[0]=kDxyz[0];
    par2[1]=kDxyz[1];
    par2[2]=kDxyz[2];
    IlcTARGETgeomSSD::ResetSSD(par2,kangleA,kangleC,
				 kNstrips+1,leA,kNstrips+1,leC);
    delete leA;
    delete leC;
//    cout << "IlcTARGETgeomSSD275and75 default creator called: end" << endl;
}
//________________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETgeomSSDStripLay &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSSDStripLay &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
IlcTARGETgeomSSD& IlcTARGETgeomSSDStripLay::operator=(const IlcTARGETgeomSSD &source) {
////////////////////////////////////////////////////////////////////////
//    assignment operator
////////////////////////////////////////////////////////////////////////
  

    if(this == &source) return *this;
    Error("IlcTARGETgeomSSDStripLay","Not allowed to make a = with "
          "IlcTARGETgeomSSDStripLay Using default creater instead");

    return *this;
}

//======================================================================

