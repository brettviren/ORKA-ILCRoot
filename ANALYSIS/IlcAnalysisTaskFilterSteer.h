#ifndef ILCANALYSISTASKFILTERSTEER_H
#define ILCANALYSISTASKFILTERSTEER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/*$Id$*/

//*************************************************************************
// Class IlcAnalysisTaskFilterSteer
// Implementing the filtering of the friend in order to reduce them to 1%
// 
//*************************************************************************

#include "IlcAnalysisTaskFilter.h"

class IlcAnalysisTaskFilterSteer : public IlcAnalysisTaskFilter
{
 public:

  IlcAnalysisTaskFilterSteer();
  IlcAnalysisTaskFilterSteer(const char *name);
  virtual ~IlcAnalysisTaskFilterSteer();

  // Implementation of interface methods
  virtual void UserCreateOutputObjects();
  virtual Bool_t UserSelectESDfriendForCurrentEvent();
  virtual void Init();
  virtual void LocalInit() {Init();}
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);
  void SetFraction(Double_t fraction) {fFraction = fraction;}
  Double_t GetFraction() const {return fFraction;}

 private:

  IlcAnalysisTaskFilterSteer(const IlcAnalysisTaskFilterSteer &);
  IlcAnalysisTaskFilterSteer& operator=(const IlcAnalysisTaskFilterSteer&);

  Double_t      fFraction;    // fraction of events for which to keep the friends
  //
  IlcESDEvent  *fESDInput;        // ESD input object
  IlcESDfriend *fESDfriendInput;  // ESD input friend object
  ClassDef(IlcAnalysisTaskFilterSteer,1); 
};

#endif

