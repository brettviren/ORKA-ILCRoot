#ifdef __CINT__
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: TARGETsimLinkDef.h,v 1.3 2008/11/05 01:47:14 rucco Exp $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Standard TARGET classes

#pragma link C++ class  IlcTARGET+;
#pragma link C++ class  IlcTARGETvORKA+;
#pragma link C++ class  IlcTARGEThit+;
#pragma link C++ class  IlcTARGETGeant3Geometry+;
// Standard TARGET detector class initializers
#pragma link C++ class  IlcTARGETmodule+;
#pragma link C++ class  IlcTARGETsimulation+;
#pragma link C++ class  IlcTARGETsimulationSPD+;
#pragma link C++ class  IlcTARGETsimulationSDD+;
#pragma link C++ class  IlcTARGETsimulationSSD+;
#pragma link C++ class  IlcTARGETTableSSD+;
#pragma link C++ class  IlcTARGETsimulationFastPoints+;
#pragma link C++ class  IlcTARGETsimulationFastPointsV0+;
#pragma link C++ class  IlcTARGETDetTypeSim+;
#pragma link C++ class  IlcTARGETstatistics+;
#pragma link C++ class  IlcTARGETstatistics2+;
// These streamers must be formatted according to the raw data fromat

#pragma link C++ class  IlcTARGETHNode+;
#pragma link C++ class  IlcTARGETHTable+;
#pragma link C++ class  IlcTARGETetfSDD+;
// SSD simulation and reconstruction
#pragma link C++ class  IlcTARGETdcsSSD+;
#pragma link C++ class  IlcTARGETsDigitize+;
#pragma link C++ class  IlcTARGETDigitizer+;
#pragma link C++ class  IlcTARGETFDigitizer+;
// Raw data

#pragma link C++ class IlcTARGETTrigger+;



#endif
