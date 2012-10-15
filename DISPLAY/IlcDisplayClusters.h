#ifndef ILCDISPLAYCLUSTERS_H
#define ILCDISPLAYCLUSTERS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
/////////////////////////////////////////////////////////////////////////
// ILC DISPLAY CLUSTERS CLASS                                        //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>

class TPolyMarker3D;

class IlcDisplayClusters{
  //This class is an interface to the clusters data

public:
 IlcDisplayClusters();
 virtual ~IlcDisplayClusters();

 void          LoadClusters(const char * name,Int_t nevent);
 void          LoadITSClusters(Int_t nevent);
 void          LoadTPCClusters(Int_t nevent);
 void          Draw();
 Int_t         GetNbClusters();

private: 
 TPolyMarker3D *fPoints; //fPoints[i]=set of cluster coordinates in detector i;
 Int_t         fNb;      //Nimber of clusters
 char          **fName; //fName[i]=name of the detector i 

 RQ_OBJECT("IlcDisplayClusters")

 ClassDef(IlcDisplayClusters,0);
};
#endif
