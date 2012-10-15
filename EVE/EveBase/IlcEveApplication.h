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
 
#ifndef IlcEveApplication_H
#define IlcEveApplication_H

#include <TRint.h>

class IlcEveApplication : public TRint
{
public:
	IlcEveApplication(const char* appClassName, Int_t* argc, char** argv, void* options = 0, Int_t numOptions = 0, Bool_t noLogo = kFALSE);
	virtual ~IlcEveApplication();
	
	void Init(); // Initialize IlcEve & Rint Environment
	
private:
	IlcEveApplication(const IlcEveApplication&);               // not implemented
	IlcEveApplication& operator=(const IlcEveApplication&);    // not implemented
	
public:

	ClassDef(IlcEveApplication, 0); // IlcEve application
};
#endif
