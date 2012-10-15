#ifndef ILCESDTRDTRIGGER_H
#define ILCESDTRDTRIGGER_H

#include "TObject.h"

class IlcESDTrdTrigger : public TObject
{
 public:
  IlcESDTrdTrigger();
  IlcESDTrdTrigger(const IlcESDTrdTrigger &rhs);
  IlcESDTrdTrigger& operator=(const IlcESDTrdTrigger &rhs);
  ~IlcESDTrdTrigger();

  UInt_t GetFlags(const Int_t sector) const { return fFlags[sector]; }

  void SetFlags(const Int_t sector, const UInt_t flags) { fFlags[sector] = flags; }

 protected:
  static const Int_t fgkNsectors = 18;	  // number of sectors

  UInt_t fFlags[fgkNsectors];	          // trigger flags for every sector

  ClassDef(IlcESDTrdTrigger, 1);
};

#endif
