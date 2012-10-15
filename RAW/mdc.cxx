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

/* $Id: mdc.cxx 26401 2008-06-04 16:34:44Z cvetan $ */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// c interface to IlcMDC                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "mdc.h"
#include "IlcMDC.h"
#include "IlcLog.h"

void* ilcmdcCreate(int compress, int filterMode, 
		   double maxSizeTagDB, const char* fileNameTagDB,
		   const char* guidFileFolder,
		   int basketsize)
{
// create an IlcMDC object

  return new IlcMDC(compress, kFALSE, IlcMDC::EFilterMode(filterMode), 
		    maxSizeTagDB, fileNameTagDB,
		    guidFileFolder,
		    basketsize);

}

int ilcmdcOpen(void* ilcmdc, int mode, const char* fileName)
{
// open a new raw DB

  return ((IlcMDC*)ilcmdc)->Open(IlcMDC::EWriteMode(mode), fileName);
}

int ilcmdcProcessEvent(void* ilcmdc, void* event, int isIovecArray)
{
// process one event

  return ((IlcMDC*)ilcmdc)->ProcessEvent(event, isIovecArray);
}

long long ilcmdcGetTotalFileSize(void* ilcmdc)
{
// return the total current file size

  return ((IlcMDC*)ilcmdc)->GetTotalSize();
}

long long ilcmdcClose(void* ilcmdc)
{
// close the raw DB

  return ((IlcMDC*)ilcmdc)->Close();
}

void  ilcmdcDelete(void* ilcmdc)
{
// delete the IlcMDC object

  delete (IlcMDC*)ilcmdc;
}

void  ilcmdcEnableDebug()
{
// enable debug and log messages

  IlcLog::EnableDebug(kTRUE);
  IlcLog::SetGlobalLogLevel(IlcLog::kMaxType);
  IlcLog::SetGlobalDebugLevel(IlcLog::kMaxType);
  IlcLog::SetPrintRepetitions(kFALSE);
  IlcLog::SetHandleRootMessages(kTRUE);
}

long long  ilcmdcAutoSave(void* ilcmdc)
{
  // Auto-save the raw-data tree

  return ((IlcMDC*)ilcmdc)->AutoSave();
}
  
