/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcHeader.cxx 51189 2011-08-19 14:01:33Z cvetan $ */

//-----------------------------------------------------------------------
//     Simulation event header class
//     Collaborates with IlcRun, IlcStack, and IlcGenReaderTreeK classes
//     Many other classes depend on it
//     Author:
//-----------------------------------------------------------------------


#include <stdio.h>
#include <TObjArray.h>

#include "IlcLog.h"
#include "IlcHeader.h"
#include "IlcDetectorEventHeader.h"
#include "IlcGenEventHeader.h"
    
 
ClassImp(IlcHeader)

//_______________________________________________________________________
IlcHeader::IlcHeader():
  fRun(-1),
  fNvertex(0),
  fNprimary(0),
  fNtrack(0),
  fEvent(0),
  fEventNrInRun(0),
  fTimeStamp(0),
  fStack(0),
  fGenHeader(0),
  fDetHeaders(0)
{
  //
  // Default constructor
  //
 }

//_______________________________________________________________________
IlcHeader::IlcHeader(const IlcHeader& head):
  TObject(head),
  fRun(-1),
  fNvertex(0),
  fNprimary(0),
  fNtrack(0),
  fEvent(0),
  fEventNrInRun(0),
  fTimeStamp(0),
  fStack(0),
  fGenHeader(0),
  fDetHeaders(0)
{
  //
  // Copy constructor
  //
  head.Copy(*this);
}

//_______________________________________________________________________
IlcHeader::IlcHeader(Int_t run, Int_t event):
  fRun(run),
  fNvertex(0),
  fNprimary(0),
  fNtrack(0),
  fEvent(event),
  fEventNrInRun(0),
  fTimeStamp(0),
  fStack(0),
  fGenHeader(0), 
  fDetHeaders(0) 
{
  //
  // Standard constructor
  //
}

//_______________________________________________________________________
IlcHeader::IlcHeader(Int_t run, Int_t event, Int_t evNumber):
  fRun(run),
  fNvertex(0),
  fNprimary(0),
  fNtrack(0),
  fEvent(event),
  fEventNrInRun(evNumber),
  fTimeStamp(0),
  fStack(0),
  fGenHeader(0), 
  fDetHeaders(0) 
{
  //
  // Standard constructor
  //
}

IlcHeader::~IlcHeader()
{
    //
    // Destructor
    //
    if (fDetHeaders) {
	fDetHeaders->Delete();
	delete fDetHeaders;
    }
    delete fGenHeader;
}



//_______________________________________________________________________
void IlcHeader::Reset(Int_t run, Int_t event)
{
  //
  // Resets the header with new run and event number
  //
  fRun=run;	
  fNvertex=0;
  fNprimary=0;
  fNtrack=0;
  fEvent=event;
  fTimeStamp=0;
  if (fDetHeaders) fDetHeaders->Delete();
}

//_______________________________________________________________________
void IlcHeader::Reset(Int_t run, Int_t event, Int_t evNumber)
{
  //
  // Resets the header with new run and event number
  //
  fRun=run;	
  fNvertex=0;
  fNprimary=0;
  fNtrack=0;
  fEvent=event;
  fEventNrInRun=evNumber;
  fTimeStamp=0;
  if (fDetHeaders) fDetHeaders->Clear();
}

//_______________________________________________________________________
void IlcHeader::Print(const char*) const
{
  //
  // Dumps header content
  //
  printf(
"\n=========== Header for run %d Event %d = beginning ======================================\n",
  fRun,fEvent);
  printf("              Number of Vertex %d\n",fNvertex);
  printf("              Number of Primary %d\n",fNprimary);
  printf("              Number of Tracks %d\n",fNtrack);
  printf("              Time-stamp %ld\n",fTimeStamp);
  printf(
  "=========== Header for run %d Event %d = end ============================================\n\n",
  fRun,fEvent);

}

//_______________________________________________________________________
IlcStack* IlcHeader::Stack() const
{
// Return pointer to stack
    return fStack;
}

//_______________________________________________________________________
void IlcHeader::SetStack(IlcStack* stack)
{
// Set pointer to stack
    fStack = stack;
}

//_______________________________________________________________________
void IlcHeader::SetGenEventHeader(IlcGenEventHeader* header)
{
// Set pointer to header for generated event
    fGenHeader = header;
}

void IlcHeader::AddDetectorEventHeader(IlcDetectorEventHeader* header)
{
// Add a detector specific header
//
//  Create the array of headers
    if (!fDetHeaders) fDetHeaders = new TObjArray(77);

//  Some basic checks

    if (!header) {
	Warning("AddDetectorEventHeader","Detector tries to add empty header \n");
	return;
    }
    
    if (strlen(header->GetName()) == 0) {
	Warning("AddDetectorEventHeader","Detector tries to add header without name \n");
	return;
    }
    
    TObject *mod=fDetHeaders->FindObject(header->GetName());
    if(mod) {
	Warning("AddDetectorEventHeader","Detector %s tries to add more than one header \n", header->GetName());
	return;
    }
    

//  Add the header to the list 
    fDetHeaders->Add(header);
}

IlcDetectorEventHeader* IlcHeader::GetDetectorEventHeader(const char *name) const
{
//
// Returns detector specific event header
//
    if (!fDetHeaders) {
	Warning("GetDetectorEventHeader","There are no  detector specific headers for this event");
	return 0x0;
    }
    return  dynamic_cast<IlcDetectorEventHeader*>(fDetHeaders->FindObject(name)) ;
}


//_______________________________________________________________________
IlcGenEventHeader*  IlcHeader::GenEventHeader() const
{
// Get pointer to header for generated event
    return fGenHeader;
}

//_______________________________________________________________________
void IlcHeader::Copy(TObject&) const
{
  IlcFatal("Not implemented");
}



