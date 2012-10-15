#ifndef ILCTARGETGEOMSPD_H
#define ILCTARGETGEOMSPD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSPD.h,v 1.3 2008/11/07 00:03:53 cgatto Exp $ */

#include <TObject.h>
#include <TBRIK.h>
#include "IlcTARGETgeom.h"

////////////////////////////////////////////////////////////////////////
// This class is for the Silicon Pixel Detector, SPD, specific geometry.
// It is being replaced by IlcTARGETsegmentationSPD class. This file also
// constains classes derived from IlcTARGETgeomSPD which do nothing but
// initilize this one with predefined values.
////////////////////////////////////////////////////////////////////////

class TShape;

class IlcTARGETgeomSPD : public TObject {

 public:
    IlcTARGETgeomSPD();
    IlcTARGETgeomSPD(Double_t dy,Int_t nx,Double_t *bx,Int_t nz,Double_t *bz);
    IlcTARGETgeomSPD(IlcTARGETgeomSPD &source);
    IlcTARGETgeomSPD& operator=(IlcTARGETgeomSPD &source);
    virtual ~IlcTARGETgeomSPD();
    virtual void ReSetBins(Double_t dy,Int_t nx,Float_t *bx,
			   Int_t nz,Float_t *bz);
    virtual void ReSetBins(Double_t dy,Int_t nx,Double_t *bx,
			   Int_t nz,Double_t *bz);
    virtual TShape *GetShapeSPD() const {return fShapeSPD;}
    virtual Double_t GetDx() const { // Get TBRIK Dx
        if(fShapeSPD!=0) return fShapeSPD->GetDx();
        else return 0.0;}
    virtual Double_t GetDy() const {// Get TBRIK Dy
        if(fShapeSPD!=0) return fShapeSPD->GetDy();
        else return 0.0;}
    virtual Double_t GetDz() const {// Get TBRIK Dz
        if(fShapeSPD!=0) return fShapeSPD->GetDz();
        else return 0.0;}
    virtual Int_t GetNbinsX() const {return fNbinx-1;} // returns the number of bins x
    virtual Int_t GetNbinsZ() const {return fNbinz-1;} // returns the number of bins z
    virtual Double_t GetBinSizeX(Int_t i) const
	{if(i<fNbinx-1&&i>=0) return fLowBinEdgeX[i+1]-fLowBinEdgeX[i];
	else return 0.0;}; // give size of bin i in x.
    virtual Double_t GetBinSizeZ(Int_t i) const
	{if(i<fNbinx-1&&i>=0) return fLowBinEdgeZ[i+1]-fLowBinEdgeZ[i];
	else return 0.0;}; // give size of bin i in z.
    virtual Double_t GetBinLowEdgeX(Int_t i) const
	{if(i<fNbinx-1&&i>=0) return fLowBinEdgeX[i];
	else return 0.0;}; // give size of bin i in x.
    virtual Double_t GetBinLowEdgeZ(Int_t i) const
	{if(i<fNbinz-1&&i>=0) return fLowBinEdgeZ[i];
	else return 0.0;}; // give size of bin i in z.
    virtual void InitLowBinEdgeX(){// allocate memory for fLowBinEdgeX.
	if(fLowBinEdgeX!=0) delete[] fLowBinEdgeX;
	if(fNbinx>0) fLowBinEdgeX = new Float_t[fNbinx];else fLowBinEdgeX = 0;}
    virtual void InitLowBinEdgeZ(){// allocate memory for fLowBinEdgeZ.
	if(fLowBinEdgeZ!=0) delete[] fLowBinEdgeZ;
	if(fNbinz>0) fLowBinEdgeZ = new Float_t[fNbinz];else fLowBinEdgeZ = 0;}
    virtual void SetShape(const char *name,const char *title,const char *mat,
			  Double_t dx,Double_t dy,Double_t dz)
	{fShapeSPD = new TBRIK(name,title,mat,dx,dy,dz);};
    virtual void SetNbinX(Int_t i){fNbinx = i+1;} // Set nubmer of pixels in x
    virtual void SetNbinZ(Int_t i){fNbinz = i+1;} // Set nubmer of pixels in z
    virtual void SetLowBinEdgeX(Int_t i,Double_t s){//puts value in fLowbinEdgeX
        if(i>=0&&i<fNbinx) fLowBinEdgeX[i] = s;}
    virtual void SetLowBinEdgeZ(Int_t i,Double_t s){//puts value in fLowbinEdgeZ
	if(i>=0&&i<fNbinz) fLowBinEdgeZ[i] = s;};
    virtual void LToDet(Double_t xl,Double_t zl,Int_t &row,Int_t &col);
    virtual void DetToL(Int_t row,Int_t col,Double_t &xl,Double_t &zl);
    virtual void Print(ostream *os) const; // output streamer to standard out.
    virtual void Read(istream *is); // input streamer from standard in.
    virtual void Print(Option_t *option="") const {TObject::Print(option);}
    virtual Int_t Read(const char *name) {return TObject::Read(name);}

 protected:
    TBRIK  *fShapeSPD; // SPD active area shape
    Int_t fNbinx;  // the number of elements in fLowBinEdgeX (#bins-1)
    Int_t fNbinz;  // the number of elements in fLowBinEdgeZ (#bins-1)
    Float_t *fLowBinEdgeX; //[fNbinx] Array of X lower bin edges for the pixels
    Float_t *fLowBinEdgeZ; //[fNbinz] Array of Z lower bin edges for the pixels

    ClassDef(IlcTARGETgeomSPD,1) // TARGET SPD detector geometry class..

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPD &source);
istream &operator>>(istream &os,IlcTARGETgeomSPD &source);
#endif
//======================================================================

#ifndef ILCTARGETGEOMSPD300_H
#define ILCTARGETGEOMSPD300_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSPD.h,v 1.3 2008/11/07 00:03:53 cgatto Exp $ */

//#include "IlcTARGETgeomSPD.h"


class IlcTARGETgeomSPD300 : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPD300();

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPD300,0) // TARGET SPD detector geometry class for 300X50 micron pixel size.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPD300 &source);
istream &operator>>(istream &os,IlcTARGETgeomSPD300 &source);
#endif
//======================================================================

#ifndef ILCTARGETGEOMSPD20SHORT_H
#define ILCTARGETGEOMSPD20SHORT_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSPD.h,v 1.3 2008/11/07 00:03:53 cgatto Exp $ */

//#include "IlcTARGETgeomSPD.h"


// temporary - this will migrate into the segmentation class


class IlcTARGETgeomSPD20Short : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPD20Short();
    IlcTARGETgeomSPD20Short(Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPD20Short,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPD20Short &source);
istream &operator>>(istream &os,IlcTARGETgeomSPD20Short &source);
#endif
//======================================================================

#ifndef ILCTARGETGEOMSPD425SHORT_H
#define ILCTARGETGEOMSPD425SHORT_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSPD.h,v 1.3 2008/11/07 00:03:53 cgatto Exp $ */

//#include "IlcTARGETgeomSPD.h"


// temporary - this will migrate into the segmentation class


class IlcTARGETgeomSPD425Short : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPD425Short();
    IlcTARGETgeomSPD425Short(Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPD425Short,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPD425Short &source);
istream &operator>>(istream &os,IlcTARGETgeomSPD425Short &source);
#endif
//======================================================================

#ifndef ILCTARGETGEOMSPDPIXEL_H
#define ILCTARGETGEOMSPDPIXEL_H

class IlcTARGETgeomSPDPixel : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPDPixel();
    IlcTARGETgeomSPDPixel(Int_t lay,IlcTARGETgeom *,Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPDPixel,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPDPixel &source);
istream &operator>>(istream &os,IlcTARGETgeomSPDPixel &source);
#endif
//======================================================================
//======================================================================

#ifndef ILCTARGETGEOMSPDFIBERSEGM_H
#define ILCTARGETGEOMSPDFIBERSEGM_H

class IlcTARGETgeomSPDFiberSegm : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPDFiberSegm();
    IlcTARGETgeomSPDFiberSegm(Int_t lay,IlcTARGETgeom *,Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPDFiberSegm,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPDFiberSegm &source);
istream &operator>>(istream &os,IlcTARGETgeomSPDFiberSegm &source);
#endif
//======================================================================
#ifndef ILCTARGETGEOMSPDPIXELEND_H
#define ILCTARGETGEOMSPDPIXELEND_H

class IlcTARGETgeomSPDPixelEnd : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPDPixelEnd();
    IlcTARGETgeomSPDPixelEnd(Int_t lay,IlcTARGETgeom *,Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPDPixelEnd,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPDPixelEnd &source);
istream &operator>>(istream &os,IlcTARGETgeomSPDPixelEnd &source);
#endif
//======================================================================
//======================================================================
#ifndef ILCTARGETGEOMSPDPIXELENDV4TH_H
#define ILCTARGETGEOMSPDPIXELENDV4TH_H

class IlcTARGETgeomSPDPixelEndv4th : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPDPixelEndv4th();
    IlcTARGETgeomSPDPixelEndv4th(Int_t lay,IlcTARGETgeom *,Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPDPixelEndv4th,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPDPixelEndv4th &source);
istream &operator>>(istream &os,IlcTARGETgeomSPDPixelEndv4th &source);
#endif
//======================================================================


#ifndef ILCTARGETGEOMSPD20LONG_H
#define ILCTARGETGEOMSPD20LONG_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSPD.h,v 1.3 2008/11/07 00:03:53 cgatto Exp $ */

//#include "IlcTARGETgeomSPD.h"


// temporary - this will migrate into the segmentation class


class IlcTARGETgeomSPD20Long : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPD20Long();
    IlcTARGETgeomSPD20Long(Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPD20Long,0) // TARGET SPD detector geometry class for 425X50 micron pixel size Short version.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPD20Long &source);
istream &operator>>(istream &os,IlcTARGETgeomSPD20Long &source);
#endif
//======================================================================

#ifndef ILCTARGETGEOMSPD425LONG_H
#define ILCTARGETGEOMSPD425LONG_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSPD.h,v 1.3 2008/11/07 00:03:53 cgatto Exp $ */

//#include "IlcTARGETgeomSPD.h"


// temporary - this will migrate into the segmentation class


class IlcTARGETgeomSPD425Long : public IlcTARGETgeomSPD {

 public:
    IlcTARGETgeomSPD425Long();
    IlcTARGETgeomSPD425Long(Int_t npar,Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SPD detector geometry.
    ClassDef(IlcTARGETgeomSPD425Long,0) // TARGET SPD detector geometry class for 425X50 micron pixel size.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSPD425Long &source);
istream &operator>>(istream &os,IlcTARGETgeomSPD425Long &source);
#endif
