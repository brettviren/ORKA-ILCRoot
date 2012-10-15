* $Id: bk.F 1074 2000-05-17 15:13:04Z fca $
C
C
C
	FUNCTION BK(X)
#include "besel_hijing.inc"
	BK=EXP(-X)*(X**2-X4**2)**2.50/15.0
	RETURN
	END
