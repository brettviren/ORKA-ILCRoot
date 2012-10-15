#ifndef ILCGLOBALQADATAMAKER_H
#define ILCGLOBALQADATAMAKER_H

/*
 The class for calculating the global (not detector specific) quality assurance.
 It reuses the following TLists from its base class 
    IlcQADataMaker::fRecPointsQAList (for keeping the track residuals)
    IlcQADataMaker::fESDsQAList      (for keeping global ESD QA data)
*/

#include "IlcQADataMakerRec.h"

class IlcESDEvent;

class IlcGlobalQADataMaker: public IlcQADataMakerRec {
public:
  enum {
    kEvt0,
    kClr0,kClr1,kClr2,kClr3,
    kTrk0,kTrk1,kTrk2,kTrk3,kTrk4,kTrk5,kTrk6,kTrk7,kTrk8,kTrk9,kTrk10,
    kK0on,kK0off,kL0on,kL0off,
    kPid0,kPid1,kPid2,kPid3,
    kMlt0,kMlt1,
    kLast
  };
  IlcGlobalQADataMaker(const Char_t *name="Global", 
                       const Char_t *title="Global QA data maker"):
	IlcQADataMakerRec(name,title) {;}
  IlcGlobalQADataMaker(const IlcQADataMakerRec& qadm):
	IlcQADataMakerRec(qadm) {;}

  void InitRecPointsForTracker() { InitRecPoints(); }
  void InitRecPoints();
  void InitESDs();

private:
	void   EndOfDetectorCycle(IlcQAv1::TASKINDEX_t, TObjArray ** list) ;

	void InitRaws(); 
  
  void InitRecoParams() ; 
  
	void MakeRaws(IlcRawReader* rawReader) ; 
  void MakeESDs(IlcESDEvent *event);

  void StartOfDetectorCycle() {;}

  IlcGlobalQADataMaker &operator=(const IlcGlobalQADataMaker &qadm);

  ClassDef(IlcGlobalQADataMaker,1)  // Global QA 
};

#endif
