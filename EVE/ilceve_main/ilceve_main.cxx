// $Id: ilceve_main.cxx 58244 2012-08-21 12:57:19Z hristov $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007
// Mihai Niculescu 2012

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
#include <TEveBrowser.h>

#include <IlcEveApplication.h>
#include <IlcEveManager.h>

int main(int argc, char **argv)
{
  IlcEveApplication app("IlcEve", &argc, argv, 0 , -1);
  gEve = IlcEveManager::Create();
  
  // close application when closing EveWindow from X window button
	gEve->GetBrowser()->Connect("CloseWindow()", "TApplication", &app, "Terminate(=0)");
	
  // process command line arguments
	app.GetOptions(&argc, argv);
	
	app.Run();

	if(gEve){
	 delete gEve;
   gEve = 0;
	}
	
  return 0;
}
