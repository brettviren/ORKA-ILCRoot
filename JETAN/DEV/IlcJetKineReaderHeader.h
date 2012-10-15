#ifndef ILCJETKINEREADERHEADER_H
#define ILCJETKINEREADERHEADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */
 
//----------------------------------------------------
// Jet Kinematics Reader Header
// Header for the reader in the jet analysis for Kinematics
// Author: Andreas Morsch (andreas.morsch@cern.ch)
//----------------------------------------------------

#include "IlcJetReaderHeader.h"
 
class IlcJetKineReaderHeader : public IlcJetReaderHeader
{
 public:
  IlcJetKineReaderHeader();
  virtual ~IlcJetKineReaderHeader() {}
  
  // Setters
  void    SetFastSimTPC(Bool_t flag = kTRUE)   {fFastSimTPC = flag;}   // if TPC fast simulation
  void    SetFastSimEMCAL(Bool_t flag = kTRUE) {fFastSimEMCAL = flag;} // if EMCAL fast simulation
  void    SetChargedOnly(Bool_t flag = kTRUE)  {fChargedOnly = flag;}  // for charged particles only, no smearing and no acceptance cuts

  // Getter
  Bool_t  FastSimTPC() const    {return fFastSimTPC;}
  Bool_t  FastSimEMCAL() const  {return fFastSimEMCAL;}
  Bool_t  ChargedOnly() const   {return fChargedOnly;}

 protected:
  //parameters set by user
  Bool_t  fFastSimTPC;               // TPC fast simulation flag
  Bool_t  fFastSimEMCAL;             // EMCAL fast simulation flag
  Bool_t  fChargedOnly;              // charged particles only flag

  ClassDef(IlcJetKineReaderHeader,3) // Kinematics reader header class
};
 
#endif
