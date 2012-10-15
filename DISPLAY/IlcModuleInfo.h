#ifndef ILCMODULEINFO_H
#define ILCMODULEINFO_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC MODULE INFORMATION CLASS                                      //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>

class IlcModuleInfo{
 public:
  IlcModuleInfo(int n);
  IlcModuleInfo(const IlcModuleInfo& rh);
  IlcModuleInfo& operator = (const IlcModuleInfo& rh);
  
  virtual ~IlcModuleInfo();

  void     SetId(const char* name,Int_t id);
  void     Add(const char * name,Int_t i);
  Int_t    Id(const char *name) const;
  const char*    Name(Int_t id) const;
  Bool_t   IsEnabled(Int_t id) const;
  Bool_t   IsEnabled(const char *name) const {return IsEnabled(Id(name));};
  void     Disable(Int_t id);
  void     Disable(const char* name){Disable(Id(name));};
  void     Enable(Int_t id);
  void     Enable(const char *name){Enable(Id(name));};
  void     Print() const;
  
 private:
  //The purposes of this class is to link each module to its Id
  char     **fName; // Array containing the names of the modules
  Int_t    *fId;    // Array of module's Ids
  Bool_t   *fEnabled; // Array of flags to enable/disable modules
  Int_t    fNb; // Number of modules

  ClassDef(IlcModuleInfo,0);
};

#endif
