* $Id: fnstrum.F 1074 2000-05-17 15:13:04Z fca $
C
C
C
	FUNCTION FNSTRUM(X)
#include "hiparnt.inc"
	FNSTRUM=1.0/((1.0-X)**2+HIPR1(45)**2/HINT1(1)**2)**HIPR1(46)
     &          /(X**2+HIPR1(45)**2/HINT1(1)**2)**HIPR1(46)
	RETURN
	END
