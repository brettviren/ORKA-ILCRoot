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

// This class handles the mapping of the Altro channels
// The mapping is read from an external mapping files
// The class is used as a base class by TPC,PHOS and FMD
// Author: C.Cheshkov

#include "IlcAltroMapping.h"
#include "IlcLog.h"
#include <Riostream.h>


ClassImp(IlcAltroMapping)

//_____________________________________________________________________________
IlcAltroMapping::IlcAltroMapping():
  fIn(NULL),
  fNumberOfChannels(0),
  fMaxHWAddress(0),
  fMappingSize(0),
  fMapping(NULL)
{
  // Default constructor
}

//_____________________________________________________________________________
IlcAltroMapping::IlcAltroMapping(const char *mappingFile):
  fIn(NULL),
  fNumberOfChannels(0),
  fMaxHWAddress(0),
  fMappingSize(0),
  fMapping(NULL)
{
  // Constructor
  // Reads the mapping from an external file
  if (mappingFile)
    OpenMappingFile(mappingFile);
  else
    IlcFatal("Mapping file not specified !");
}

//_____________________________________________________________________________
IlcAltroMapping::~IlcAltroMapping()
{
  // destructor
  CloseMappingFile();

  if (fMapping) delete [] fMapping;
}

//_____________________________________________________________________________
Bool_t IlcAltroMapping::OpenMappingFile(const char *mappingFile)
{
  // Initalizes the ALTRO mapping from a file
  // Look at the TPC module for the format of
  // the mapping file
  fIn = new ifstream(mappingFile);
  if (!*fIn) {
    IlcFatal(Form("Missing mapping file (%s) !",mappingFile));
    CloseMappingFile();
    return kFALSE;
  }
  if (!(*fIn >> fNumberOfChannels)) {
    IlcFatal(Form("Syntax of the mapping file is wrong (%s) !",mappingFile));
    CloseMappingFile();
    return kFALSE;
  }
  if (!(*fIn >> fMaxHWAddress)) {
    IlcFatal(Form("Syntax of the mapping file is wrong (%s) !",mappingFile));
    CloseMappingFile();
    return kFALSE;
  }

  return kTRUE;
}

//_____________________________________________________________________________
void IlcAltroMapping::CloseMappingFile()
{
  // Closes the external mapping
  // file
  if (fIn) {
    fIn->close();
    delete fIn;
    fIn = NULL;
  }
}
