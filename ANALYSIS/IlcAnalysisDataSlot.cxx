/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcAnalysisDataSlot.cxx 58023 2012-07-31 16:29:45Z hristov $ */
// Author: Andrei Gheata, 31/05/2006

//==============================================================================
//   IlcAnalisysDataSlot - Class representing a data slot of an analysis task.
//      An analysis slot enforces a certain data type required by the Exec()
//      method of the analysis task. The slot must be connected to a data 
//      container with data of the same type.
//
// The class should not be directly created by users - it is created by
// each IlcAnalysisTask when defining its input/output slots using:
//
//    IlcAnalysisTask::SetInput(Int_t index, TClass *type);
//    IlcAnalysisTask::SetOutput(TClass *type);
//
// An existing data contaner (IlcAnalysisDataContainer) can be connected to the
// input/output slots of an analysis task using:
//
//   IlcAnalysisModule::ConnectInput(IlcAnalysisTask *task, Int_t islot,
//                                   IlcAnalysisDataContainer *cont)
//   IlcAnalysisModule::ConnectOutput(IlcAnalysisTask *task,
//                                    IlcAnalysisDataContainer *cont)
// To connect a slot to a data container, the data types declared by both must
// match.
//==============================================================================

#include <Riostream.h>
#include <TROOT.h>
#include <TClass.h>
#include <TTree.h>
#include <TLeaf.h>

#include "IlcAnalysisDataSlot.h"
#include "IlcAnalysisTask.h"
#include "IlcAnalysisDataContainer.h"

using std::endl;
using std::cout;
ClassImp(IlcAnalysisDataSlot)

//______________________________________________________________________________
IlcAnalysisDataSlot::IlcAnalysisDataSlot(TClass *type, IlcAnalysisTask *task)
                    :TNamed(), 
                     fType(type),
                     fParent(task), 
                     fContainer(NULL)
{
// Default constructor.
   SetTitle(fType->GetName());
}

//______________________________________________________________________________
IlcAnalysisDataSlot::IlcAnalysisDataSlot(const IlcAnalysisDataSlot &slot)
                    :TNamed(slot), 
                     fType(NULL), 
                     fParent(slot.fParent), 
                     fContainer(slot.fContainer)
{
// Copy ctor.
   GetType();
}                        

//______________________________________________________________________________
IlcAnalysisDataSlot& IlcAnalysisDataSlot::operator=(const IlcAnalysisDataSlot &slot)
{
// Assignment
   if (&slot == this) return *this;
   TNamed::operator=(slot);
   GetType();
   fParent = slot.fParent;
   fContainer = slot.fContainer;   
   return *this;
}

//______________________________________________________________________________
Bool_t IlcAnalysisDataSlot::ConnectContainer(IlcAnalysisDataContainer *cont)
{
// Connect the data slot with a given container. The operation will succeed only
// if the type defined by the slot inherits from the type enforced by the container.
// The error message in case of failure is posted by the caller.
   if (!cont || !GetType()) return kFALSE;
   if (!fType->InheritsFrom(cont->GetType())) {
     cout<<"Data slot of type "<<GetTitle()<<" of task "<<fParent->GetName()<<" cannot be connected to data container "<<cont->GetName()<<" of type "<<cont->GetTitle()<<endl;
     //IlcError(Form("Data slot of type %s of task %s cannot be connected to data container %s of type %s", fType->GetName(), fParent->GetName(), cont->GetName(), cont->GetType()->GetName()));
      return kFALSE;
   }   
   fContainer = cont;
   return kTRUE;
}   

//______________________________________________________________________________
TClass *IlcAnalysisDataSlot::GetType() const
{
// Get class type for this slot.
   IlcAnalysisDataSlot *slot = (IlcAnalysisDataSlot*)this;
   if (!fType) slot->SetType(gROOT->GetClass(fTitle.Data()));
   if (!fType) printf("IlcAnalysisDataSlot: Unknown class: %s\n", GetTitle());
   return fType;
}
   
//______________________________________________________________________________
void *IlcAnalysisDataSlot::GetBranchAddress(const char *branchname) const
{
// Retrieve the address for a given branch. One should always test this before
// using SetBranchAddress because the address gets set by the first caller.
// Call this in MyTask::Init()
   if (!GetType()) return NULL;
   if (!fType->InheritsFrom(TTree::Class())) {
     cout<<"Cannot call GetBranchAddress() for data slot of task "<<fParent->GetName()<<" not pointing to tree-type data"<<endl;
     //IlcFatal(Form("Cannot call GetBranchAddress() for data slot of task %s not pointing to tree-type data", fParent->GetName()));
      return NULL;
   }
   if (!IsDataReady()) {
     cout<<"Cannot call GetBranchAddress() for data slot of task "<<fParent->GetName()<<" while data is not ready"<<endl;
     //IlcFatal(Form("Cannot call GetBranchAddress() for data slot of task %s while data is not ready", fParent->GetName()));
      return NULL;
   }
   TTree *tree = (TTree*)GetData();
   TBranch *br = tree->GetBranch(branchname);
   if (!br) {   
     cout<<"Branch "<<branchname<<" not found in tree "<<tree->GetName()<<" as input of task "<<fParent->GetName()<<"..."<<endl;
     //IlcFatal(Form("Branch %s not found in tree %s as input of task %s...", branchname, tree->GetName(), fParent->GetName()));
      return NULL;
   }
   return br->GetAddress();
}   

//______________________________________________________________________________
Int_t IlcAnalysisDataSlot::EnableBranch(const char *bname, TTree *tree)
{
// Static method to enable recursively a branch in a tree (why this in not in ROOT?)
   TBranch *branch = tree->GetBranch(bname);
   Int_t count = 0;
//   static Int_t indent = 0;
   if (!branch) return count;
//   TString s;
//   for (Int_t i=0; i<indent; i++) s += " ";
   count++;
//   printf("%sbranch %s: kDoNotProcess=%d\n",s.Data(), branch->GetName(), branch->TestBit(kDoNotProcess));
   branch->SetBit(kDoNotProcess, kFALSE);
   TIter next(branch->GetListOfBranches());
   TBranch *branch_sub;
   // Activate all sub-branches
//   indent++;
   while ((branch_sub=(TBranch*)next())) {
      count += IlcAnalysisDataSlot::EnableBranch(branch_sub->GetName(), tree);
   }
//   indent--;
   return count;   
}   

//______________________________________________________________________________
Bool_t IlcAnalysisDataSlot::SetBranchAddress(const char *branchname, void *address)
{
// Set a branch address for input tree. To be called during MyTask::Init()
// only if GetBranchAddress() returns a NULL pointer for a tree-type slot.
   if (GetBranchAddress(branchname)) {
      Error("SetBranchAddress","Branch address for %s already set by other task. Call first GetBranchAddress() in %s::ConnectInputData()",branchname, fParent->GetName());
      return kFALSE;
   }
   TTree *tree = (TTree*)GetData();
   tree->SetBranchAddress(branchname, address);
   return kTRUE;
}   
      
//______________________________________________________________________________
TObject *IlcAnalysisDataSlot::GetData() const
{
// Retreives the data from the container if it is ready.
   if (!fContainer) {
     //IlcError(Form("Data slot of type %s of task %s has no connected data container",fType->GetName(), fParent->GetName()));    
      return NULL;
   }
   if (!fContainer->IsDataReady()) return NULL;
   return fContainer->GetData();
}

//______________________________________________________________________________
Bool_t  IlcAnalysisDataSlot::IsDataReady() const
{
// Check if data for this slot is ready in its container.
   if (!fContainer) {
     cout<<"Data slot of type "<<GetTitle()<<" of task "<<fParent->GetName()<<" has no connected data container"<<endl;
     //IlcError(Form("Data slot of type %s of task %s has no connected data container",fType->GetName(), fParent->GetName()));    
      return kFALSE;
   }
   return fContainer->IsDataReady();
}
   
