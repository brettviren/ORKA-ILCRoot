#ifndef ILCSURVEYTOILCGNOBJS_H
#define ILCSURVEYTOILCGNOBJS_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
//     Base class for creating the alignment objects from survey       //
//     for a given subdetector                                         //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
#include <TSystem.h>
#include <TObjArray.h>
#include <TClonesArray.h>

#include "IlcAlignObjParams.h"
#include "IlcSurveyObj.h"


class IlcSurveyToAlignObjs : public TObject {

 public:
  IlcSurveyToAlignObjs();
  IlcSurveyToAlignObjs(const IlcSurveyToAlignObjs &align); // copy constructor
  IlcSurveyToAlignObjs &operator = (const IlcSurveyToAlignObjs &align); //assignment operator

  Bool_t LoadSurveyFromLocalFile(const char* filename);
  Bool_t LoadSurveyFromAlienFile(const char* det, Int_t repNum, Int_t repVersion);

  virtual Bool_t CreateAlignObjs() = 0;
  virtual void Run() = 0;
  Bool_t StoreAlignObjToFile(const char* filename, const char* det);
  Bool_t StoreAlignObjToCDB(const char* cdbFolder, const char* det);
  TClonesArray* GetAlignObjsArray() {return fIlcgnObjArray;}
  virtual   ~IlcSurveyToAlignObjs();
  //
 protected:
  IlcSurveyObj      *fSurveyObj;         // survey object
  TObjArray         *fSurveyPoints;     // array of survey points
  TClonesArray      *fIlcgnObjArray;    // TClonesArray of alignment objects
  IlcAlignObjParams *fIlcgnObj;         // alignment object

  ClassDef(IlcSurveyToAlignObjs,0);
};
#endif
