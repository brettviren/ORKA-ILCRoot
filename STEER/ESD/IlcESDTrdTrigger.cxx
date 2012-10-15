#include "TObject.h"

#include "IlcESDTrdTrigger.h"

IlcESDTrdTrigger::IlcESDTrdTrigger() : 
  TObject()
{
  // default ctor

  for (Int_t iSector = 0; iSector < fgkNsectors; iSector++) {
    fFlags[iSector] = 0x0;
  }
}

IlcESDTrdTrigger::IlcESDTrdTrigger(const IlcESDTrdTrigger &rhs) :
  TObject(rhs)
{
  // copy ctor

  for (Int_t iSector = 0; iSector < fgkNsectors; iSector++) {
    fFlags[iSector] = rhs.fFlags[iSector];
  }
}

IlcESDTrdTrigger& IlcESDTrdTrigger::operator=(const IlcESDTrdTrigger &rhs)
{
  // assignment operator
  if (&rhs != this) {
    TObject::operator=(rhs);
    for (Int_t iSector = 0; iSector < fgkNsectors; iSector++) {
      fFlags[iSector] = rhs.fFlags[iSector];
    }
  }

  return *this;
}

IlcESDTrdTrigger::~IlcESDTrdTrigger()
{
  // dtor

}
