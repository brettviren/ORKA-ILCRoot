#ifndef MDC_H
#define MDC_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: mdc.h 26401 2008-06-04 16:34:44Z cvetan $ */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// c interface to IlcMDC                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

void* ilcmdcCreate(int compress, int filterMode, 
		   double maxSizeTagDB, const char* fileNameTagDB,
		   const char* guidFileFolder,
		   int basketsize);
int   ilcmdcOpen(void* ilcmdc, int mode, const char* fileName);
int   ilcmdcProcessEvent(void* ilcmdc, void* event, int isIovecArray);
long long  ilcmdcGetTotalFileSize(void* ilcmdc);
long long  ilcmdcClose(void* ilcmdc);
void  ilcmdcDelete(void* ilcmdc);
void  ilcmdcEnableDebug();
long long  ilcmdcAutoSave(void* ilcmdc);

#ifdef __cplusplus
}
#endif

#endif
