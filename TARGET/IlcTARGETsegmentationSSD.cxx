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

/* $Id: IlcTARGETsegmentationSSD.cxx,v 1.3 2009/12/11 11:36:18 vitomeg Exp $ */

#include <Riostream.h>
#include <TMath.h>

#include "IlcRun.h"
#include "IlcTARGETsegmentationSSD.h"
#include "IlcTARGETgeomSSD.h"

//////////////////////////////////////////////////////
// Segmentation class for                           //
// silicon strips                                   //
//                                                  //
//////////////////////////////////////////////////////
const Float_t IlcTARGETsegmentationSSD::fgkDxDefault = 72960.;
const Float_t IlcTARGETsegmentationSSD::fgkDzDefault = 40000.;
const Float_t IlcTARGETsegmentationSSD::fgkDyDefault = 300.;
const Float_t IlcTARGETsegmentationSSD::fgkPitchDefault = 95.;
const Int_t IlcTARGETsegmentationSSD::fgkNstripsDefault = 768;

ClassImp(IlcTARGETsegmentationSSD)
IlcTARGETsegmentationSSD::IlcTARGETsegmentationSSD(): IlcTARGETsegmentation2() {
    // default constructor
}
//----------------------------------------------------------------------
IlcTARGETsegmentationSSD::IlcTARGETsegmentationSSD(IlcTARGETgeom *geom) {
    // constuctor
    fGeom2 = geom;
    fCorr = 0;
    const Float_t kconv=10000.;
    Float_t pixelx,pixely,pixelz;
    Int_t npixelx;
    IlcTARGETgeomSSD  *s0;
    //dan-commento cout <<"chiama prima il costruttore della segmentazione"<<endl;
//if (geom->GetDetVersion() !=3 ) cout <<"in IlcTARGETsegmentationSSD: Minor Version: "<<geom->GetDetVersion()<<endl;

    Int_t nlayersStrip=fParam->GetNlayersStrip();
    Int_t nendcapsStrip=fParam->GetNendcapsStrip();

    Float_t stereop;
    Float_t stereon;
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    fNstrips=new Int_t[nendcapsStrip+nlayersStrip];
    fStereoP=new Float_t[nendcapsStrip+nlayersStrip];
    fStereoN=new Float_t[nendcapsStrip+nlayersStrip];
    fPitch=new Float_t[nendcapsStrip+nlayersStrip];
    fStereoPl=new Float_t[nendcapsStrip+nlayersStrip];
    fStereoNl=new Float_t[nendcapsStrip+nlayersStrip];

    for (Int_t lay=0;lay<nendcapsStrip+nlayersStrip;lay++) {
        stereop=fParam->GetStereoP(lay+nlayers+nendcaps);
        stereon=fParam->GetStereoN(lay+nlayers+nendcaps);
        SetAngles(lay,stereop,stereon);
        fGeom2->GetPixelDimension(lay+nlayers+nendcaps,pixelx,pixely,pixelz);
	SetPadSize(lay,pixelx,0.);
	fGeom2->GetNPixelX(lay+nlayers+nendcaps,npixelx);
        SetNPads(lay,npixelx,0);

	s0 = dynamic_cast<IlcTARGETgeomSSD*> (fGeom2->GetShape2(lay+nlayers+nendcaps));
	if(!s0){
	  SetDetSize(lay+nlayers+nendcaps,0,0,0);
	  continue;
	}

        //cout <<"dimensione pitch in SegmentationSSD: "<<pixelx<<endl;
        //dan-commento cout <<"Segmentazione... Dx: "<<s0->GetDx()<<"Dy: "<<s0->GetDy()<<"Dz: "<<s0->GetDz()<<endl;
        SetDetSize(lay+nlayers+nendcaps,s0->GetDx()*2.*kconv,s0->GetDz()*2.*kconv,s0->GetDy()*2.*kconv);
        //cout <<"stereop: "<<stereop<<" stereon: "<<stereon<<endl;
        fLayer = 0;
    }
}

//______________________________________________________________________
void IlcTARGETsegmentationSSD::Copy(TObject &obj) const {
    // protected method. copy this to obj
    IlcTARGETsegmentation2::Copy(obj);
    ((IlcTARGETsegmentationSSD& ) obj).Clear();
    ((IlcTARGETsegmentationSSD& ) obj).fNstrips = fNstrips;
    ((IlcTARGETsegmentationSSD& ) obj).fStereoP = fStereoP;
    ((IlcTARGETsegmentationSSD& ) obj).fStereoN = fStereoN;
    ((IlcTARGETsegmentationSSD& ) obj).fLayer   = fLayer;
    ((IlcTARGETsegmentationSSD& ) obj).fPitch   = fPitch;
    ((IlcTARGETsegmentationSSD& ) obj).fLayer   = fLayer;

}

//______________________________________________________________________
IlcTARGETsegmentationSSD& IlcTARGETsegmentationSSD::operator=(
    const IlcTARGETsegmentationSSD &source) {
// Operator =
    if (this != &source) {
        source.Copy(*this);
    }
    return *this;
}
//______________________________________________________________________
IlcTARGETsegmentationSSD::IlcTARGETsegmentationSSD(const IlcTARGETsegmentationSSD &source):
        IlcTARGETsegmentation2(source) {
    // copy constructor
    source.Copy(*this);
}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::Init() {
    //cout <<"chiama questo init!!"<<endl;
    // standard initializer
    //cout <<"Sto Chiamando init di SSD segmentazione"<<endl;
    //SetPadSize(fgkPitchDefault,0.);
    //SetNPads(fgkNstripsDefault,0);
    //SetAngles();
}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::Angles(Int_t lay,Float_t &aP,Float_t &aN) const {
    // P and N side stereo angles

    aP = fStereoPl[lay];
    aN = fStereoNl[lay];

}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::SetLayer(Int_t l) {
    //set fLayer data member (only 5 or 6 are allowed)
    fLayer =l;

}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::GetPadTxz(Int_t lay,Float_t &x,Float_t &z) const {
    // returns P and N sided strip numbers for a given location.
    // Transformation from microns detector center local coordinates
    // to detector P and N side strip numbers..
    /*                       _-  Z
                    + angle /    ^
        fNstrips           v     |   N-Side        ...0
            \-------/------------|-----------\--------\
            |\\\\\\/////////////.|\\\\\\\\\\\\\\\\\\\\|
            |0\\\\/////////////..|.\\\\\\\\\\\\\\\\\\\|
            |00\\/////////////...|..\\\\\\\\\\\\\\\\\\|
       X <--|000/////////////... |...\\\\\\\\\\\\\\\\\|
            |00/////////////...  | ...\\\\\\\\\\\\\\\\|
            |0/////////////...   |  ...\\\\\\\\\\\\\\\|
            |//////////////...   |  ...\\\\\\\\\\\\\\\|
            /-----\--------------|--------------------/
        fNstrips-1             P-Side              ...0
                     |0\
                     |00\
    Dead region: |000/
                     |00/
                     |0/
    // expects x, z in microns
    */

//if (Geometry()->GetDetVersion() !=3 ) cout <<"in IlcTARGETsegmentationSSD: Minor Version: "<<Geometry()->GetDetVersion()<<endl;


    Int_t nlayersStrip=fParam->GetNlayersStrip();
    Int_t nendcapsStrip=fParam->GetNendcapsStrip();

    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();

    Float_t stereoP, stereoN;
    Angles(lay-(nlayers+nendcaps),stereoP,stereoN);
    //cout <<"stereoP: "<<stereoP<<"stereoN: "<<stereoN<<endl;
    Float_t tanP = TMath::Tan(stereoP);
    Float_t tanN = TMath::Tan(-stereoN);
    Float_t x1 = x;
    Float_t z1 = z;
    x1 += fDx[lay]/2.;
    z1 += fDz[lay]/2.;
    //dan-commento cout << "pad ..fDx[lay]:" <<fDx[lay]<<" <<fDz[lay]: "<<fDz[lay]<<endl;
    //dan-commento cout << "stereop:" <<stereoP<<" stereon: "<<stereoN<<endl;
    //cout <<"fPitch[lay-(nlayers+nendcaps)]: "<<fPitch[lay-(nlayers+nendcaps)]<<endl;
    x   = (x1 - z1*tanP)/fPitch[lay-(nlayers+nendcaps)];
    //cout <<"P side strip (x): "<<x<<endl;
    //cout <<"fDx: "<<fDx[lay]<<" fDy: "<<fDy[lay]<<" fDz: "<<fDz[lay]<<" fPitch: "<< fPitch[lay-(nlayers+nendcaps)] << endl;
    z   = (x1 - tanN*(z1 - fDz[lay]))/fPitch[lay-(nlayers+nendcaps)];
    //cout <<"N side strip (z): "<<z<<endl;
}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::GetPadIxz(Int_t lay,Float_t x,Float_t z,Int_t &iP,Int_t &iN) const {
    // returns P and N sided strip numbers for a given location.
    /*                       _-  Z
                    + angle /    ^
        fNstrips           v     |   N-Side        ...0
            \-------/------------|-----------\--------\
            |\\\\\\/////////////.|\\\\\\\\\\\\\\\\\\\\|
            |0\\\\/////////////..|.\\\\\\\\\\\\\\\\\\\|
            |00\\/////////////...|..\\\\\\\\\\\\\\\\\\|
       X <--|000/////////////... |...\\\\\\\\\\\\\\\\\|
            |00/////////////...  | ...\\\\\\\\\\\\\\\\|
            |0/////////////...   |  ...\\\\\\\\\\\\\\\|
            |//////////////...   |  ...\\\\\\\\\\\\\\\|
            /-----\--------------|--------------------/
        fNstrips-1             P-Side              ...0
                     |0\
                     |00\
    Dead region: |000/
                     |00/
                     |0/

    // expects x, z in microns
    */

    Float_t stereoP, stereoN;
    Angles(lay,stereoP,stereoN);
    Float_t tanP=TMath::Tan(stereoP);
    Float_t tanN=TMath::Tan(stereoN);
    Float_t x1=x,z1=z;
    x1 += fDx[lay]/2;
    z1 += fDz[lay]/2;
    Float_t  ldX = x1 - z1*tanP;          // distance from left-down edge
    iP = (Int_t)(ldX/fPitch[lay]);
    iP = (iP<0)? -1: iP;
    iP = (iP>fNstrips[lay])? -1: iP;

    ldX = x1 - tanN*(fDz[lay] - z1);
    iN = (Int_t)(ldX/fPitch[lay]);
    iN = (iN<0)? -1: iN;
    iN = (iN>fNstrips[lay])? -1: iN;

}
//-------------------------------------------------------
void IlcTARGETsegmentationSSD::GetPadCxz(Int_t lay,Int_t iP,Int_t iN,Float_t &x,Float_t &z) const {
    // actually this is the GetCrossing(Float_t &,Float_t &)
    // returns local x, z  in microns !

    Float_t lDx = fDx[lay]; // detector size in x direction, microns
    Float_t lDz = fDz[lay]; // detector size in z direction, microns
    Float_t xP; // x coordinate in the P side from the first P strip
    Float_t xN; // x coordinate in the N side from the first N strip
    Float_t stereoP, stereoN;
    Angles(lay,stereoP,stereoN);
    Float_t kP=TMath::Tan(stereoP);
    Float_t kN=TMath::Tan(stereoN);

    xP=iP*fPitch[lay];
    xN=iN*fPitch[lay];
    x = xP + kP*(lDz*kN-xP+xN)/(kP+kN);
    z = (lDz*kN-xP+xN)/(kP+kN);
    x -= lDx/2;
    z -= lDz/2;
    //if(TMath::Abs(z) > Dz/2) cout<<"Warning, wrong z local ="<<z<<endl;
    // Check that zL is inside the detector for the
    // correspondent xP and xN coordinates

    return;
}
//______________________________________________________________________
Bool_t IlcTARGETsegmentationSSD::LocalToDet(Int_t lay,Float_t x,Float_t z,
        Int_t &iP,Int_t &iN) const {
    // Transformation from Geant cm detector center local coordinates
    // to detector P and N side strip numbers..
    /*                       _-  Z
                    + angle /    ^
        fNstrips           v     |   N-Side        ...0
            \-------/------------|-----------\--------\
            |\\\\\\/////////////.|\\\\\\\\\\\\\\\\\\\\|
            |0\\\\/////////////..|.\\\\\\\\\\\\\\\\\\\|
            |00\\/////////////...|..\\\\\\\\\\\\\\\\\\|
       X <--|000/////////////... |...\\\\\\\\\\\\\\\\\|
            |00/////////////...  | ...\\\\\\\\\\\\\\\\|
            |0/////////////...   |  ...\\\\\\\\\\\\\\\|
            |//////////////...   |  ...\\\\\\\\\\\\\\\|
            /-----\--------------|--------------------/
        fNstrips-1             P-Side              ...0
                     |0\
                     |00\
    Dead region: |000/
                     |00/
                     |0/
    */
    const Double_t kconst = 1.0E-04; // convert microns to cm.

    x /= kconst;  // convert to microns
    z /= kconst;  // convert to microns
    Float_t x2=x;
    Float_t z2=z;
    this->GetPadTxz(lay,x2,z2);

    // first for P side
    iP = (Int_t) x2;
    if (iP<0 || iP>=fNstrips[lay]) iP=-1; // strip number must be in range.
    // Now for N side)
    iN = (Int_t) z2;
    if (iN<0 || iN>=fNstrips[lay]) iN=-1; // strip number must be in range.
    return kTRUE;
}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::DetToLocal(Int_t lay,Int_t ix,Int_t iPN,
                                       Float_t &x,Float_t &z) const {
    // Transformation from detector segmentation/cell coordiantes starting
    // from 0. iPN=0 for P side and 1 for N side strip. Returned is z=0.0
    // and the corresponding x value..
    /*                       _-  Z
                    + angle /    ^
        fNstrips           v     |   N-Side        ...0
            \-------/------------|-----------\--------\
            |\\\\\\/////////////.|\\\\\\\\\\\\\\\\\\\\|
            |0\\\\/////////////..|.\\\\\\\\\\\\\\\\\\\|
            |00\\/////////////...|..\\\\\\\\\\\\\\\\\\|
       X <--|000/////////////... |...\\\\\\\\\\\\\\\\\|
            |00/////////////...  | ...\\\\\\\\\\\\\\\\|
            |0/////////////...   |  ...\\\\\\\\\\\\\\\|
            |//////////////...   |  ...\\\\\\\\\\\\\\\|
            /-----\--------------|--------------------/
        fNstrips-1             P-Side              ...0
                     |0\
                     |00\
    Dead region: |000/
                     |00/
                     |0/
    */
    // for strips p-side
    // x = a + b + z*tan(fStereoP); a = Dpx(iP)*(iP+0.5)-dx; b = dz*th;
    // for strips n-side
    // x = a + b + z*tan(fStereoP); a = Dpx(iN)*(iN+0.5)-dx; b = -dz*th;
    const Double_t kconst = 1.0E-04; // convert microns to cm.
    Float_t flag=kconst*Dx(lay); // error value
    Double_t th=0.0,dx,dz,i,a,b=0.0,xb[4],zb[4];
    Float_t stereoP, stereoN;
    Angles(lay,stereoP,stereoN);

    z = 0.0;  // Strip center in z.
    if (iPN<0 || iPN>1) {// if error return full detector size in x.
        x = z = flag;
        return;
    } // end if
    if (ix<0 || ix>=fNstrips[lay]) { // if error return full detector size in x.
        x = z = flag;
        return;
    } // end if
    i  = (Double_t) ix;      // convert to double
    dx = 0.5*kconst*Dx(lay);    // half distance in x in cm
    dz = 0.5*kconst*Dz(lay);    // half distance in z in cm
    a  = kconst*Dpx(lay,0)*(i+0.5)-dx; // Min x value.
    if (iPN==0) { //P-side angle defined backwards.
        th = TMath::Tan(stereoP);
        b  = dz*th;
    } else if (iPN==1) { // N-side
        th = TMath::Tan(-stereoN);
        b  = -dz*th;
    } // end if
    // compute average/center position of the strip.
    xb[0] = +dx;
    if (th!=0.0) zb[0] = (+dx-a-b)/th;
    else zb[0] = 0.0;
    xb[1] = -dx;
    if (th!=0.0) zb[1] = (-dx-a-b)/th;
    else zb[1] = 0.0;
    xb[2] = a+b+dz*th;
    zb[2] = +dz;
    xb[3] = a+b-dz*th;
    zb[3] = -dz;
    x = 0.0;
    z = 0.0;
    for (Int_t j=0;j<4;j++) {
        if (xb[j]>=-dx && xb[j]<=dx && zb[j]>=-dz && zb[j]<=dz) {
            x += xb[j];
            z += zb[j];
        } // end if
    } // end for
    x *= 0.5;
    z *= 0.5;
    return;
}
//----------------------------------------------------------------------
Bool_t IlcTARGETsegmentationSSD::GetCrossing(Int_t lay,Int_t iP,Int_t iN,
        Float_t &x,Float_t &z,
        Float_t c[2][2]) {
    // Given one P side strip and one N side strip, Returns kTRUE if they
    // cross each other and the location of the two crossing strips and
    // their correxlation matrix c[2][2].
    /*                       _-  Z
                    + angle /    ^
        fNstrips           v     |   N-Side        ...0
            \-------/------------|-----------\--------\
            |\\\\\\/////////////.|\\\\\\\\\\\\\\\\\\\\|
            |0\\\\/////////////..|.\\\\\\\\\\\\\\\\\\\|
            |00\\/////////////...|..\\\\\\\\\\\\\\\\\\|
       X <--|000/////////////... |...\\\\\\\\\\\\\\\\\|
            |00/////////////...  | ...\\\\\\\\\\\\\\\\|
            |0/////////////...   |  ...\\\\\\\\\\\\\\\|
            |//////////////...   |  ...\\\\\\\\\\\\\\\|
            /-----\--------------|--------------------/
        fNstrips-1             P-Side              ...0
                     |0\
                     |00\
    Dead region: |000/
                     |00/
                     |0/
       c[2][2] is defined as follows
       /c[0][0]  c[0][1]\ /delta iP\ = /delta x\
       \c[1][0]  c[1][1]/ \delta iN/ = \delta z/
    */
    const Double_t kconst = 1.0E-04; // convert microns to cm.
    Double_t thp,thn,th,dx,dz,p,ip,in;
    Float_t stereoP, stereoN;
    Angles(lay,stereoP,stereoN);

    thp = TMath::Tan(stereoP);
    thn = TMath::Tan(-stereoN);
    th  = thp-thn;
    if (th==0.0) { // parall strips then never cross.
        x = 0.0;
        z = 0.0;
        c[0][0] = c[1][0] = c[0][1] = c[1][1] = 0.0;
        return kFALSE;
    } // end if
    // The strips must cross some place in space.
    ip = (Double_t) iP;       // convert to double now for speed
    in = (Double_t) iN;       // convert to double now for speed
    dx = 0.5*kconst*Dx(lay);     // half distance in x in cm
    dz = 0.5*kconst*Dz(lay);     // half distance in z in cm
    p  = kconst*Dpx(lay,0);      // Get strip spacing/pitch now
    x  = 0.5*p+dx + (p*(in*thp-ip*thn)-2.0*dz*thp*thn)/th;
    z  =(p*(in-ip)-dz*(thp+thn))/th;
    // compute correlations.
    c[0][0] = -thn*p/th; // dx/diP
    c[1][1] = p/th;      // dz/diN
    c[0][1] = p*thp/th;  // dx/diN
    c[1][0] = -p/th;     // dz/diP
    if (x<-dx || x>dx || z<-dz || z>dz) return kFALSE; // crossing is outside
    // of the detector so
    // these strips don't
    // cross.
    return kTRUE;
}

//----------------------------------------------------------------------
void IlcTARGETsegmentationSSD::PrintDefaultParameters() const {
// Print default values for parameters.
// Values specified as static const data members are shown

    //cout<<"fgkDxDefault = "<<fgkDxDefault<<endl;
    //cout<<"fgkDzDefault = "<<fgkDzDefault<<endl;
    //cout<<"fgkDyDefault = "<<fgkDyDefault<<endl;
    //cout<<"fgkPitchDefault = "<<fgkPitchDefault<<endl;
    //cout<<"fgkNstripsDefault = "<<fgkNstripsDefault<<endl;
}
