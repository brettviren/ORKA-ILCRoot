*
* $Id: gauss64.F 7870 2003-07-14 13:35:23Z hristov $
*
#include "gen/pilot.h"
#if defined(CERNLIB_DOUBLE)
      FUNCTION DGAUSS(F,A,B,EPS)
#include "gen/imp64.inc"

      CHARACTER NAME*(*)
      PARAMETER (NAME = 'DGAUSS')
#include "gausscod.inc"

   99 DGAUSS=H
      RETURN
      END
#else
      SUBROUTINE DGAUSS_DUMMY
      END
#endif
