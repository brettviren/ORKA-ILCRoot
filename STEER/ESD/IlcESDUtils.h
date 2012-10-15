// -*- mode: C++ -*- 
#ifndef ILCESDUTILS_H
#define ILCESDUTILS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcESDUtils.h 54304 2012-02-01 10:37:13Z shahoian $ */

//-------------------------------------------------------------------------
//   IlcESDUtils - This is a namespace that temporary provides general 
//                 purpose ESD utilities to avoid unnecessary dependencies
//                 between PWG libraries. To be removed/replaced by AODB
//                 framework.
//      
//-------------------------------------------------------------------------
// Author: Andrei Gheata

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
class IlcESDEvent;
class IlcVertexerTracks;

namespace IlcESDUtils {

  /********************************/
  /* Centrality-related corrections */
  /********************************/

  Float_t GetCorrV0(const IlcESDEvent* esd, Float_t &v0CorrResc, Float_t *v0multChCorr = NULL, Float_t *v0multChCorrResc = NULL);
  Float_t GetCorrSPD2(Float_t spd2raw,Float_t zv);
  Bool_t  RefitESDVertexTracks(IlcESDEvent* esdEv, Int_t algo=6);
}  

#endif
