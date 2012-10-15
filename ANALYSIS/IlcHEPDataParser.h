
#ifndef ILCHEPDATAPARSER_H
#define ILCHEPDATAPARSER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                      Implementation of   Class IlcHEPDataParser
//
//  This class is used to save the content of hisograms/graphs in the
//  HEP data format and viceversa
//  Author: Michele Floris, CERN
//-------------------------------------------------------------------------

#include "TObject.h"
#include "TString.h"

class TH1;
class TGraph;

class IlcHEPDataParser : public TObject{


public:
  IlcHEPDataParser();
  IlcHEPDataParser(TH1 * hStat, TH1 * hSyst);
  IlcHEPDataParser(TGraph * grStat, TGraph * grSyst);
  IlcHEPDataParser(const char * hepfileName);

  ~IlcHEPDataParser();
  
  TH1 * GetHistoStat() { return fHistStat;}
  TH1 * GetHistoSyst() { return fHistSyst;}
  TGraph * GetGraphStat() { return fGraphStat;}
  TGraph * GetGraphSyst() { return fGraphSyst;}
  void SaveHEPDataFile(const char * hepfileName, Bool_t trueUseGraphFalesUseHisto = 0);

  void SetName(const char * name) { fValueName = name;}

protected:

  TH1 * fHistStat; // statistical errors (hist)
  TH1 * fHistSyst; // systematic errors (hist) 
  TGraph * fGraphStat; // statistical errors (hist)
  TGraph * fGraphSyst; // systematic errors (hist)  
  TObjArray * fHEPDataFileLines;// TClones array of TObjString
  TString fValueName; // title for the y axis on the ascii file


  ClassDef(IlcHEPDataParser, 1);
    
private:


  IlcHEPDataParser(const IlcHEPDataParser&);
  IlcHEPDataParser& operator=(const IlcHEPDataParser&);
};

#endif
