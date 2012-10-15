#ifndef ILCPVECCPVRAWWRITE_H
#define ILCPVECCPVRAWWRITE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: $ */

/* History:
 *
 * $Log$
 */

//_________________________________________________________________________
//  Create a raw data stream for the CPV detector
//  Input:  IlcPVECDigit or TClonesArray of IlcPVECDigits
//  Output: IlcFstream, a raw data stream in DDL format
//  Author: Yuri Kharlov
//  14 April 2008
//_________________________________________________________________________

// --- ROOT system ---

class TObject;
class TClonesArray;
class IlcFstream;
 
// --- Standard library ---

// --- IlcRoot header files ---
class IlcPVECDigit;

class IlcPVECCpvRawWrite : public TObject
{
public:

  IlcPVECCpvRawWrite() ;
  virtual ~IlcPVECCpvRawWrite() ;  
  void WriteRaw(const TObjArray *digits);
  void HWaddress(const IlcPVECDigit *digit, UInt_t &w32, Int_t &ddl, Int_t &row, Int_t &dilogic, Int_t &address);
  void WriteRowMarker (IlcFstream *ddl,UInt_t size);
  void WriteSegMarker (IlcFstream *ddl,UInt_t row, Int_t nwInSeg); 
  void WriteEoE       (IlcFstream *ddl,UInt_t row,UInt_t dil,UInt_t wordCnt); 

 protected:

  Int_t    fNDDL ;          // Number of DDLs
  Int_t    fNRow ;          // Number of row controllers per DDL
  Int_t    fNDilogic ;      // Number of DLOGIC chips per column
  Int_t    fNPad ;          // Number of pads per DLOGIC
  
  ClassDef(IlcPVECCpvRawWrite,1)  // CPV raw data writer

};

#endif // IlcPVECCPVRAWWRITE_H
