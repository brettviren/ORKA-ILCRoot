* $Id: flap1.F 1074 2000-05-17 15:13:04Z fca $
C
C
        FUNCTION FLAP1(X)
#include "pact_hijing.inc"
        R=SQRT(BB**2+X**2)
        FLAP1=WDSAX1(R)
        RETURN
        END
