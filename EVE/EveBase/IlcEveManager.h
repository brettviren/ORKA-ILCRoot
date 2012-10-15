// Author: Mihai Niculescu 2012

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          										 *
 * full copyright notice.                                                																						 *
 **************************************************************************/
 
#ifndef IlcEveManager_H
#define IlcEveManager_H

#include <TEveManager.h>

class IlcEveManager : public TEveManager
{
public:
	IlcEveManager(UInt_t w, UInt_t h, Bool_t map_window=kTRUE, Option_t* opt="FIV");
	~IlcEveManager();

	static IlcEveManager* Create(Bool_t map_window=kTRUE, Option_t* opt="FIV");
	
	void CloseEveWindow();
	void Terminate();
protected:
	void Init();

public:

	ClassDef(IlcEveManager, 0); // Eve application manager
};
#endif
