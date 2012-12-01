#ifdef __CINT__
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: DCHbaseLinkDef.h,v 1.3 2008/04/22 09:43:37 tassiell Exp $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class  IlcDCHgeometry+;
#pragma link C++ class  IlcDCHParam+;
#pragma link C++ class  IlcDCHwireposition+;
#pragma link C++ class  IlcDCHwiredata+;
#pragma link C++ class  IlcDCHXTREL+;
#pragma link C++ class  XTDataVlGas+;

//#pragma link C++ class  IlcDCHXTDataVlGas+;
 
#pragma link C++ class  IlcDCHhit+;

#pragma link C++ class  IlcDCHHitWireStat+;

#pragma link C++ class  IlcDCH+;
#pragma link C++ class  IlcDCHv1+;

 
#pragma link C++ class IlcDCHclusterizer+;
#pragma link C++ class IlcDCHcluster+;


#pragma link C++ class IlcComplexCluster+;
#pragma link C++ class IlcDCHTrackerPoint+;   // defined in IlcComplexCluster.h
#pragma link C++ class IlcDCHClusterPoint+;   // defined in IlcComplexCluster.h
#pragma link C++ class IlcDCHExactPoint+;     // defined in IlcComplexCluster.h
#pragma link C++ class IlcDCHTrackPoint+;     // defined in IlcComplexCluster.h
#pragma link C++ class IlcDCHTrackPoint2+;    // defined in IlcComplexCluster.h
#pragma link C++ class IlcDCHTrackPointRef+;  // defined in IlcComplexCluster.h
			  
#pragma link C++ class IlcDCHtrack+;
#pragma link C++ class IlcDCHseed+;
#pragma link C++ class IlcDCHpolyTrack+;
#pragma link C++ class IlcDCHrecoParam+;
#pragma link C++ class IlcDCHRecoParam+;;
#pragma link C++ class IlcDCHReconstructor+;
#pragma link C++ class IlcDCHtracker+;

 
#pragma link C++ class IlcDCHFast+;

#endif
