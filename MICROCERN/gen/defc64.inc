*
* $Id: defc64.inc 7865 2003-07-13 09:26:14Z hristov $
*
* $Log$
* Revision 1.2.4.1  2002/11/26 16:50:55  hristov
* Merging NewIO with v3-09-04
*
* Revision 1.2  2002/10/14 14:57:11  hristov
* Merging the VirtualMC branch to the main development branch (HEAD)
*
* Revision 1.1.2.1  2002/07/11 17:14:48  alibrary
* Adding MICROCERN
*
* Revision 1.1.1.1  1999/05/18 15:55:35  fca
* IlcRoot sources
*
* Revision 1.1.1.1  1996/04/01 15:02:59  mclareni
* Mathlib gen
*
*
* defc64.inc
*
#if (defined(CERNLIB_DOUBLE))&&(defined(CERNLIB_F90))
      COMPLEX(2)

#elif !defined(CERNLIB_DOUBLE)
      COMPLEX

#elif 1
      COMPLEX*16

#endif
