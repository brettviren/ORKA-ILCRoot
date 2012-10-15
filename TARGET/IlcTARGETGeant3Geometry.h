#ifndef ILCTARGETGEANT3GEOMETRY_H
#define ILCTARGETGEANT3GEOMETRY_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: */
///////////////////////////////////////////////////////////////////////
//  Version: 0                                                       //
//  Written by Bjorn S. Nilsen with code stolen from Andreas Morsch's//
//  IlcGeant3GeometryGUI class.                                      //
///////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TGeant3.h"

class IlcTARGETGeant3Geometry : public TObject {
 public:
    IlcTARGETGeant3Geometry();
    virtual ~IlcTARGETGeant3Geometry(){}
    IlcTARGETGeant3Geometry(const IlcTARGETGeant3Geometry &s) : TObject(s) {}
    IlcTARGETGeant3Geometry & operator=(const IlcTARGETGeant3Geometry &s) 
    {if(&s==this) return *this; return *this;}
    
    // Return number of children for volume idvol
    Int_t NChildren(Int_t idvol);
    // Return child number idc of volume idvol
    Int_t Child(Int_t idvol, Int_t idc);
    // Return medium number for given volume idvol
    Int_t Medium(Int_t idvol);
    // Return material number for given volume idvol
    Int_t Material(Int_t idvol);
    // Reads the zebra geometry tree and put it into the ListTree
//    void  ReadGeometryTree();
    // Read material and media information and put it into ComboBox 
//    void  ReadMaterials();
//    Float_t Cut(Int_t idmed, Int_t icut);
    Int_t GetShape(Int_t idvol,Int_t &npar,Int_t &natt,Float_t *par,
		   Float_t *att);
    void GetGeometry(Int_t nlevels,Int_t *lnam,Int_t *lnum,Double_t *xt,
		     Double_t *r,Int_t &idshape,Int_t &npar,Int_t &natt,
		     Float_t *par,Float_t *att,Int_t &imat,Int_t &imed);
    Int_t StringToInt(char *name);

 private:
// Zebra bank related information	
    Int_t    *fZlq;              // pointer to Zebra bank lq
    Float_t  *fZq;               // pointer to Zebra bank q
    Int_t    *fZiq;              // pointer to Zebra bank iq
    Gclink_t *fGclink;           // pointer to Geant common block 
    Gcnum_t  *fGcnum;            // pointer to Geant common block
    Gcvolu_t *fGcvolu;           // pointer to Geant common block

    ClassDef(IlcTARGETGeant3Geometry,0)  // TARGET Geant3 geometry functions
};
#endif
