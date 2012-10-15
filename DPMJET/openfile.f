      SUBROUTINE DPMJET_OPENINP()
*
*----------------------------------------------------------------------*
* Opens a file with a given unit number
*
*
* IOUNIT: Input unit to be assiged to the file
* FILNAM: Name of the file
*
*----------------------------------------------------------------------*
*
      IMPLICIT NONE
      LOGICAL EXISTS
      INTEGER LNROOT
      INTEGER LNBLNK
      CHARACTER*1000 FILNAM
      CHARACTER*1000 CHROOT
      CHROOT=' '
      CALL GETENVF('ILC_ROOT',CHROOT)
      LNROOT = LNBLNK(CHROOT)
      IF(LNROOT.LE.0) THEN
         FILNAM='dpmjet.dat'
      ELSE
         FILNAM=CHROOT(1:LNROOT)//'/DPMJET/dpmjet.dat'
      ENDIF
      INQUIRE(FILE=FILNAM,EXIST=EXISTS)
      IF(.NOT.EXISTS) THEN
         PRINT*,'***********************************'
         PRINT*,'*           openfile              *'
         PRINT*,'*        ---------------          *'
         PRINT*,'*   File dpmjet.dat not found     *'
         PRINT*,'*         Program STOP            *'
         PRINT*,'*   Check ILC_ROOT environment  *'
         PRINT*,'*           variable              *'
         PRINT*,'***********************************'
         STOP
      ENDIF
      OPEN (10, FILE="dpmjet.inp", STATUS="OLD")
      OPEN (23, FILE=FILNAM, STATUS="OLD")
      RETURN
      END
