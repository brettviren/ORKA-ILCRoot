#ifndef ILCTREELOADER_H
#define ILCTREELOADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTreeLoader.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////
//                                        //
//  class IlcTreeLoader                   //
//                                        //
//  Loader responsible for one data type  //
//  i.e. Hits, Kine, etc.                 //
//  many objects type can be assciated    //
//  with one data type: storing object    //
//  (usually tree), task producing it,    //
//  Quality Assurance(QA), QA Task, and   //
//  others.                               //
//                                        //
//                                        //
////////////////////////////////////////////

class TString;

#include "IlcObjectLoader.h"
#include <TTree.h>
#include <TObject.h>

class IlcTreeLoader: public IlcObjectLoader
 {
public:
     IlcTreeLoader(){};
     IlcTreeLoader(const TString& name, IlcDataLoader* dl, Bool_t storeontop = kFALSE);
     virtual ~IlcTreeLoader(){};
     
     virtual TTree*     Tree() const {return dynamic_cast<TTree*>(Get());}
     virtual void       MakeTree();
     virtual Int_t      WriteData(Option_t* opt="");

private:
     IlcTreeLoader(const IlcTreeLoader&);            //Not implemented
     IlcTreeLoader& operator=(const IlcTreeLoader&); //Not implemented

     ClassDef(IlcTreeLoader,1)    
 };

#endif


