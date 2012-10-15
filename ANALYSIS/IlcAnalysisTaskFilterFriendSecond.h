#ifndef ILCANALYSISTASKFILTERFRIENDSECOND_H
#define ILCANALYSISTASKFILTERFRIENDSECOND_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/*$Id$*/

//*************************************************************************
// Class IlcAnalysisTaskFilterFriendSecond
// Test Task 
//*************************************************************************

#include <TString.h>
#include "IlcAnalysisTaskFilter.h"

class IlcAnalysisTaskFilterFriendSecond : public IlcAnalysisTaskFilter
{
 public:

  IlcAnalysisTaskFilterFriendSecond();
  IlcAnalysisTaskFilterFriendSecond(const char *name);
  virtual ~IlcAnalysisTaskFilterFriendSecond();


  // Implementation of interface methods
  virtual void UserCreateOutputObjects();
  virtual Bool_t UserSelectESDfriendForCurrentEvent();
  virtual void Init();
  virtual void LocalInit() {Init();}
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);

 private:

  IlcAnalysisTaskFilterFriendSecond(const IlcAnalysisTaskFilterFriendSecond &);
  IlcAnalysisTaskFilterFriendSecond& operator=(const IlcAnalysisTaskFilterFriendSecond&);

  IlcESDEvent  *fESDInput;        // ESD input object
  IlcESDfriend *fESDfriendInput;  // ESD input friend object
  ClassDef(IlcAnalysisTaskFilterFriendSecond,1); 
};

#endif

