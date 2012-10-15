#if !defined(__CINT__) || defined(__MAKECINT__)

// Root include files
#include <Riostream.h>
#include <TString.h>
#include <TChain.h>
#include <TSystem.h>

// IlcROOT include files
#include <IlcLog.h>

#else
#endif

// Define global parameters 
// const TString kgESDTreeName     = "esdTree" ;
// const UInt_t  kgeventsToRead    = 1 ;
// const char *  kgPattern         = "Evt" ;

TChain * IlcReadESDfromdisk(const UInt_t eventsToRead,
			       const TString dirName, 
			       const TString esdTreeName = "esdTree",
			       const char *  pattern     = "Evt") ;
TChain * IlcReadESD(const UInt_t eventsToRead = 1,
		     TString fileName  = "",
		     const TString esdTreeName = "esdTree",
		     const char *  pattern     = "Evt" ) ;

// Needed for IlcLog (return the macro name)
char * ClassName() { return "macro" ; } 
   
