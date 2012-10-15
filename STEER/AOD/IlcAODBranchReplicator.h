#ifndef ILCAODBRANCHREPLICATOR_H
#define ILCAODBRANCHREPLICATOR_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice                               */

// $Id: IlcAODBranchReplicator.h 50615 2011-07-16 23:19:19Z hristov $

// Base class of an object used for the replication 
// (and possibly filtering) of one (or several) AOD branches.
//
// Author L. Aphecetche (Subatech)

#ifndef ROOT_TNamed
#  include "TNamed.h"
#endif

class IlcAODEvent;

class IlcAODBranchReplicator : public TNamed
{
public:
  IlcAODBranchReplicator(const char* name="", const char* title="");
  
  virtual ~IlcAODBranchReplicator();

  /// Return the list of object we manage
  virtual TList* GetList() const = 0;
  
  /// Replicate (and optionally filter) the given aod event
  virtual void ReplicateAndFilter(const IlcAODEvent& source) = 0;

  ClassDef(IlcAODBranchReplicator,1) // AOD branch replicator base class
};

#endif

#ifndef ILCAODBRANCHREPLICATOR_H
#define ILCAODBRANCHREPLICATOR_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice                               */

// $Id: IlcAODBranchReplicator.h 50615 2011-07-16 23:19:19Z hristov $

//
// Base class of an object used for the replication 
// (and possibly filtering) of one (or several) AOD branches.
//

#ifndef ROOT_TNamed
#  include "TNamed.h"
#endif

class IlcAODEvent;

class IlcAODBranchReplicator : public TNamed
{
public:
  IlcAODBranchReplicator(const char* name="", const char* title="");
  
  virtual ~IlcAODBranchReplicator();

  /// Return the list of object we manage
  virtual TList* GetList() const = 0;
  
  /// Replicate (and optionally filter) the given aod event
  virtual void ReplicateAndFilter(const IlcAODEvent& source) = 0;

  ClassDef(IlcAODBranchReplicator,1) // AOD branch replicator base class
};

#endif

#ifndef ILCAODBRANCHREPLICATOR_H
#define ILCAODBRANCHREPLICATOR_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice                               */

// $Id: IlcAODBranchReplicator.h 50615 2011-07-16 23:19:19Z hristov $

//
// Base class of an object used for the replication 
// (and possibly filtering) of one (or several) AOD branches.
//

#ifndef ROOT_TNamed
#  include "TNamed.h"
#endif

class IlcAODEvent;

class IlcAODBranchReplicator : public TNamed
{
public:
  IlcAODBranchReplicator(const char* name="", const char* title="");
  
  virtual ~IlcAODBranchReplicator();

  /// Return the list of object we manage
  virtual TList* GetList() const = 0;
  
  /// Replicate (and optionally filter) the given aod event
  virtual void ReplicateAndFilter(const IlcAODEvent& source) = 0;

  ClassDef(IlcAODBranchReplicator,1) // AOD branch replicator base class
};

#endif

