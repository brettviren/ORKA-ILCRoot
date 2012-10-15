* $Id: hijflv.F 1074 2000-05-17 15:13:04Z fca $
C
C	
C***************************************
	SUBROUTINE HIJFLV(ID)
	ID=1
	RNID=RLU_HIJING(0)
	IF(RNID.GT.0.43478) THEN
		ID=2
		IF(RNID.GT.0.86956) ID=3
	ENDIF
	RETURN
	END
