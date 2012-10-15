#ifndef ILCANALYSISTASKMIXINFO_H
#define ILCANALYSISTASKMIXINFO_H

//
// Class IlcAnalysisTaskMixInfo
//
// IlcAnalysisTaskMixInfo is task
// for mixing info
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#include "IlcLog.h"

#include "IlcAnalysisTaskSE.h"

class IlcMixInputEventHandler;
class TList;
class IlcMixInfo;
class IlcAnalysisTaskMixInfo : public IlcAnalysisTaskSE {
public:
   IlcAnalysisTaskMixInfo(const char *name = "<default name>");
   virtual ~IlcAnalysisTaskMixInfo();

   virtual void    UserCreateOutputObjects();
   virtual void    UserExec(Option_t *option);
   virtual void    Terminate(Option_t *);
   virtual void    UserExecMix(Option_t *option = "");
   virtual void    FinishTaskOutput();

   void            InitInputHandlers();
   void            InitMixInfo();
   // sets log type to list of classes
   void            SetLogType(IlcLog::EType_t type, TString allClasses = "");
   // sets correctly debug level to IlcLog for all classes listed in fLogClassesString
   void            SetDebugForAllClasses();

   void            PrintEventInfo();

private:

   IlcMultiInputEventHandler  *fInputEHMain;       //! input multi handler
   IlcMixInputEventHandler    *fInputEHMix;        //! mix input handler

   TList                      *fOutputList;        //! output list
   IlcMixInfo                 *fMixInfo;           //! mix info

   Long64_t                    fCurrentEntryTmp;   //! temporary current entry number

   IlcLog::EType_t             fLogType;           // log type
   TString                     fLogClassesString;  // all classes string divided with ":"

   IlcAnalysisTaskMixInfo(const IlcAnalysisTaskMixInfo &); // not implemented
   IlcAnalysisTaskMixInfo &operator=(const IlcAnalysisTaskMixInfo &); // not implemented

   ClassDef(IlcAnalysisTaskMixInfo, 1); // example of analysis
};

#endif
