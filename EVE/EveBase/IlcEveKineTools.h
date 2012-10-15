// $Id: IlcEveKineTools.h 24810 2008-03-31 16:33:43Z mtadel $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

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

#ifndef IlcEveKineTools_H
#define IlcEveKineTools_H

#include <TEveUtil.h>
#include <TObject.h>

class TTree;
class IlcStack;

class IlcEveKineTools
{
public:
  IlcEveKineTools() {}
  virtual ~IlcEveKineTools() {}

  void SetDaughterPathMarks(TEveElement* cont, IlcStack* stack, Bool_t recurse=kFALSE);
  void SetTrackReferences  (TEveElement* cont, TTree* treeTR=0, Bool_t recurse=kFALSE);
  void SortPathMarks       (TEveElement* cont, Bool_t recurse=kFALSE);

private:
  IlcEveKineTools(const IlcEveKineTools&);            // Not implemented
  IlcEveKineTools& operator=(const IlcEveKineTools&); // Not implemented

  ClassDef(IlcEveKineTools, 0); // Tools for import of kinematics.
};

#endif
