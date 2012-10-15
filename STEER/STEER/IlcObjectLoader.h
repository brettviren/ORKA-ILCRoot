#ifndef ILCOBJECTLOADER_H
#define ILCOBJECTLOADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcObjectLoader.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////
//                                        //
//  class IlcObjectLoader                 //
//                                        //
//                                        //
////////////////////////////////////////////

class TObject;
class TFolder;
class TString;
class IlcDataLoader;

#include "IlcBaseLoader.h"

class IlcObjectLoader: public IlcBaseLoader
{
 public:
     IlcObjectLoader(){};
     IlcObjectLoader(const TString& name, IlcDataLoader* dl, Bool_t storeontop = kFALSE);
     virtual          ~IlcObjectLoader(){};
     TObject*          Get() const;

 protected:
     TFolder*          GetFolder() const;
     Int_t             AddToBoard(TObject* obj);
     void              RemoveFromBoard(TObject* obj);

 private:
     IlcObjectLoader(const IlcObjectLoader&);            //Not implemented
     IlcObjectLoader& operator=(const IlcObjectLoader&); //Not implemented


 ClassDef(IlcObjectLoader,1)    
  
};
#endif


