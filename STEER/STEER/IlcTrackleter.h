#ifndef ILCTRACKLETER_H
#define ILCTRACKLETER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//   class IlcTrackleter
//   An abstract interface for tracklet reconstruction
//-------------------------------------------------------------------------

#include <TObject.h>
class TTree;
class IlcESDEvent;
class IlcMultiplicity;

class IlcTrackleter : public TObject {
public:
 IlcTrackleter() :fMult(0) {}
  virtual ~IlcTrackleter();
  virtual void Reconstruct(IlcESDEvent* esd, TTree* treeRP) = 0;
  virtual IlcMultiplicity* GetMultiplicity() const {return fMult;}
  //
protected:
 IlcTrackleter(const IlcTrackleter &src) : TObject(src), fMult(0) {}
  IlcTrackleter & operator=(const IlcTrackleter &src) {if (&src!=this) TObject::operator=(src); return *this;}
  
 protected:
  IlcMultiplicity* fMult;   // multiplicity object

  ClassDef(IlcTrackleter,1) //base trackleter
};

#endif
