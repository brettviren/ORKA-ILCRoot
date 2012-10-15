#ifndef ILC_GRP_DCS_H
#define ILC_GRP_DCS_H

//-------------------------------------------------------------------------
//                          Class IlcGRPDCS
//   This class deals with the DCS related info of the GRP
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                        IlcGRPDCS                                     //
//                                                                      //
//           Implementation of the class that processes                 //
//           the DCS related fields of the GRP.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"

class IlcGRPDCS: public TObject {
 public:
  IlcGRPDCS();
  IlcGRPDCS(TObjArray *dcsArray, UInt_t fStart, UInt_t fStop);
  IlcGRPDCS(const IlcGRPDCS& grpDcs);

  void SetTime(UInt_t fStart, UInt_t fStop) {fStartTime = fStart; fStopTime = fStop;}
  void SetObjArray(TObjArray *dcsSArray) {fDCSArray = dcsSArray;}
  const char *ProcessDCS(Int_t iType);  
  
 private:
  UInt_t fStartTime, fStopTime; //start and stop time of the run (DAQ lb)
  TObjArray *fDCSArray; //TObjArray for a dcs data point
  
  const char *ProcessInt();
  const char *ProcessUInt();
  const char *ProcessFloat();
  const char *ProcessChar();
//  const char *ProcessString();
  const char *ProcessBoolean();
  
  IlcGRPDCS & operator=(const IlcGRPDCS & ) {return *this;}

  ClassDef(IlcGRPDCS, 0);
};

#endif
