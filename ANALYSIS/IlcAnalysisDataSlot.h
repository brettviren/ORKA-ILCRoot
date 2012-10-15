#ifndef ILCANALYSISDATASLOT_H
#define ILCANALYSISDATASLOT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisDataSlot.h 18517 2007-05-09 14:44:59Z agheata $ */
// Author: Andrei Gheata, 31/05/2006

//==============================================================================
//   IlcAnalisysDataSlot - Class representing a data slot of an analysis task.
//      An analysis slot enforces a certain data type required by the Exec()
//      method of the analysis task. The slot must be connected to a data 
//      container with data of the same type.
//==============================================================================

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

class TClass;
class TTree;
class IlcAnalysisDataContainer;
class IlcAnalysisTask;

class IlcAnalysisDataSlot : public TNamed {

public:
   IlcAnalysisDataSlot() : TNamed(), fType(NULL), fParent(NULL), fContainer(NULL) {}
   IlcAnalysisDataSlot(TClass *type, IlcAnalysisTask *task);
   IlcAnalysisDataSlot(const IlcAnalysisDataSlot &slot);
   virtual ~IlcAnalysisDataSlot() {}

   // Assignment
   IlcAnalysisDataSlot &operator=(const IlcAnalysisDataSlot &slot);
   // Connect some container to the slot
   Bool_t                    ConnectContainer(IlcAnalysisDataContainer *cont);
   static Int_t              EnableBranch(const char *bname, TTree *tree);
   // Getters
   void                     *GetBranchAddress(const char *branch) const;
   Bool_t                    SetBranchAddress(const char *branch, void *address);
   TClass                   *GetType() const;
   IlcAnalysisTask          *GetParent() const    {return fParent;}
   IlcAnalysisDataContainer *GetContainer() const {return fContainer;}
   TObject                  *GetData() const;
   // Slot status checking
   Bool_t                    IsConnected() const  {return ((fContainer)?kTRUE:kFALSE);}
   Bool_t                    IsDataReady() const;

private:
   void                      SetType(TClass *type) {fType = type;}
   
protected:
   TClass                   *fType;       //! Type of the slot
   IlcAnalysisTask          *fParent;     // Analysis task to which the slot belongs
   IlcAnalysisDataContainer *fContainer;  // Container connected to the slot
   
   ClassDef(IlcAnalysisDataSlot,1)  // Class describing an analysis data slot
};
#endif
