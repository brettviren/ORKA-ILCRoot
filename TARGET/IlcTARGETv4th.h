#ifndef ILCTARGETV4TH_H
#define ILCTARGETV4TH_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETv4th.h,v 1.1 2008/07/08 10:11:06 rucco Exp $ */

/////////////////////////////////////////////////////////
//  Manager and hits classes for set: TARGET version 10   //
/////////////////////////////////////////////////////////
#include <TArrayI.h>
#include <TArrayD.h>

#include "IlcTARGET.h"

class IlcTARGETv4th : public IlcTARGET {

public:
    IlcTARGETv4th();
    IlcTARGETv4th(const char *name, const char *title, Int_t MinorVersion);
// // // //     IlcTARGETv4th(const IlcTARGETv4th &source); // copy constructor
    IlcTARGETv4th& operator=(const IlcTARGETv4th &source); // assignment operator
    virtual       ~IlcTARGETv4th() ;
    virtual void   BuildGeometry();
    TList *   BuildGeometry2(void) ;             // creates the geometry for the ROOT display
    virtual void   CreateGeometry();
    virtual void   CreateMaterials();
    virtual Int_t  IsVersion() const {// returns the TARGET version number
        return 0;
    }
    virtual void   Init();
    virtual void   SetDefaults();
    virtual void   DrawModule() const;
    virtual void   StepManager();
    virtual void   SetPixelDimension(Int_t lay,Float_t pixelx=20.,Float_t pixely=20.,Float_t pixelz=20.) {
        fPixelX.AddAt(pixelx,lay);
        fPixelY.AddAt(pixely,lay);
        fPixelZ.AddAt(pixelz,lay);
    }
    void  SetNPixelX(Int_t lay,Int_t npixelx) {
        fNPixelX.AddAt(npixelx,lay);
    }
    void  SetNPixelY(Int_t lay,Int_t npixely) {
        fNPixelY.AddAt(npixely,lay);
    }
    void  SetNPixelZ(Int_t lay,Int_t npixelz) {
        fNPixelZ.AddAt(npixelz,lay);
    }
    void SetDisplay(Char_t display[10]="light") {
        sprintf(fDisplay,"%s",display);
    }
    virtual void   SetWriteDet(Bool_t det=kFALSE) { // set .det write
        fGeomDetOut = det;
    }
    virtual void   SetWriteDet(const char *f) { // set write file
        strncpy(fWrite,f,60);
        fGeomDetOut = kTRUE;
    }
    virtual void   SetReadDet(Bool_t det=kFALSE) { //set .det read
        fGeomDetIn = det;
    }
    virtual void   SetReadDet(const char *f) { // set read file
        strncpy(fRead,f,60);
        fGeomDetIn = kTRUE;
    }
// // // //     virtual void   SetEUCLIDFileName(const char *f) { // set write file
// // // //         fEuclidGeometry=f;
// // // //         SetEUCLID();
// // // //     }
    virtual void   SetThicknessDet1(Float_t v=200.) {
        // Set detector thickness in layer 1
        fDet1 = v;
    }
    virtual void   SetThicknessDet2(Float_t v=200.) {
        // Set detector thickness in layer 2
        fDet2 = v;
    }
    virtual void   SetThicknessChip1(Float_t v=150.) {
        // Set chip thickness in layer 1
        fChip1 = v;
    }
    virtual void   SetThicknessChip2(Float_t v=150.) {
        // Set chip thickness in layer 2
        fChip2 = v;
    }
    virtual void   SetRails(Int_t v=0) {
        // Set flag for rails
        fRails = v;
    }
    virtual void   SetCoolingFluid(Int_t v=1) {
        // Set flag for cooling fluid
        fFluid = v;
    }
    virtual void SetDensityServicesByThickness() {// uses services density
        // calculation based on the thickness of the services.
        fByThick = kTRUE;
    }
    virtual void SetDensityServicesByMass() {// uses services density
        // calculation based on the Mass of the services.
        fByThick = kFALSE;
    }
    virtual void GetPixelDimension(Int_t lay,Float_t &pixelx, Float_t &pixely,Float_t &pixelz) {
        pixelx=fPixelX.At(lay);
        pixely=fPixelY.At(lay);
        pixelz=fPixelZ.At(lay);
    }
    void  GetNPixelX(Int_t lay,Int_t &npixelx) {
        npixelx=fNPixelX.At(lay);
    }
    void  GetNPixelY(Int_t lay,Int_t &npixely) {
        npixely=fNPixelY.At(lay);
    }
    void  GetNPixelZ(Int_t lay,Int_t &npixelz) {
        npixelz=fNPixelZ.At(lay);
    }

// // // //     virtual const char  *GetEULIIDFileName() const { // return .euc file name
// // // //         return fEuclidGeometry.Data();
// // // //     }
    virtual Bool_t GetWriteDet() const { // returns value GeomDetOut flag.
        return fGeomDetOut;
    }
    virtual Bool_t GetReadDet() const { // returns value GeomDetIn flag.
        return fGeomDetIn;
    }
    virtual char  *GetReadDetFileName() { // return .det read file name
        if (fRead[0]!='\0') return fRead;
        else return fEuclidGeomDet;
    }
    virtual char  *GetWriteDetFileName() { // return .det write file name
        if (fWrite[0]!='\0') return fWrite;
        else return fEuclidGeomDet;
    }
    virtual Int_t GetMajorVersion() const {// return Major Version Number
        return fMajorVersion;
    }
    virtual Float_t GetThicknessDet1() const {
        // Get detector thickness in layer 1
        return fDet1;
    }
    virtual Float_t GetThicknessDet2() const {
        // Get detector thickness in layer 2
        return fDet2;
    }
    virtual Float_t GetThicknessChip1() const {
        // Get chip thickness in layer 1
        return fChip1;
    }
    virtual Float_t GetThicknessChip2()const {
        // Get chip thickness in layer 2
        return fChip2;
    }
    virtual Int_t GetRails() const {
        // Get flag for rails
        return fRails;
    }
    virtual Int_t GetCoolingFluid() const {
        // Get flag for cooling fluid
        return fFluid;
    }

private:
    void InitIlcTARGETgeom();

    // TString fEuclidGeomtery,fEuclidMaterial defined in IlcModule.
    Bool_t fGeomDetOut;       // Flag to write .det file out
    Bool_t fGeomDetIn;        // Flag to read .det file or directly from Geat.
    Bool_t fByThick;          // Flag to use services materials by thickness
    // ture, or mass false.
    Int_t  fMajorVersion;     // Major version number == IsVersion
    Int_t  fMinorVersion;     // Minor version number
    char   fEuclidGeomDet[60];// file where detector transormation are define.
    char   fRead[60];         //! file name to read .det file
    char   fWrite[60];        //! file name to write .det file
    Float_t  fDet1;	      // thickness of detector in SPD layer 1
    Float_t  fDet2;	      // thickness of detector in SPD layer 2
    Float_t  fChip1;	      // thickness of chip in SPD layer 1
    Float_t  fChip2;	      // thickness of chip in SPD layer 2
    Int_t    fRails;          // flag to switch rails on (=1) and off (=0)
    Int_t    fFluid;          // flag to switch between water (=1) and freon (=0)
    Char_t   fDisplay[10];
    //Int_t fIDMother;          //! TARGET Mother Volume id.
    TArrayF fPixelX;
    TArrayF fPixelY;
    TArrayF fPixelZ;
    TArrayI    fNPixelX;
    TArrayI    fNPixelY;
    TArrayI    fNPixelZ;


    ClassDef(IlcTARGETv4th,1)  //Hits manager for set:TARGET version 8
    // PPR detailed Geometry asymmetric
};

#endif
