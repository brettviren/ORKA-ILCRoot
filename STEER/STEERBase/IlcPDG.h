#ifndef ILC_PDG__H
#define ILC_PDG__H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPDG.h 50615 2011-07-16 23:19:19Z hristov $ */

#include "TObject.h"

// Class to encapsulate the ILC updates to TDatabasePDG.h
// Can be used by TGeant3 and TGeant4
// Comments to: andreas.morsch@cern.ch 

class IlcPDG : public TObject {
public:
    static void AddParticlesToPdgDataBase();
 private:    
    ClassDef(IlcPDG,1)  // PDG database related information
};


#endif //ILC_PDG__H
