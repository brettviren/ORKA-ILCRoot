#ifndef ILCHOUGHFILTER_H
#define ILCHOUGHFILTER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHoughFilter.h 32401 2009-05-15 14:01:03Z cvetan $ */

///
/// high level filter algorithm for TPC using a hough transformation
///

#include "IlcFilter.h"

class TTree;

class IlcITSgeom;


class IlcHoughFilter: public IlcFilter {
public:
  IlcHoughFilter();

  virtual Bool_t       Filter(IlcRawVEvent* event, IlcESDEvent* esd);

  void                 RunITSclusterer(IlcRawVEvent* event, TTree *treeClusters);
  void                 RunITSvertexer(IlcESDEvent* esd, TTree *treeClusters);
  void                 RunTPCtracking(IlcRawVEvent* event, IlcESDEvent* esd);
  void                 RunITStracking(IlcESDEvent* esd, TTree *treeClusters);
private:
  IlcHoughFilter(const IlcHoughFilter&);
  IlcHoughFilter &operator=(const IlcHoughFilter&);

  Float_t fPtmin;        //Low limit on Pt

  IlcITSgeom *fITSgeom;  //Pointer to the ITS geometry

  ClassDef(IlcHoughFilter, 0)   // TPC hough filter
};

#endif
