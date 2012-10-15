#include "macros/IlcReadESD.h"

//======================================================================================
TChain * IlcReadESDfromdisk(const UInt_t eventsToRead, 
				   const TString dirName, 
				   const TString esdTreeName, 
				   const char *  pattern) 
{
  // Reads ESDs from Disk
 TChain *  rv = 0; 

  IlcInfo( Form("\nReading files in %s \nESD tree name is %s \nReading %d events", 
		dirName.Data(), esdTreeName.Data(), eventsToRead) ) ;
  
  // create a TChain of all the files 
  TChain * cESDTree = new TChain(esdTreeName) ; 

  // read from the directory file until the require number of events are collected
  void * from = gSystem->OpenDirectory(dirName) ;
  if (!from) {
    IlcError( Form("Directory %s does not exist") ) ;
    rv = 0 ;
  }
  else{ // reading file names from directory
    const char * subdir ; 
    // search all subdirectories witch matching pattern
    while( (subdir = gSystem->GetDirEntry(from))  && 
	   (cESDTree->GetEntries() < eventsToRead)) {
      if ( strstr(subdir, pattern) != 0 ) { 
	char file[200] ; 
        sprintf(file, "%s%s/IlcESDs.root", dirName.Data(), subdir); 	
	IlcInfo( Form("Adding %s\n", file) );
	cESDTree->Add(file) ;
      }
    } // while file
  
    IlcInfo( Form(" %d events read", cESDTree->GetEntriesFast()) ) ;
    rv = cESDTree ; 
    
  } // reading file names from directory
  return rv ; 
}

//======================================================================================
TChain * IlcReadESD(const UInt_t eventsToRead,
		  const TString dirName, 
		  const TString esdTreeName, 
		  const char *  pattern)  
{
  // Read IlcESDs files and return a Chain of events
 
  if ( dirName == "" ) {
    IlcError("Give the name of the DIR where to find files") ; 
    return 0 ; 
  }
  if ( esdTreeName == "" ) 
    return IlcReadESDfromdisk(eventsToRead, dirName) ;
  else if ( strcmp(pattern, "") == 0 )
    return IlcReadESDfromdisk(eventsToRead, dirName, esdTreeName) ;
  else 
    return IlcReadESDfromdisk(eventsToRead, dirName, esdTreeName, pattern) ;	    
}
