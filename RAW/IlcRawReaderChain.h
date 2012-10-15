#ifndef ILCRAWREADERCHAIN_H
#define ILCRAWREADERCHAIN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a class for reading raw data from a root chain.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcRawReaderRoot.h"
#include <TString.h>

class TChain;
class TFileCollection;
class TEntryList;

class IlcRawReaderChain: public IlcRawReaderRoot {
  public :
    IlcRawReaderChain();
    IlcRawReaderChain(const char* fileName);
    IlcRawReaderChain(TFileCollection *collection);
    IlcRawReaderChain(TChain *chain);
    IlcRawReaderChain(TEntryList *elist);
    IlcRawReaderChain(Int_t runNumber);
    IlcRawReaderChain(const IlcRawReaderChain& rawReader);
    IlcRawReaderChain& operator = (const IlcRawReaderChain& rawReader);
    virtual ~IlcRawReaderChain();

    virtual Bool_t   NextEvent();
    virtual Bool_t   RewindEvents();
    virtual Bool_t   GotoEvent(Int_t event);
    virtual Int_t    GetNumberOfEvents() const;

    virtual TChain*  GetChain() const { return fChain; }
    //
    static const char* GetSearchPath()                               {return fgSearchPath;}
    static       void  SetSearchPath(const char* path="/ilc/data");
  protected :
    TChain*          fChain;        // root chain with raw events
    static TString   fgSearchPath;   // search path for "find"
    ClassDef(IlcRawReaderChain, 0) // class for reading raw digits from a root file
};

#endif
