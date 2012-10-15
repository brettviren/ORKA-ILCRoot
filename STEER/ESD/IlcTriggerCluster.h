#ifndef ILCTRIGGERCLUSTER_H
#define ILCTRIGGERCLUSTER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTriggerCluster.h 50636 2011-07-18 11:27:31Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define a Trigger Cluster  
//
//  A Trigger Cluster is a group of detector to be trigger together
//
//////////////////////////////////////////////////////////////////////////////
class TObject;
class TString;

class IlcTriggerCluster : public TNamed {

public:
                          IlcTriggerCluster();
			  IlcTriggerCluster( TString & name, UChar_t index, TString & detectors );
			  IlcTriggerCluster( const IlcTriggerCluster &clus );
               virtual   ~IlcTriggerCluster() {}

  //  Getters
	    const char*   GetDetectorsInCluster() const { return GetTitle(); }
                Bool_t    IsDetectorInCluster( TString & det );
		UChar_t   GetClusterMask() const { return fClusterMask; }

          virtual void    Print( const Option_t* opt ="" ) const;

private:
	       UChar_t    fClusterMask; // The trigger cluster mask pattern
                Bool_t    IsSelected( TString detName, TString & detectors ) const;
		IlcTriggerCluster&   operator=(const IlcTriggerCluster& clus);

   ClassDef( IlcTriggerCluster, 1 )  // Define a Trigger Cluster
};

#endif
