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

/* $Id: IlcTARGETgeomSPD.cxx,v 1.4 2008/11/07 00:03:53 cgatto Exp $ */

////////////////////////////////////////////////////////////////////////
// This class is for the Silicon Pixel Detector, SPD, specific geometry.
// It is being replaced by IlcTARGETsegmentationSPD class. This file also
// constains classes derived from IlcTARGETgeomSPD which do nothing but
// initilize this one with predefined values.
////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TShape.h>
#include <TMath.h>

#include "IlcLog.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETgeomSPD.h"

ClassImp(IlcTARGETgeomSPD)

IlcTARGETgeomSPD::IlcTARGETgeomSPD(){
// Default Constructor. Set everthing to null.

    fShapeSPD    = 0;
    fNbinx       = 0;
    fNbinz       = 0;
    fLowBinEdgeX = 0;
    fLowBinEdgeZ = 0;
//cout<<"Sono in IlcTARGETgeomSPD() 1"<<endl;
}
//______________________________________________________________________
IlcTARGETgeomSPD::IlcTARGETgeomSPD(Double_t dy,Int_t nx,Double_t *bx,
			     Int_t nz,Double_t *bz){
// Standard Constructor. Set everthing to null.

    fShapeSPD    = 0;
    fNbinx       = 0;
    fNbinz       = 0;
    fLowBinEdgeX = 0;
    fLowBinEdgeZ = 0;
    ReSetBins(dy,nx,bx,nz,bz);
    return;
//cout<<"Sono in IlcTARGETgeomSPD() 2"<<endl;
}
void IlcTARGETgeomSPD::ReSetBins(Double_t ,Int_t ,Float_t *,
			      Int_t ,Float_t *){
  IlcFatal("Not implemented");
}
//______________________________________________________________________
void IlcTARGETgeomSPD::ReSetBins(Double_t dy,Int_t nx,Double_t *bx,
			      Int_t nz,Double_t *bz){
// delets the contents of this and replaces it with the given values.
    Int_t i;
    Double_t dx = 0.0, dz = 0.0;
//cout<<"Sono in IlcTARGETgeomSPD() 3"<<endl;
    // Compute size in x and z (based on bins).
    for(i=0;i<nx;i++) dx += bx[i];
    for(i=0;i<nz;i++) dz += bz[i];
    dx *= 0.5;
    dz *= 0.5;

    delete fShapeSPD; // delete existing shape

    SetNbinX(nx);
    SetNbinZ(nz);
    InitLowBinEdgeX();
    InitLowBinEdgeZ();
    fLowBinEdgeX[0] = -dx;
    fLowBinEdgeZ[0] = -dz;
    double dxx=-dx,dzz=-dz;
    for(i=0;i<nx;i++) {dxx+=bx[i]; fLowBinEdgeX[i+1] = dxx;}
    for(i=0;i<nz;i++) {dzz+=bz[i]; fLowBinEdgeZ[i+1] = dzz;}
// // // //     IlcInfo(Form("*** %f %f %f", dx,dy,dz));
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",dx,dy,dz);
    return;
}
//______________________________________________________________________
IlcTARGETgeomSPD::IlcTARGETgeomSPD(IlcTARGETgeomSPD &source) : TObject(source){
    // Copy constructor
//cout<<"Sono in IlcTARGETgeomSPD() 4"<<endl;
    *this = source; // just use the = operator for now.
    return;
}
//______________________________________________________________________
IlcTARGETgeomSPD& IlcTARGETgeomSPD::operator=(IlcTARGETgeomSPD &source){
    // = operator
    Int_t i;
//cout<<"Sono in IlcTARGETgeomSPD() 5"<<endl;
    if(&source == this) return *this;
    this->fShapeSPD = new TBRIK(*(source.fShapeSPD));
    if(this->fLowBinEdgeX) delete[] this->fLowBinEdgeX;
    if(this->fLowBinEdgeZ) delete[] this->fLowBinEdgeZ;
    this->fNbinx = source.fNbinx;
    this->fNbinz = source.fNbinz;
    this->InitLowBinEdgeX();
    this->InitLowBinEdgeZ();
    for(i=0;i<fNbinx;i++) this->fLowBinEdgeX[i] = source.fLowBinEdgeX[i];
    for(i=0;i<fNbinz;i++) this->fLowBinEdgeZ[i] = source.fLowBinEdgeZ[i];
    return *this;
}
//______________________________________________________________________
IlcTARGETgeomSPD::~IlcTARGETgeomSPD(){
// Destructor
//cout<<"Sono in IlcTARGETgeomSPD() 6"<<endl;
    delete fShapeSPD;
    if(this->fLowBinEdgeX) delete[] this->fLowBinEdgeX;
    if(this->fLowBinEdgeZ) delete[] this->fLowBinEdgeZ;
    fShapeSPD    = 0;
    fNbinx       = 0;
    fNbinz       = 0;
    fLowBinEdgeX = 0;
    fLowBinEdgeZ = 0;
}
//______________________________________________________________________
void IlcTARGETgeomSPD::LToDet(Double_t xl,Double_t zl,Int_t &row,Int_t &col){
// Returns the row and column pixel numbers for a given local coordinate
// system. If they are outside then it will return -1 or fNbinx/z.
    Int_t i;
//cout<<"Sono in IlcTARGETgeomSPD() 7"<<endl;
    if(xl<fLowBinEdgeX[0]) row = -1;
    else{
	for(i=0;i<fNbinx;i++) if(xl<=fLowBinEdgeX[i]) break;
	row = i;
    } //end if too low.
    if(zl<fLowBinEdgeX[0]) col = -1;
    else{
	for(i=0;i<fNbinz;i++) if(zl<=fLowBinEdgeZ[i]) break;
	col = i;
    } //end if too low.
    return;
}
//______________________________________________________________________
void IlcTARGETgeomSPD::DetToL(Int_t row,Int_t col,Double_t &xl,Double_t &zl){
// returns the pixel center local coordinate system location for a given
// row and column number. It the row or column number is outside of the 
// defined range then it will return the nearest detector edge.
//cout<<"Sono in IlcTARGETgeomSPD() 8"<<endl;
    if(row>=0||row<fNbinx-1) xl = 0.5*(fLowBinEdgeX[row]+fLowBinEdgeX[row+1]);
    else if(row<0) xl = fLowBinEdgeX[0];else xl = fLowBinEdgeX[fNbinx-1];
    if(col>=0||col<fNbinz-1) zl = 0.5*(fLowBinEdgeZ[col]+fLowBinEdgeZ[col+1]);
    else if(col<0) zl = fLowBinEdgeZ[0];else zl = fLowBinEdgeZ[fNbinz-1];
    return;
}
//______________________________________________________________________
void IlcTARGETgeomSPD::Print(ostream *os) const {
// Standard output format for this class
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
//cout<<"Sono in IlcTARGETgeomSPD() 9"<<endl;
    fmt = os->setf(ios::scientific); // set scientific floating point output
    *os << "TBRIK" << " ";
    *os << setprecision(16) << GetDx() << " ";
    *os << setprecision(16) << GetDy() << " ";
    *os << setprecision(16) << GetDz() << " ";
    *os << fNbinx-1 << " " << fNbinz-1 << " ";
    for(i=0;i<fNbinx;i++) *os << setprecision(16) << fLowBinEdgeX[i] << " ";
    for(i=0;i<fNbinz;i++) *os << setprecision(16) << fLowBinEdgeZ[i] << " ";
    *os << endl;
    os->flags(fmt);
    return;
}
//______________________________________________________________________
void IlcTARGETgeomSPD::Read(istream *is){
// Standard input format for this class
    Int_t i,j;
    Double_t dx,dy,dz;
    char shape[20];
//cout<<"Sono in IlcTARGETgeomSPD() 10"<<endl;

    for(i=0;i<20;i++) shape[i]='\0';
    *is >> shape;
    if(strcmp(shape,"TBRIK")) Warning("::Read","Shape not a TBRIK");
    *is >> dx >> dy >> dz;
    if(fShapeSPD!=0) delete fShapeSPD;
    IlcFatal("");SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",dx,dy,dz);
    *is >> i >> j;
    SetNbinX(i);
    SetNbinZ(j);
    InitLowBinEdgeX();
    InitLowBinEdgeZ();
    for(i=0;i<fNbinx;i++) *is >> fLowBinEdgeX[i];
    for(i=0;i<fNbinz;i++) *is >> fLowBinEdgeZ[i];
    return;
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPD &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 11"<<endl;
    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPD &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 12"<<endl;
    r.Read(&is);
    return is;
}
//=====================================================================

ClassImp(IlcTARGETgeomSPD300)

IlcTARGETgeomSPD300::IlcTARGETgeomSPD300() : IlcTARGETgeomSPD(){
//cout<<"Sono in IlcTARGETgeomSPD() 13"<<endl;
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET TDR geometry. This only consists of
// a default constructor to construct the defalut TDR SPD detector geometry
// 256 X 279 300 by 50 micron pixels.
////////////////////////////////////////////////////////////////////////
const Double_t kdx=0.6400,kdy=0.0075,kdz=4.1900; // cm; Standard pixel detector
                                                // size is 2dx wide, 2dz long,
                                                // and 2dy thick. Geant 3.12
                                                // units.
const Double_t kbinx0 = 0.0050; // cm; Standard pixel size in x direction.
const Int_t   knbinx = 256;    // number of pixels along x direction.
const Double_t kbinz0 = 0.0300; // cm; Standard pixel size in z direction.
const Double_t kbinz1 = 0.0350; // cm; Edge pixel size in z direction.
const Int_t   knbinz = 279;    // number of pixels along z direction.
    Int_t i;
    Double_t dx=0.0,dz=0.0;

//    cout << "IlcTARGETgeomSPD300 default creator called: start" << endl;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    for(i=0;i<knbinx;i++) dx += kbinx0; // Compute size x.
    dx *= 0.5;
    for(i=0;i<knbinz;i++) dz += kbinz0; // Compute size z.
    dz += 2.0*(kbinz1-kbinz0);
    dz *= 0.5;
    InitLowBinEdgeX();
    InitLowBinEdgeZ();
    SetLowBinEdgeX(0,-dx); // Starting position X
    for(i=0;i<knbinx;i++) SetLowBinEdgeX(i+1,GetBinLowEdgeX(i)+kbinx0);
    SetLowBinEdgeZ(0,-dz); // Starting position z
    SetLowBinEdgeZ(1,GetBinLowEdgeZ(0)+kbinz1);
    for(i=1;i<knbinz;i++) SetLowBinEdgeZ(i+1,GetBinLowEdgeZ(i)+kbinz0);
    SetLowBinEdgeZ(knbinz,GetBinLowEdgeZ(knbinz-1)+kbinz1);

    if(TMath::Abs(dx-kdx)>1.0E-4 || TMath::Abs(dz-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");
    IlcFatal("");SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",dx,kdy,dz);
//    cout << "IlcTARGETgeomSPD300 default creator called: end" << endl;
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPD300 &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 14"<<endl;
    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPD300 &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//=====================================================================

ClassImp(IlcTARGETgeomSPD20Short)

IlcTARGETgeomSPD20Short::IlcTARGETgeomSPD20Short() : IlcTARGETgeomSPD(){
//cout<<"Sono in IlcTARGETgeomSPD() 15"<<endl;
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
//----------------------------------------------------------------------
IlcTARGETgeomSPD20Short::IlcTARGETgeomSPD20Short(Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
//cout<<"Sono in IlcTARGETgeomSPD() 16"<<endl;
////////////////////////////////////////////////////////////////////////
//    Standard constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////

    const Double_t kdx=0.455/*,kdy=0.015*/,kdz=6.250; // cm; Standard pixel
                                                      // detector size is 2dx
                                                      //  wide, 2dz long, and
                                                      //  2dy thick. Geant 3.12
                                                      // units.
    const Double_t kbinx0 = 0.0020; // cm; Standard pixel size in x direction.
    const Int_t   knbinx = 455;    // number of pixels along x direction.
    const Double_t kbinz0 = 0.0020; // cm; Standard pixel size in z direction.
    //const Double_t kbinz1 = 0.0625; // cm; Special pixel size in z direction.
    const Int_t   knbinz = 6250;    // number of pixels along z direction.
    Int_t i;
    Double_t dx,dz,*binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    /*binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;
*/
    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;

    if(npar<3){
	Error("IlcTARGETgeomSPD20Short",
              "npar=%d<3 array par must be at least [3] or larger",npar);
	return;
    } // end if
    IlcFatal("");SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",
	     par[0],par[1],par[2]);
    if(TMath::Abs(dx-kdx)>1.0E-4 || TMath::Abs(dz-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    SetLowBinEdgeX(0,-dx);
    SetLowBinEdgeZ(0,-dz);
    for(i=0;i<knbinx;i++) SetLowBinEdgeX(i+1,GetBinLowEdgeX(i)+binSizeX[i]);
    for(i=0;i<knbinz;i++) SetLowBinEdgeZ(i+1,GetBinLowEdgeZ(i)+binSizeZ[i]);
    delete[] binSizeX;
    delete[] binSizeZ;
}

//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPD20Short &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPD20Short &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 17"<<endl;
    r.Read(&is);
    return is;
}
//======================================================================

ClassImp(IlcTARGETgeomSPDPixel)

IlcTARGETgeomSPDPixel::IlcTARGETgeomSPDPixel() : IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
//----------------------------------------------------------------------
IlcTARGETgeomSPDPixel::IlcTARGETgeomSPDPixel(Int_t lay,IlcTARGETgeom *geom,Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    Standard constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 18"<<endl;
  IlcFatal("Not implemented");
//cout<<"Ho chiamato il metodo IlcTARGETgeomSPDPixel"<<endl;
    Float_t pixelx,pixely,pixelz;
    const Double_t kconv=10000.;
    const Double_t kconv2=0.0001;

    geom->GetPixelDimension(lay,pixelx,pixely,pixelz);
 //   cout <<"1Segm... pixelx: "<<pixelx<< " pixelz: "<<pixelz<<endl;
 //   cout <<"1Segm Conv... pixelx_conv: "<<pixelx*kconv2<< " pixelz_conv: "<<pixelz*kconv2<<endl;
    const Double_t kdx=par[0]/*,kdy=0.015*/,kdz=par[2]; // cm; Standard pixel
           // detector size is 2dx
                                                      //  wide, 2dz long, and
                                                      //  2dy thick. Geant 3.12
                                                      // units.
//cout<<"layer "<<lay  <<endl;                                           
//cout<<"kdx "<<kdx  <<endl;                                           
//cout<<"kdy "<<kdx  <<endl;                                           
//cout<<"kdz "<<kdz  <<endl;                                           
    const Double_t kbinx0 = pixelx*kconv2; // cm; Standard pixel size in x direction.

    const Int_t   knbinx = (Int_t)((kdx*2.*kconv)/pixelx+0.01);    // number of pixels along x direction.
//cout<<"Numero di Pixel lungo X "<<knbinx<<endl;

    geom->SetNPixelX(lay,knbinx);
    const Double_t kbinz0 = pixelz*kconv2; // cm; Standard pixel size in z direction.
    //const Double_t kbinz1 = 0.0625; // cm; Special pixel size in z direction.
    const Int_t   knbinz = (Int_t)((kdz*2.*kconv)/pixelz+0.01);    // number of pixels along z direction.

//cout<<"Numero di Pixel lungo Z "<<knbinz<<endl;

    geom->SetNPixelZ(lay,knbinz);

    Int_t i;
    Double_t dx,dz,*binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    /*binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;
*/
    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;

    if(npar<3){
	Error("IlcTARGETgeomSPDPixel",
              "npar=%d<3 array par must be at least [3] or larger",npar);
	return;
    } // end if
//    IlcDebug(1,Form("lay:%d ,par[0]:%f ,par[1]:%f ,par[2]:%f", lay, par[0], par[1], par[2]));
    IlcFatal("");SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",
	     2.*par[6]*TMath::Tan(par[1]/2.*TMath::DegToRad()), par[6]-par[5], par[7]-par[4]);
    IlcInfo(Form("%f %f %f", 2.*par[6]*TMath::Tan(par[1]/2.*TMath::DegToRad()), par[6]-par[5], par[7]-par[4]));
    if(TMath::Abs(2.*par[6]*TMath::Tan(par[1]/2.*TMath::DegToRad())-kdx)>1.0E-4 || TMath::Abs(par[7]-par[4]-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");

    //cout << "Segmenta... kconv:"<<kconv<<" pixelx:"<<pixelx<< " kdx:"<<kdx<<"  pixelz:"<< pixelz<<" kdz:"<<kdz<<" par[0]:"<<par[0]<<" par[2]:"<<par[2]<<"  kbinx0:"<<kbinx0<<" kbinz0:"<<kbinz0<<" knbinx:"<<knbinx<<" knbinz:"<<knbinz<<endl;
    

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    //SetLowBinEdgeX(0,-par[0]);
    //SetLowBinEdgeZ(0,-par[2]);
    for(i=0;i<knbinx;i++) {
    SetLowBinEdgeX(i,-par[0]+i*kbinx0);
    //cout << "2Segmenta... GetBinLowEdgeX(i):"<<GetBinLowEdgeX(i)<<" kbinx0:" << kbinx0 <<endl; 


}
    for(i=0;i<knbinz;i++){
    
    SetLowBinEdgeZ(i,-par[2]+i*kbinz0);
//cout << "2Segmenta... GetBinLowEdgeZ(i):"<<GetBinLowEdgeZ(i)<<" kbinz0:" << kbinz0 <<endl; 

  }
    ReSetBins(par[1],knbinx,binSizeX,knbinz,binSizeZ);
    delete[] binSizeX;
    delete[] binSizeZ;

}//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPDPixel &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPDPixel&r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================

//-------------------->
//======================================================================

ClassImp(IlcTARGETgeomSPDFiberSegm)

IlcTARGETgeomSPDFiberSegm::IlcTARGETgeomSPDFiberSegm() : IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
//----------------------------------------------------------------------
IlcTARGETgeomSPDFiberSegm::IlcTARGETgeomSPDFiberSegm(Int_t lay,IlcTARGETgeom *geom,Int_t npar,Float_t *par) : IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    Standard constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 18"<<endl;

    
  if(npar<3){
	Error("IlcTARGETgeomSPDFiberSegm",
              "npar=%d<3 array par must be at least [3] or larger",npar);
	return;
  } // end if
//   IlcInfo(Form("npar: %d par: %f %f %f", npar, par[0], par[1], par[2]));

    Float_t pixelx,pixely,pixelz;
    const Double_t kconv=10000.;
    const Double_t kconv2=0.0001;

    geom->GetPixelDimension(lay,pixelx,pixely,pixelz);
//    cout <<"1Segm... pixelx: "<<pixelx<< " pixelz: "<<pixelz<<endl;
 //   cout <<"1Segm Conv... pixelx_conv: "<<pixelx*kconv2<< " pixelz_conv: "<<pixelz*kconv2<<endl;

   Double_t kdx_tpm=-1., kdy_tpm=-1., kdz_tpm=-1.;
   if(geom->GetDetVersion()==1){  //square fiber version
     kdx_tpm = par[0];
     kdy_tpm = par[1];
     kdz_tpm = par[2];
  }
  else if(geom->GetDetVersion()==2){  //poligonal version
    kdx_tpm = TMath::Pi()*par[6]/((lay==0)?1.:24.);
    kdy_tpm = (par[6]-par[5])/2.;
    kdz_tpm = (par[7]-par[4])/2.;
  }

   const Double_t kdx= kdx_tpm;
   const Double_t kdz= kdz_tpm;
                                // cm;  // detector size is 2dx
                                       //  wide, 2dz long, and
                                       //  2dy thick. Geant 3.12
                                       // units.
// cout<<"layer "<<lay;
// cout<<" kdx "<<kdx;
//cout<<" kdy "<<kdx;
// cout<<" kdz "<<kdz  <<endl;
    const Double_t kbinx0 = pixelx*kconv2; // cm; Standard pixel size in x direction.

    const Int_t   knbinx = (Int_t)((kdx*2.*kconv)/pixelx+0.01);    // number of pixels along x direction.
//     cout<<"Numero di Pixel lungo X "<<knbinx<<endl;

    geom->SetNPixelX(lay,knbinx);
    const Double_t kbinz0 = pixelz*kconv2; // cm; Standard pixel size in z direction.

    const Int_t   knbinz = (Int_t)((kdz*2.*kconv)/pixelz+0.01);    // number of pixels along z direction.
//     cout<<"Numero di Pixel lungo Z "<<knbinz<<endl;

//     IlcInfo(Form("kbinx0: %f kbinz0: %f", kbinx0, kbinz0));

    geom->SetNPixelZ(lay,knbinz);

    Int_t i;
    Double_t dx,dz,*binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.

    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;

    //    IlcDebug(1,Form("lay:%d ,par[0]:%f ,par[1]:%f ,par[2]:%f", lay, par[0], par[1], par[2]));
//     IlcInfo(Form("lay:%d ,par[0]:%f ,par[1]:%f ,par[2]:%f", lay, par[0], par[1], par[2]));
    
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET", kdx_tpm, kdy_tpm, kdz_tpm);
    

// // // //     IlcInfo(Form("*** %f %f %f", TMath::Pi()*par[6]/((lay==0)?1.:24.), (par[6]-par[5])/2., (par[7]-par[4])/2.));
    
    if( (geom->GetDetVersion()==1 && (TMath::Abs(par[0]-kdx)>1.0E-4 || TMath::Abs(par[2]-kdz)>1.0E-4)) || (geom->GetDetVersion()==2 && (TMath::Abs(TMath::Pi()*par[6]/((lay==0)?1.:24.)-kdx)>1.0E-4 || TMath::Abs((par[7]-par[4])/2.-kdz)>1.0E-4)) ) 
	Warning("Default Creator","Detector size may be wrong.");

//     cout << "Segmenta... kconv:"<<kconv<<" pixelx:"<<pixelx<< " kdx:"<<kdx<<"  pixelz:"<< pixelz<<" kdz:"<<kdz<<" par[0]:"<<par[0]<<" par[2]:"<<par[2]<<"  kbinx0:"<<kbinx0<<" kbinz0:"<<kbinz0<<" knbinx:"<<knbinx<<" knbinz:"<<knbinz<<endl;
    

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    //SetLowBinEdgeX(0,-par[0]);
    //SetLowBinEdgeZ(0,-par[2]);
    for(i=0;i<knbinx+1;i++) {
      SetLowBinEdgeX(i,-kdx_tpm+i*kbinx0);
      //cout << "2Segmenta... GetBinLowEdgeX(i):"<<GetBinLowEdgeX(i)<<" kbinx0:" << kbinx0 <<endl; 
    }

    for(i=0;i<knbinz+1;i++){
      SetLowBinEdgeZ(i,-kdz_tpm+i*kbinz0);
      //cout << "2Segmenta... GetBinLowEdgeZ(i):"<<GetBinLowEdgeZ(i)<<" kbinz0:" << kbinz0 <<endl; 
    }

    ReSetBins(kdy_tpm,knbinx,binSizeX,knbinz,binSizeZ);

    
    delete[] binSizeX;
    delete[] binSizeZ;

}



//-------------------->
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPDFiberSegm &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPDFiberSegm&r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================

ClassImp(IlcTARGETgeomSPDPixelEnd)

IlcTARGETgeomSPDPixelEnd::IlcTARGETgeomSPDPixelEnd() : IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
//----------------------------------------------------------------------
IlcTARGETgeomSPDPixelEnd::IlcTARGETgeomSPDPixelEnd(Int_t lay,IlcTARGETgeom *geom,Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 19"<<endl;
//    Standard constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
IlcFatal("Not implemented");
    Float_t pixelx,pixely,pixelz;
    const Double_t kconv=10000.;
    const Double_t kconv2=0.0001;
//cout<<"Ho chiamato il metodo IlcTARGETgeomSPDPixelEnd"<<endl;
    geom->GetPixelDimension(lay,pixelx,pixely,pixelz);
    //cout <<"1Segm.End.. pixelx: "<<pixelx<< " pixelz: "<<pixelz<<endl;
    //cout <<"1Segm Conv.End.. pixelx_conv: "<<pixelx*kconv2<< " pixelz_conv: "<<pixelz*kconv2<<endl;
    const Double_t kdx=par[0]/*,kdy=0.015*/,kdz=par[4]; // cm; Standard pixel
                                                      // detector size is 2dx
                                                      //  wide, 2dz long, and
                                                      //  2dy thick. Geant 3.12
                                                      // units.

//cout<<"layer "<<lay  <<endl;                                           
//cout<<"kdx "<<kdx  <<endl;                                           
//cout<<"kdz "<<kdz  <<endl;                                           

    const Double_t kbinx0 = pixelx*kconv2; // cm; Standard pixel size in x direction.
    const Int_t   knbinx = (Int_t)((kdx*2.*kconv)/pixelx+0.01);    // number of pixels along x direction.
//cout << "number pixels lungo X: End " << knbinx << endl;

    geom->SetNPixelX(lay,knbinx);
    const Double_t kbinz0 = pixelz*kconv2; // cm; Standard pixel size in z direction.
    /**/const Int_t knbinz = (Int_t)((kdz*2.*kconv)/pixelz+0.01);
    /*if (lay==5 || lay==6 || lay==9 || lay==10)
    { 
    knbinz =knbinz+1;    // number of pixels along z direction.
   }

else if (lay==7 || lay==8 || lay==11 || lay==12)
{
knbinz=knbinz;
}*/

//cout << "number pixels lungo Z End: " << knbinz << endl;
    geom->SetNPixelZ(lay,knbinz);

    Int_t i;
    Double_t dx,dz;
    Double_t *binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    /*binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;
*/
    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;

    if(npar<3){
	Error("IlcTARGETgeomSPDPixelEnd",
              "npar=%d<3 array par must be at least [3] or larger",npar);
	return;
    } // end if
//    IlcDebug(1,Form("lay:%d ,par[0]:%f ,par[1]:%f ,par[2]:%f", lay, par[0], par[1], par[2]));
    IlcFatal("");
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",
	     par[0],par[2],par[4]);
    if(TMath::Abs(par[0]-kdx)>1.0E-4 || TMath::Abs(par[4]-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");

   // cout << "Segmenta..End. kconv:"<<kconv<<" pixelx:"<<pixelx<< " kdx:"<<kdx<<"  pixelz:"<< pixelz<<" kdz:"<<kdz<<" par[0]:"<<par[0]<<" par[2]:"<<par[2]<<"  kbinx0:"<<kbinx0<<" kbinz0:"<<kbinz0<<" knbinx:"<<knbinx<<" knbinz:"<<knbinz<<endl;
    

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    //SetLowBinEdgeX(0,-par[0]);
    //SetLowBinEdgeZ(0,-par[2]);
    for(i=0;i<knbinx;i++) {
    SetLowBinEdgeX(i,-par[0]+i*kbinx0);
    //cout << "2Segmenta... GetBinLowEdgeX(i):"<<GetBinLowEdgeX(i)<<" kbinx0:" << kbinx0 <<endl; 


}
    for(i=0;i<knbinz;i++){
    
    SetLowBinEdgeZ(i,-par[4]+i*kbinz0);
//cout << "2Segmenta... GetBinLowEdgeZ(i):"<<GetBinLowEdgeZ(i)<<" kbinz0:" << kbinz0 <<endl; 

  }
    ReSetBins(par[2],knbinx,binSizeX,knbinz,binSizeZ);
    delete[] binSizeX;
    delete[] binSizeZ;

}

//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPDPixelEnd &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPDPixelEnd &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================

//---------------->
//======================================================================

ClassImp(IlcTARGETgeomSPDPixelEndv4th)

IlcTARGETgeomSPDPixelEndv4th::IlcTARGETgeomSPDPixelEndv4th() : IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
IlcTARGETgeomSPDPixelEndv4th::IlcTARGETgeomSPDPixelEndv4th(Int_t lay,IlcTARGETgeom *geom,Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 19"<<endl;
//    Standard constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
    IlcFatal("Not implemented");
    Float_t pixelx,pixely,pixelz;
    const Double_t kconv=10000.;
    const Double_t kconv2=0.0001;
//cout<<"Ho chiamato il metodo IlcTARGETgeomSPDPixelEndv4th"<<endl;
    geom->GetPixelDimension(lay,pixelx,pixely,pixelz);
    //cout <<"1Segm.End.. pixelx: "<<pixelx<< " pixelz: "<<pixelz<<endl;
    //cout <<"1Segm Conv.End.. pixelx_conv: "<<pixelx*kconv2<< " pixelz_conv: "<<pixelz*kconv2<<endl;
    const Double_t kdx=par[0]/*,kdy=0.015*/,kdz=par[4]; // cm; Standard pixel
                                                      // detector size is 2dx
                                                      //  wide, 2dz long, and
                                                      //  2dy thick. Geant 3.12
                                                      // units.

//cout<<"layer "<<lay  <<endl;                                           
//cout<<"kdx "<<kdx  <<endl;                                           
//cout<<"kdz "<<kdz  <<endl;                                           

    const Double_t kbinx0 = pixelx*kconv2; // cm; Standard pixel size in x direction.
    const Int_t   knbinx = (Int_t)((kdx*2.*kconv)/pixelx+0.01);    // number of pixels along x direction.
//cout << "number pixels lungo X: End " << knbinx << endl;

    geom->SetNPixelX(lay,knbinx);
    const Double_t kbinz0 = pixelz*kconv2; // cm; Standard pixel size in z direction.
    const Int_t knbinz = (Int_t)(((kdz*2.*kconv)/pixelz)+0.01);
    //const Int_t knbinz = ((kdz*2.*kconv)/pixelz);
    /*if (lay==5 || lay==6 || lay==9 || lay==10)
    { 
    knbinz =knbinz+1;    // number of pixels along z direction.
   }

else if (lay==7 || lay==8 || lay==11 || lay==12)
{
knbinz=knbinz;
}*/

//cout << "number pixels lungo Z End: " << knbinz << endl;
    geom->SetNPixelZ(lay,knbinz);

    Int_t i;
    Double_t dx,dz;
    Double_t *binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    /*binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;
*/
    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;

    if(npar<3){
	Error("IlcTARGETgeomSPDPixelEndv4th",
              "npar=%d<3 array par must be at least [3] or larger",npar);
	return;
    } // end if
//    IlcDebug(1,Form("lay:%d ,par[0]:%f ,par[1]:%f ,par[2]:%f", lay, par[0], par[1], par[2]));
    IlcFatal("");
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",
	     par[0],par[2],par[4]);
    if(TMath::Abs(par[0]-kdx)>1.0E-4 || TMath::Abs(par[4]-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");

   // cout << "Segmenta..End. kconv:"<<kconv<<" pixelx:"<<pixelx<< " kdx:"<<kdx<<"  pixelz:"<< pixelz<<" kdz:"<<kdz<<" par[0]:"<<par[0]<<" par[2]:"<<par[2]<<"  kbinx0:"<<kbinx0<<" kbinz0:"<<kbinz0<<" knbinx:"<<knbinx<<" knbinz:"<<knbinz<<endl;
    

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    //SetLowBinEdgeX(0,-par[0]);
    //SetLowBinEdgeZ(0,-par[2]);
    for(i=0;i<knbinx;i++) {
    SetLowBinEdgeX(i,-par[0]+i*kbinx0);
    //cout << "2Segmenta... GetBinLowEdgeX(i):"<<GetBinLowEdgeX(i)<<" kbinx0:" << kbinx0 <<endl; 


}
    for(i=0;i<knbinz;i++){
    
    SetLowBinEdgeZ(i,-par[4]+i*kbinz0);
//cout << "2Segmenta... GetBinLowEdgeZ(i):"<<GetBinLowEdgeZ(i)<<" kbinz0:" << kbinz0 <<endl; 

  }
    ReSetBins(par[2],knbinx,binSizeX,knbinz,binSizeZ);
    delete[] binSizeX;
    delete[] binSizeZ;

}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPDPixelEndv4th &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPDPixelEndv4th &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================


//--------------->

ClassImp(IlcTARGETgeomSPD20Long)

IlcTARGETgeomSPD20Long::IlcTARGETgeomSPD20Long() : IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}

IlcTARGETgeomSPD20Long::IlcTARGETgeomSPD20Long(Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
////////////////////////////////////////////////////////////////////////
//    Standard constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
//cout<<"Sono in IlcTARGETgeomSPD() 20"<<endl;
npar=0;
par=0x0;
    const Double_t kdx=0.665000/*,kdy=0.015*/,kdz=6.250000; // cm; Standard pixel
                                                      // detector size is 2dx
                                                      //  wide, 2dz long, and
                                                      //  2dy thick. Geant 3.12
                                                      // units.
    const Double_t kbinx0 = 0.0020; // cm; Standard pixel size in x direction.
    const Int_t   knbinx = 665;    // number of pixels along x direction.
    const Double_t kbinz0 = 0.0020; // cm; Standard pixel size in z direction.
    //const Double_t kbinz1 = 0.0625; // cm; Special pixel size in z direction.
    const Int_t   knbinz = 6250;    // number of pixels along z direction.
    Int_t i;
    Double_t *binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    /*binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;
*/
    // correct detector size for bin size.
/*    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;*/

    if(npar<3){
	Error("IlcTARGETgeomSPD20Long",
              "npar=%d<3 array par must be at least [3] or larger",npar);
	return;
    } // end if
    IlcFatal("");
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",
	     par[0],par[1],par[2]);
/*IlcDebug(1,Form("dx=%f, kdx=%f",dx,kdx));
IlcDebug(1,Form("dz=%f, kdz=%f",dz,kdz));
    if(TMath::Abs(dx-kdx)>1.0E-4 || TMath::Abs(dz-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");*/

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    SetLowBinEdgeX(0,-kdx);
    SetLowBinEdgeZ(0,-kdz);
    for(i=0;i<knbinx;i++) SetLowBinEdgeX(i+1,GetBinLowEdgeX(i)+binSizeX[i]);
    for(i=0;i<knbinz;i++) SetLowBinEdgeZ(i+1,GetBinLowEdgeZ(i)+binSizeZ[i]);
}

//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPD20Long &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPD20Long &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================

ClassImp(IlcTARGETgeomSPD425Long)

IlcTARGETgeomSPD425Long::IlcTARGETgeomSPD425Long(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
IlcTARGETgeomSPD425Long::IlcTARGETgeomSPD425Long(Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
//cout<<"Sono in IlcTARGETgeomSPD() 21"<<endl;
npar=0;
par=0x0;
    const Double_t kdx=0.6400,kdy=0.0075,kdz=4.2650; // cm; Standard pixel
                                                    // detector size is 2dx
                                                    //  wide, 2dz long, and
                                                    //  2dy thick. Geant 3.12
                                                    // units.
    const Double_t kbinx0 = 0.0050; // cm; Standard pixel size in x direction.
    const Int_t   knbinx = 256;    // number of pixels along x direction.
    const Double_t kbinz0 = 0.0425; // cm; Standard pixel size in z direction.
    const Double_t kbinz1 = 0.0625; // cm; Special pixel size in z direction.
    const Int_t   knbinz = 192;    // number of pixels along z direction.
    Int_t i;
    Double_t dx,dz,*binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;

    binSizeZ[159] = kbinz1;
    binSizeZ[160] = kbinz1;

    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;
    IlcFatal("");
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",dx,kdy,dz);
    if(TMath::Abs(dx-kdx)>1.0E-4 || TMath::Abs(dz-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    SetLowBinEdgeX(0,-dx);
    SetLowBinEdgeZ(0,-dz);
    for(i=0;i<knbinx;i++) SetLowBinEdgeX(i+1,GetBinLowEdgeX(i)+binSizeX[i]);
    for(i=0;i<knbinz;i++) SetLowBinEdgeZ(i+1,GetBinLowEdgeZ(i)+binSizeZ[i]);
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPD425Long &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPD425Long &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================
ClassImp(IlcTARGETgeomSPD425Short)

IlcTARGETgeomSPD425Short::IlcTARGETgeomSPD425Short(){
////////////////////////////////////////////////////////////////////////
//    default constructor, for TARGET post TDR geometry. This only consists of
// a default constructor to construct the defalut post TDR SPD detector 
// geometry 256 X 197 425 by 50 micron pixels with interleaved 625 by 50
// micron pixels (large detector).
////////////////////////////////////////////////////////////////////////
}
IlcTARGETgeomSPD425Short::IlcTARGETgeomSPD425Short(Int_t npar,Float_t *par) :
                                                              IlcTARGETgeomSPD(){
//cout<<"Sono in IlcTARGETgeomSPD() 22"<<endl;
npar=0;
par=0x0;
    const Double_t kdx=0.6400,kdy=0.0075,kdz=4.2650; // cm; Standard pixel
                                                    // detector size is 2dx
                                                    //  wide, 2dz long, and
                                                    //  2dy thick. Geant 3.12
                                                    // units.
    const Double_t kbinx0 = 0.0050; // cm; Standard pixel size in x direction.
    const Int_t   knbinx = 256;    // number of pixels along x direction.
    const Double_t kbinz0 = 0.0425; // cm; Standard pixel size in z direction.
    const Double_t kbinz1 = 0.0625; // cm; Special pixel size in z direction.
    const Int_t   knbinz = 192;    // number of pixels along z direction.
    Int_t i;
    Double_t dx,dz,*binSizeX,*binSizeZ;

    SetNbinX(knbinx); // default number of bins in x.
    SetNbinZ(knbinz); // default number of bins in z.

    binSizeX = new Double_t[knbinx]; // array of bin sizes along x.
    for(i=0;i<knbinx;i++) binSizeX[i] = kbinx0; // default x bin size.
    binSizeZ = new Double_t[knbinz]; // array of bin sizes along z.
    for(i=0;i<knbinz;i++) binSizeZ[i] = kbinz0; // default z bin size.
    binSizeZ[ 31] = kbinz1;
    binSizeZ[ 32] = kbinz1;

    binSizeZ[ 63] = kbinz1;
    binSizeZ[ 64] = kbinz1;

    binSizeZ[ 95] = kbinz1;
    binSizeZ[ 96] = kbinz1;

    binSizeZ[127] = kbinz1;
    binSizeZ[128] = kbinz1;

    binSizeZ[159] = kbinz1;
    binSizeZ[160] = kbinz1;

    // correct detector size for bin size.
    dx = 0.0;
    for(i=0;i<knbinx;i++) dx += binSizeX[i];
    dx *= 0.5;
    dz = 0.0;
    for(i=0;i<knbinz;i++) dz += binSizeZ[i];
    dz *= 0.5;
IlcFatal("");
    SetShape("ActiveSPD","Active volume of SPD","SPD SI DET",dx,kdy,dz);
    if(TMath::Abs(dx-kdx)>1.0E-4 || TMath::Abs(dz-kdz)>1.0E-4) 
	Warning("Default Creator","Detector size may not be write.");

    InitLowBinEdgeX(); // array of bin sizes along x.
    InitLowBinEdgeZ(); // array of bin sizes along x.
    SetLowBinEdgeX(0,-dx);
    SetLowBinEdgeZ(0,-dz);
    for(i=0;i<knbinx;i++) SetLowBinEdgeX(i+1,GetBinLowEdgeX(i)+binSizeX[i]);
    for(i=0;i<knbinz;i++) SetLowBinEdgeZ(i+1,GetBinLowEdgeZ(i)+binSizeZ[i]);
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETgeomSPD425Short &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////

    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETgeomSPD425Short &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////

    r.Read(&is);
    return is;
}
//======================================================================
