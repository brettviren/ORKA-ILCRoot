#ifndef ILCJETKINEREADER_H
#define ILCJETKINEREADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
// Jet Kine Reader 
// MC Kinematics reader for jet analysis
// Author: Andreas Morsch (andreas.morsch@cern.ch)

#include "IlcJetReader.h"

class IlcRunLoader;
class IlcHeader;
class IlcMCEvent;
class TClonesArray;


class IlcJetKineReader : public IlcJetReader
{
 public: 
  IlcJetKineReader();
  virtual ~IlcJetKineReader();
  // Setters
  Bool_t  FillMomentumArray();
  void    OpenInputFiles();
  void    SetInputEvent(const TObject* esd, const TObject* aod, const TObject* mc);
  // Fast Simulation
  Float_t SmearMomentum(Int_t ind, Float_t p);
  Bool_t  Efficiency(Float_t pt, Float_t eta, Float_t phi);
  // Others
  TClonesArray*      GetGeneratedJets();
  virtual IlcHeader* GetIlcHeader() const {return fIlcHeader;}
  
 protected:
  IlcJetKineReader(const IlcJetKineReader& rJetKine);
  IlcJetKineReader& operator = (const IlcJetKineReader& rkr);

  IlcHeader          *fIlcHeader;       //! Header
  IlcMCEvent         *fMCEvent;  //! Monte Carlo Event Handler
  TClonesArray       *fGenJets;         //! List of generated jets
  ClassDef(IlcJetKineReader,1)
};
 
#endif
