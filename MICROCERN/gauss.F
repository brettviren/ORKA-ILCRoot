*
* $Id: gauss.F 7870 2003-07-14 13:35:23Z hristov $
*
#include "gen/pilot.h"
C This will be GAUSS,IF=DOUBLE and GAUSS64,IF=-DOUBLE.
      FUNCTION GAUSS(F,A,B,EPS)

      CHARACTER NAME*(*)
      PARAMETER (NAME = 'GAUSS')
#include "gausscod.inc"
   99 GAUSS=H
      RETURN
      END
