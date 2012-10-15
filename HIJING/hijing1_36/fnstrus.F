* $Id: fnstrus.F 1074 2000-05-17 15:13:04Z fca $
C
C
	FUNCTION FNSTRUS(X)
#include "hiparnt.inc"
	FNSTRUS=(1.0-X)**HIPR1(47)/
     &		(X**2+HIPR1(45)**2/HINT1(1)**2)**HIPR1(48)
	RETURN
	END
