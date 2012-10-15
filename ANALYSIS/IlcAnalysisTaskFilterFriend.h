#ifndef ILCANALYSISTASKFILTERFRIEND_H
#define ILCANALYSISTASKFILTERFRIEND_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/*$Id$*/

//*************************************************************************
// Class IlcAnalysisTaskFilterFriend
// Test Task 
//*************************************************************************

#include "IlcAnalysisTaskFilter.h"

class IlcAnalysisTaskFilterFriend : public IlcAnalysisTaskFilter
{
 public:

  IlcAnalysisTaskFilterFriend();
  IlcAnalysisTaskFilterFriend(const char *name);
  virtual ~IlcAnalysisTaskFilterFriend();

  // Implementation of interface methods
  virtual void UserCreateOutputObjects();
  virtual Bool_t UserSelectESDfriendForCurrentEvent();
  virtual void Init();
  virtual void LocalInit() {Init();}
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);

 private:

  IlcAnalysisTaskFilterFriend(const IlcAnalysisTaskFilterFriend &);
  IlcAnalysisTaskFilterFriend& operator=(const IlcAnalysisTaskFilterFriend&);

  IlcESDEvent  *fESDInput;        // ESD input object
  IlcESDfriend *fESDfriendInput;  // ESD input friend object
  ClassDef(IlcAnalysisTaskFilterFriend,1); 
};

#endif

