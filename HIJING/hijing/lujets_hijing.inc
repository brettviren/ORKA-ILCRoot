* $Id: lujets_hijing.inc 11585 2005-04-29 01:47:59Z alibrary $
      REAL P, V
      COMMON/LUJETS_HIJING/N,K(9000,5),P(9000,5),V(9000,5)
#ifndef BLANKET_SAVE
      SAVE /LUJETS_HIJING/ 
#endif
