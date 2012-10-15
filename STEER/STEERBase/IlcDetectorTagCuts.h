#ifndef ILCDETECTORTAGCUTS_H
#define ILCDETECTORTAGCUTS_H
/*  See cxx source for full Copyright notice */


/* $Id: IlcDetectorTagCuts.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//                       Class IlcDetectorTagCuts
//              This is the class for the cuts in run tags
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>
#include <TString.h>
#include <IlcDAQ.h>

class IlcDetectorTag;

//___________________________________________________________________________
class IlcDetectorTagCuts : public TObject {
 public:
  IlcDetectorTagCuts();
  ~IlcDetectorTagCuts();
   
 //____________________________________________________//
  void SetListOfDetectors(const TString& detectors) {fDetectorsDAQ = IlcDAQ::DetectorPattern(detectors); fDetectorsReco = IlcDAQ::DetectorPattern(detectors); fDetectorsFlag = kTRUE;}
  void SetListOfDetectorsDAQ(const TString& detectors) {fDetectorsDAQ = IlcDAQ::DetectorPattern(detectors); fDetectorsFlag = kTRUE;}
  void SetListOfDetectorsReco(const TString& detectors) {fDetectorsReco = IlcDAQ::DetectorPattern(detectors); fDetectorsFlag = kTRUE;}
  void SetDetectorValidityValue(TString det, UShort_t val);
 
  Bool_t IsAccepted(IlcDetectorTag *detTag) const;

  //____________________________________________________//
 private:
  //  Bool_t  IsSelected(TString detName, TString& detectors) const;

  //  TString fDetectors; //detectors active
  UInt_t fDetectorsReco;  //selected detector pattern for Reco
  UInt_t fDetectorsDAQ;   //selected detector pattern for DAQ
  Bool_t fDetectorsFlag; //cut used or not
  UShort_t   fDetectorValidityMatch[IlcDAQ::kHLTId]; // Detector validity to match
  Bool_t     fDetectorValidityFlag[IlcDAQ::kHLTId];  // Flag if validity match is to be used
  
  ClassDef(IlcDetectorTagCuts, 3)
};

#endif
