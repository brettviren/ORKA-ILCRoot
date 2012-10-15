#ifdef __APPLE__
#ifndef __INTEL_COMPILER
#define STOP CALL EXIT !
#define stop CALL EXIT !
#endif
#endif
* $Id: ludat1_hijing.inc 16990 2007-02-20 20:47:11Z hristov $
      REAL PARU, PARJ
      COMMON/LUDAT1_HIJING/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
#ifndef BLANKET_SAVE
      SAVE /LUDAT1_HIJING/ 
#endif
