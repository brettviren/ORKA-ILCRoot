#ifndef ILCGRPMANAGER_H
#define ILCGRPMANAGER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// IlcGRPManager class                                                    //
// The class can be used in order to access and read the Global Run       //
// Parameters entry from OCDB.                                            //
// It has a methods to set the magnetic field instanton and return        //
// the run and event info objects.                                        //
//                                                                        //
// cvetan.cheshkov@cern.ch 15/06/2009                                     //
////////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class IlcRunInfo;
class IlcGRPObject;

class IlcGRPManager: public TObject {
public:
  IlcGRPManager();
  virtual ~IlcGRPManager();

  const IlcGRPObject* GetGRPData() const { return fGRPData; }

  void        SetGRPEntry(const IlcGRPObject* source);
  Bool_t      ReadGRPEntry();
  Bool_t      SetMagField();

  IlcRunInfo* GetRunInfo();

private:
  
  IlcGRPObject*  fGRPData;        // Data from the GRP/GRP/Data CDB folder

  IlcGRPManager(const IlcGRPManager& man);
  IlcGRPManager& operator = (const IlcGRPManager& man);

  ClassDef(IlcGRPManager, 0)      // class for accessing GRP
};

#endif
