#ifndef ILCTARGETGEOMSSD_H
#define ILCTARGETGEOMSSD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

////////////////////////////////////////////////////////////////////////
// This class is for the Silicon Strip Detector, SSD, specific geometry.
// It is being replaced by IlcTARGETsegmentationSSD class. This file also
// constains classes derived from IlcTARGETgeomSSD which do nothing but
// initilize this one with predefined values.
////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TBRIK.h>
#include "IlcTARGETgeom.h"
#include "IlcTARGETParam.h"

class TShape;

class IlcTARGETgeomSSD : public TObject {

 public:
    IlcTARGETgeomSSD(); // default constructor
    IlcTARGETgeomSSD(const Double_t *box,Double_t ap,Double_t an,
		  Int_t np,Double_t *p,Int_t nn,Double_t *n); // Constructor
    virtual ~IlcTARGETgeomSSD(); // Destructor
    IlcTARGETgeomSSD(const IlcTARGETgeomSSD &source);// copy constructor
    virtual IlcTARGETgeomSSD& operator=(const IlcTARGETgeomSSD &source); // = opt.
    void ResetSSD(const Double_t *box,Double_t ap,Double_t an,
		  Int_t np,Double_t *p,Int_t nn,Double_t *n); // Filler
    virtual TShape *GetShape() const {return fShapeSSD;}// get shape
    virtual Double_t GetDx() const {if(fShapeSSD!=0) return fShapeSSD->GetDx();
                    else return 0.0;}// get Dx
    virtual Double_t GetDy() const {if(fShapeSSD!=0) return fShapeSSD->GetDy();
                    else return 0.0;}// get Dy
    virtual Double_t GetDz() const {if(fShapeSSD!=0) return fShapeSSD->GetDz();
                    else return 0.0;}// get Dz
    virtual Int_t GetNAnodes() const {return fNp-1;}//the number of Anodes "P"
    virtual Int_t GetNCathodess() const {return fNn-1;}//the number of Cathodes "N"
    virtual Double_t GetAnodePitch(Int_t i=0) const { //anode pitch for anode i
	if(i>=0&&i<fNp) return fLowEdgeP[i+1]-fLowEdgeP[i];else return 0.0;}
    virtual Float_t GetCathodePitch(Int_t i=0) const { // cathode pitch for cathode i
	if(i>0&&i<fNn) return fLowEdgeN[1]-fLowEdgeN[0];else return 0.0;}
    virtual Float_t GetAnodeAngle() const {return fAngleP;}//anode strip angle.
    virtual Float_t GetCathodeAngle() const {return fAngleN;}//cathode strip angle.
    virtual void SetShape(char *name,char *title,char *mat,
                          Float_t dx,Float_t dy,Float_t dz){
	// defines TBRIK with given paramters
        fShapeSSD = new TBRIK(name,title,mat,dx,dy,dz);};
    virtual void SetNAnodes(Int_t n) {// sets the number of Anodes "P" and
	// allocates array of low edges.
	fNp=n+1;delete fLowEdgeP;fLowEdgeP = new Double_t[fNp];}
    virtual void SetNCathotess(Int_t n) {// sets the number of Anodes "N" and 
	// allocates array of low edges.
	fNn=n+1;delete fLowEdgeN;fLowEdgeN =new  Double_t[fNn];}
    virtual void SetAnodeLowEdges(Float_t *p){// sets Anode low edges +1.
	for(Int_t i=0;i<fNp;i++)fLowEdgeP[i]=p[i];}
    virtual void SetCathodeLowEdges(Float_t *p){// sets Cathodes low edges +1.
	for(Int_t i=0;i<fNn;i++)fLowEdgeN[i]=p[i];}
    virtual void SetAnodeAngle(Float_t a){fAngleP=a;} //sets anode angle.
    virtual void SetCathodeAngle(Float_t a){fAngleN=a;}//sets cathode  angle.

    virtual void Local2Det(Float_t x,Float_t z,Int_t &a,Int_t &c);
    virtual void Det2Local(Int_t a,Int_t c,Float_t &x,Float_t &z);

    virtual void Print(ostream *os) const;  // Output streamer to standard out.
    virtual void Read(istream *is);   // Input streamer to standard in.
    virtual void Print(Option_t *option="") const {TObject::Print(option);}
    virtual Int_t Read(const char *name) {return TObject::Read(name);}

 protected:
    // -+-> x
    //  |
    //  V
    //  z
    TBRIK *fShapeSSD; // comment
    Int_t   fNp;      // Number of Anode strips.
    Int_t   fNn;      // Number of Cathode strips.
    Double_t *fLowEdgeP;  //[fNp] Anode side strip pitch angle==0.
    Double_t *fLowEdgeN;  //[fNn] Cathode side strip pich angle==0.
    Double_t fAngleP;  // Anode side strip angle (rad).
    Double_t fAngleN;  // Cathode side strip angle (rad).
    // or what other or different information that is needed.
    
    ClassDef(IlcTARGETgeomSSD,1) // TARGET SSD detector geometry class

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSSD &source);
istream &operator>>(istream &os,IlcTARGETgeomSSD &source);
#endif
//======================================================================
#ifndef ILCTARGETGEOMSSD175_H
#define ILCTARGETGEOMSSD175_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

//#include "IlcTARGETgeomSSD.h"


class TShape;

class IlcTARGETgeomSSD175 : public IlcTARGETgeomSSD {

 public:
    IlcTARGETgeomSSD175();
    virtual IlcTARGETgeomSSD& operator=(const IlcTARGETgeomSSD &source);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SDD detector geometry.
    ClassDef(IlcTARGETgeomSSD175,0) // TARGET SSD detector with stips at +- 0.0175 rad.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSSD175 &source);
istream &operator>>(istream &os,IlcTARGETgeomSSD175 &source);
#endif
//======================================================================
#ifndef ILCTARGETGEOMSSD27575_H
#define ILCTARGETGEOMSSD27575_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

//#include "IlcTARGETgeomSSD.h"


class TShape;

class IlcTARGETgeomSSD275and75 : public IlcTARGETgeomSSD {

 public:
    IlcTARGETgeomSSD275and75();
    IlcTARGETgeomSSD275and75(Int_t npar,Double_t *par);
    virtual IlcTARGETgeomSSD& operator=(const IlcTARGETgeomSSD &source);

 // This clas now has version 0 so that it will not be
    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SDD detector geometry.
    ClassDef(IlcTARGETgeomSSD275and75,0) // TARGET SSD detector with 0.0275 and 0.0075 rad strip angles.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSSD275and75 &source);
istream &operator>>(istream &os,IlcTARGETgeomSSD275and75 &source);
#endif
//======================================================================
#ifndef ILCTARGETGEOMSSD75275_H
#define ILCTARGETGEOMSSD75275_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

//#include "IlcTARGETgeomSSD.h"

class TShape;

class IlcTARGETgeomSSD75and275 : public IlcTARGETgeomSSD {

 public:
    IlcTARGETgeomSSD75and275();
    IlcTARGETgeomSSD75and275(Int_t npar,Double_t *par);
    virtual IlcTARGETgeomSSD& operator=(const IlcTARGETgeomSSD &source);

 // This clas now has version 0 so that it will not be
    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SSD detector geometry.
    ClassDef(IlcTARGETgeomSSD75and275,0) // TARGET SSD detector geometry class for 0.0075 and 0.0275 rad angled strips.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSSD75and275 &source);
istream &operator>>(istream &os,IlcTARGETgeomSSD75and275 &source);
#endif

//======================================================================
#ifndef ILCTARGETGEOMSSDSTRIPEND_H
#define ILCTARGETGEOMSSDSTRIPEND_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

//#include "IlcTARGETgeomSSD.h"

class TShape;

class IlcTARGETgeomSSDStripEnd : public IlcTARGETgeomSSD {

 public:
    IlcTARGETgeomSSDStripEnd();
    IlcTARGETgeomSSDStripEnd(Int_t lay,IlcTARGETgeom *geom, IlcTARGETParam *Param,Int_t npar,Float_t *par);
    virtual IlcTARGETgeomSSD& operator=(const IlcTARGETgeomSSD &source);

 // This clas now has version 0 so that it will not be
    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SSD detector geometry.
    ClassDef(IlcTARGETgeomSSDStripEnd,0) // TARGET SSD detector geometry class for 0.0075 and 0.0275 rad angled strips.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSSDStripEnd &source);
istream &operator>>(istream &os,IlcTARGETgeomSSDStripEnd &source);
#endif


//======================================================================
#ifndef ILCTARGETGEOMSSDSTRIPLAY_H
#define ILCTARGETGEOMSSDSTRIPLAY_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

//#include "IlcTARGETgeomSSD.h"

class TShape;

class IlcTARGETgeomSSDStripLay : public IlcTARGETgeomSSD {

 public:
    IlcTARGETgeomSSDStripLay();
    IlcTARGETgeomSSDStripLay(Int_t lay,IlcTARGETgeom *geom, IlcTARGETParam *Param, Int_t npar,Float_t *par);
    virtual IlcTARGETgeomSSD& operator=(const IlcTARGETgeomSSD &source);

 // This clas now has version 0 so that it will not be
    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SSD detector geometry.
    ClassDef(IlcTARGETgeomSSDStripLay,0) // TARGET SSD detector geometry class for 0.0075 and 0.0275 rad angled strips.

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSSDStripLay &source);
istream &operator>>(istream &os,IlcTARGETgeomSSDStripLay &source);
#endif



