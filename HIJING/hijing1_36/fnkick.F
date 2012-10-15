* $Id: fnkick.F 1074 2000-05-17 15:13:04Z fca $
C
C
C
	FUNCTION FNKICK(X)
#include "hiparnt.inc"
	FNKICK=1.0/(X+HIPR1(19)**2)/(X+HIPR1(20)**2)
     &		/(1+EXP((SQRT(X)-HIPR1(20))/0.4))
	RETURN
	END
