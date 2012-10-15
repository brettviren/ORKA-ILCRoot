* $Id: fnkick2.F 1074 2000-05-17 15:13:04Z fca $
C
C
	FUNCTION FNKICK2(X)
#include "hiparnt.inc"
	FNKICK2=X*EXP(-2.0*X/HIPR1(42))
	RETURN
	END
