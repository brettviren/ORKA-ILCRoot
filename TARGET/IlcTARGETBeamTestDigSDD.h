#ifndef ILCTARGETBEAMTESTDIGSDD_H
#define ILCTARGETBEAMTESTDIGSDD_H

////////////////////////////////////////////////////
//  Class to define                               //
//  SDD beam test raw 2 dig conv.                 //
//  Origin: E. Crescio crescio@to.infn.it         //
//                                                //
////////////////////////////////////////////////////

#include "IlcTARGETBeamTestDig.h"


class IlcTARGETRawStreamSDD;

typedef enum {kAug04,kNov04} BeamtestPeriod_t;

class IlcTARGETBeamTestDigSDD: public IlcTARGETBeamTestDig {
 
 public:

 
  IlcTARGETBeamTestDigSDD();
  IlcTARGETBeamTestDigSDD(const Text_t* name, const Text_t* title);
  IlcTARGETBeamTestDigSDD(const IlcTARGETBeamTestDigSDD& bt);
  IlcTARGETBeamTestDigSDD& operator=(const IlcTARGETBeamTestDigSDD &source);

  virtual ~IlcTARGETBeamTestDigSDD();

  void SetBtPeriod(BeamtestPeriod_t per=kNov04) {fBtPer=per;}
  void SetThreshold(Int_t threshold) {fThreshold=threshold;}

  BeamtestPeriod_t GetBtPeriod() const {return fBtPer;}
  Int_t GetThreshold() const {return fThreshold;}
  
  virtual void Exec(Option_t* opt);

  
 
 protected:      

  Int_t  fSDDEvType;                 //SDD event type (real, calibration)
  const UInt_t* fSubEventAttributes; //SDD sub-event attributes 
  BeamtestPeriod_t fBtPer;           //beam test version 
                                     // November 2004 = kNov04
                                     // August 2004 = kAug04
  Int_t fThreshold;                  // Low carlos threshold 
  IlcTARGETRawStreamSDD* fStreamer;     //! SDD streamer

 private: 

  Int_t GetEventType();


  ClassDef(IlcTARGETBeamTestDigSDD,1)  // An Ilc SDD beam test digitizer

 };



#endif

    
