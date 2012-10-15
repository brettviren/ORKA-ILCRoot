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

/* $Id: IlcVertexGenFile.cxx 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Generator for vertices taken from a file                                  //
//                                                                           //
// The file name of the gilc file is passed as argument to the             //
// constructor. If a second argument is given, this determines the number    //
// of events for which the same vertex is used.                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include <TArrayF.h>
#include <TFile.h>
#include <TTree.h>

#include "IlcVertexGenFile.h"
#include "IlcLog.h"
#include "IlcGenEventHeader.h"
#include "IlcHeader.h"


ClassImp(IlcVertexGenFile)


//_____________________________________________________________________________
IlcVertexGenFile::IlcVertexGenFile() :
  fFile(NULL),
  fTree(NULL),
  fHeader(NULL),
  fEventsPerEntry(0),
  fEvent(0)
{
// default constructor: initialize data members

}

//_____________________________________________________________________________
IlcVertexGenFile::IlcVertexGenFile(const char* fileName, 
				   Int_t eventsPerEntry) :
  fFile(NULL),
  fTree(NULL),
  fHeader(NULL),
  fEventsPerEntry(eventsPerEntry),
  fEvent(0)
{
// main constructor:
// fileName is the name of the gilc file containing the vertices
// eventsPerEntry is the number of events for which the same vertex is used

  TDirectory* dir = gDirectory;

  fFile = TFile::Open(fileName);
  if (!fFile || !fFile->IsOpen()) {
    IlcError(Form("could not open file %s", fileName));
    delete fFile;
    fFile = NULL;
    return;
  }
  fTree = (TTree*) fFile->Get("TE");
  if (!fTree) {
    IlcError(Form("no header tree found in file %s", fileName));
    dir->cd();
    return;
  }
  fHeader = new IlcHeader;
  fTree->SetBranchAddress("Header", &fHeader);

  dir->cd();
}

//_____________________________________________________________________________
IlcVertexGenFile::~IlcVertexGenFile()
{
// clean up

  if (fFile) fFile->Close();
  delete fFile;
  delete fHeader;
}


//_____________________________________________________________________________
TVector3 IlcVertexGenFile::GetVertex()
{
// get the vertex from the event header tree

  Int_t entry = fEvent++ / fEventsPerEntry;
  if (!fTree) {
    IlcError("no header tree");
    return TVector3(0,0,0);
  }

  if (fTree->GetEntry(entry) <= 0) {
    IlcError(Form("error loading entry %d", entry));
    return TVector3(0,0,0);
  }

  if (!fHeader->GenEventHeader()) {
    IlcError("no generator event header");
    return TVector3(0,0,0);
  }

  TArrayF vertex(3);
  fHeader->GenEventHeader()->PrimaryVertex(vertex);
  return TVector3(vertex[0], vertex[1], vertex[2]);
}


