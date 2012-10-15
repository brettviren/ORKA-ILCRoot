#ifndef ILCDETECTORTAG_H
#define ILCDETECTORTAG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcDetectorTag.h 52266 2011-10-24 08:17:59Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcDetectorTag
//   This is the class to deal with the tags for the detector level
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include "TObject.h"
#include "TObjArray.h"
#include "IlcDAQ.h"

//___________________________________________________________________________
class IlcDetectorTag : public TObject {
 public:
  IlcDetectorTag();
  IlcDetectorTag(const IlcDetectorTag & t);

  IlcDetectorTag &operator=(const IlcDetectorTag &rhs);
  virtual ~IlcDetectorTag();
  
  void UpdateFromRunTable(IlcDetectorTag &detTag);

  //____________________________________________________//
  void SetDetectorMask(UInt_t mask)     {fMaskDAQ = mask; fMaskReco = mask; }
  void SetDetectorMaskDAQ(UInt_t mask)  {fMaskDAQ = mask;}
  void SetDetectorMaskReco(UInt_t mask) {fMaskReco = mask;}
  void SetDetectorValidityRange(UChar_t idet, UShort_t vr) {fDetectorValidityRange[idet] = vr; }
  void SetDetectorStatus(UChar_t idet, TString co) { fDetectorStatus[idet] = co; }
  TObjArray *GetDetectorMask() { return 0; } // {return fDetectorArray;}
  UInt_t GetIntDetectorMask() { return fMaskDAQ; }
  UInt_t GetIntDetectorMaskDAQ() { return fMaskDAQ; }
  UInt_t GetIntDetectorMaskReco() { return fMaskReco; }
  UShort_t GetDetectorValidityRange(UChar_t idet) const { return fDetectorValidityRange[idet]; }
  TString  GetDetectorStatus(UChar_t idet) const { return fDetectorStatus[idet]; }
  const char *GetDetectorMaskDAQ() { return IlcDAQ::ListOfTriggeredDetectors(fMaskDAQ); }
  const char *GetDetectorMaskReco() { return IlcDAQ::ListOfTriggeredDetectors(fMaskReco); }
  void PrintDetectorMask();

  //____________________________________________________//
  Bool_t GetITSSPD() const {return fMaskDAQ & IlcDAQ::kSPD;}
  Bool_t GetITSSDD() const {return fMaskDAQ & IlcDAQ::kSSD;}
  Bool_t GetITSSSD() const {return fMaskDAQ & IlcDAQ::kSSD;}
  Bool_t GetTPC()    const {return fMaskDAQ & IlcDAQ::kTPC;}
  Bool_t GetTRD()    const {return fMaskDAQ & IlcDAQ::kTRD;}
  Bool_t GetTOF()    const {return fMaskDAQ & IlcDAQ::kTOF;}
  Bool_t GetHMPID()  const {return fMaskDAQ & IlcDAQ::kHMPID;}
  Bool_t GetPHOS()   const {return fMaskDAQ & IlcDAQ::kPHOS;}
  Bool_t GetPMD()    const {return fMaskDAQ & IlcDAQ::kPMD;}
  Bool_t GetMUON()   const {return fMaskDAQ & IlcDAQ::kMUON;}
  Bool_t GetFMD()    const {return fMaskDAQ & IlcDAQ::kFMD;}
  Bool_t GetTZERO()  const {return fMaskDAQ & IlcDAQ::kT0;}
  Bool_t GetVZERO()  const {return fMaskDAQ & IlcDAQ::kVZERO;}
  Bool_t GetZDC()    const {return fMaskDAQ & IlcDAQ::kZDC;}
  Bool_t GetEMCAL()  const {return fMaskDAQ & IlcDAQ::kEMCAL;}
  
  //____________________________________________________//
 private:
  //  void Int2Bin();
  //   void SetDetectorConfiguration();

  void SetITSSPD() {fMaskDAQ |= IlcDAQ::kSPD  ;}
  void SetITSSDD() {fMaskDAQ |= IlcDAQ::kSDD  ;}
  void SetITSSSD() {fMaskDAQ |= IlcDAQ::kSSD  ;}
  void SetTPC()    {fMaskDAQ |= IlcDAQ::kTPC  ;}
  void SetTRD()    {fMaskDAQ |= IlcDAQ::kTRD  ;}
  void SetTOF()    {fMaskDAQ |= IlcDAQ::kTOF  ;}
  void SetHMPID()  {fMaskDAQ |= IlcDAQ::kHMPID;}
  void SetPHOS()   {fMaskDAQ |= IlcDAQ::kPHOS ;}
  void SetPMD()    {fMaskDAQ |= IlcDAQ::kPMD  ;}
  void SetMUON()   {fMaskDAQ |= IlcDAQ::kMUON ;}
  void SetFMD()    {fMaskDAQ |= IlcDAQ::kFMD  ;}
  void SetTZERO()  {fMaskDAQ |= IlcDAQ::kT0   ;}
  void SetVZERO()  {fMaskDAQ |= IlcDAQ::kVZERO;}
  void SetZDC()    {fMaskDAQ |= IlcDAQ::kZDC  ;}
  void SetEMCAL()  {fMaskDAQ |= IlcDAQ::kEMCAL;}
  
  //   TObjArray *fDetectorArray; //detectors' names - active
  UInt_t     fMaskDAQ;          //detector mask in DAQ
  UInt_t     fMaskReco;         //detector mask in Reco
  //   UInt_t     fDetectors[32]; //detector mask
  //   Bool_t     fITSSPD;        //ITS-SPD active = 1
  //   Bool_t     fITSSDD;        //ITS-SDD active = 1
  //   Bool_t     fITSSSD;        //ITS-SSD active = 1
  //   Bool_t     fTPC;           //TPC active = 1
  //   Bool_t     fTRD;           //TRD active = 1
  //   Bool_t     fTOF;           //TOF active = 1
  //   Bool_t     fHMPID;         //HMPID active = 1
  //   Bool_t     fPHOS;          //PHOS active = 1
  //   Bool_t     fPMD;           //PMD active = 1
  //   Bool_t     fMUON;          //MUON active = 1
  //   Bool_t     fFMD;           //FMD active = 1
  //   Bool_t     fTZERO;         //TZERO active = 1
  //   Bool_t     fVZERO;         //VZERO active = 1
  //   Bool_t     fZDC;           //ZDC active = 1
  //   Bool_t     fEMCAL;         //EMCAL active = 1

  UShort_t   fDetectorValidityRange[IlcDAQ::kHLTId];
  TString    fDetectorStatus[IlcDAQ::kHLTId];

  ClassDef(IlcDetectorTag, 6)  //(ClassName, ClassVersion)
};
//______________________________________________________________________________

#endif
