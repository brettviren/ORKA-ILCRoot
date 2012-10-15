#ifndef ILC_MISILCGNER_H
#define ILC_MISILCGNER_H

#include "TObject.h"
#include "TString.h"
#include "IlcCDBMetaData.h"

class TClonesArray;
class IlcCDBManager;

// Base class for creating a TClonesArray of simulated misalignment objects
// for a given subdetector of type ideal,residual or full
//

class IlcMisAligner : public TObject {

  public:
    IlcMisAligner();
    virtual TClonesArray* MakeAlObjsArray() =0;
    virtual IlcCDBMetaData* GetCDBMetaData() const =0;
    void SetMisalType(const char* misalType)
    {
      fMisalType=misalType;
    }
    const char* GetMisalType() const
    {
      return fMisalType.Data();
    }

  protected:
    TString fMisalType;

  private:
    ClassDef(IlcMisAligner,0);
};

#endif

