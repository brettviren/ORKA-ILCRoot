#ifndef ILCTARGETSIMULATIONFASTPOINTSV0_H
#define ILCTARGETSIMULATIONFASTPOINTSV0_H

#include "IlcTARGETsimulation.h"
/////////////////////////////////////////////////////////
//  fast simulation V0
/////////////////////////////////////////////////////////
class IlcTARGETmodule;
class IlcTARGETstatistics;
class TRandom;
class TClonesArray;

class IlcTARGETsimulationFastPointsV0 : public IlcTARGETsimulation
{

public:
  IlcTARGETsimulationFastPointsV0(); // default constructor
  IlcTARGETsimulationFastPointsV0(const char *dataType); // standard constructor
  IlcTARGETsimulationFastPointsV0(const IlcTARGETsimulationFastPointsV0 &);
  IlcTARGETsimulationFastPointsV0 & operator=(const IlcTARGETsimulationFastPointsV0 &);  
  virtual IlcTARGETsimulation& operator=(const IlcTARGETsimulation &)
    {return *this;};

  virtual ~IlcTARGETsimulationFastPointsV0(); 
  Bool_t CreateFastRecPoints(IlcTARGETmodule *mod,Int_t module,TRandom *rndm,
			   TClonesArray* recp);
private:

  void AddSPD(Float_t &e,IlcTARGETmodule *mod,Int_t trackNumber,TClonesArray* recp);
  void AddSDD(Float_t &e,IlcTARGETmodule *mod,Int_t trackNumber,TClonesArray* recp);
  void AddSSD(Float_t &e,IlcTARGETmodule *mod,Int_t trackNumber,TClonesArray* recp);

private:

  Int_t fNrecp;            //current number of  fast point
  IlcTARGETstatistics *fSx;   // pointer to IlcTARGETstatistics class
  IlcTARGETstatistics *fSz;   // pointer to IlcTARGETstatistics class

  ClassDef(IlcTARGETsimulationFastPointsV0,2) // Fast point simulator.

};

#endif
