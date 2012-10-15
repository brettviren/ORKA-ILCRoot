*
* $Id: gamma.F 7870 2003-07-14 13:35:23Z hristov $
*
#include "gen/pilot.h"
#if (defined(CERNLIB_DOUBLE))&&(!defined(CERNLIB_IBM))
      FUNCTION GAMMA(X)
      DOUBLE PRECISION DGAMMA,D

      SROUND(D)=D+(D-SNGL(D))
      GAMMA=SROUND(DGAMMA(DBLE(X)))
      RETURN
      END
#endif
