// @(#)root/eve:$Id: IlcEveMacroExecutor.h 37591 2009-12-04 11:48:09Z mtadel $
// Author: Matevz Tadel 2007

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#ifndef IlcEveMacroExecutor_H
#define IlcEveMacroExecutor_H

#include "TObject.h"

class IlcEveMacro;

//______________________________________________________________________________
// Short description of IlcEveMacroExecutor
//

class IlcEveMacroExecutor : public TObject
{
  friend class IlcEveMacroExecutorWindow;

public:
  IlcEveMacroExecutor();
  virtual ~IlcEveMacroExecutor();

  void         AddMacro(IlcEveMacro* mac);
  IlcEveMacro* FindMacro(const TString& func);

  void ExecMacros();

  void SaveAddedMacros();

  void RemoveMacros();

protected:
  TList*   fMacros;

private:
  IlcEveMacroExecutor(const IlcEveMacroExecutor&);            // Not implemented
  IlcEveMacroExecutor& operator=(const IlcEveMacroExecutor&); // Not implemented

  ClassDef(IlcEveMacroExecutor, 0); // Container for and executor of IlcEveMacros.
};

#endif
