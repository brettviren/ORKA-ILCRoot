* $Id: flap2.F 1074 2000-05-17 15:13:04Z fca $
C
C
        FUNCTION FLAP2(X)
#include "pact_hijing.inc"
        R=SQRT(BB**2+X**2)
        FLAP2=WDSAX2(R)
        RETURN
        END
