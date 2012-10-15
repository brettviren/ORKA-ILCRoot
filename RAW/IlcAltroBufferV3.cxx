/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// Interface to the Altro format
// to read and write digits
// To be used in Ilc Data Challenges 
// and in the compression of the RAW data

#include "IlcAltroBufferV3.h"
#include "IlcRawDataHeaderSim.h"
#include "IlcLog.h"
#include "IlcFstream.h"

ClassImp(IlcAltroBufferV3)

//_____________________________________________________________________________
IlcAltroBufferV3::IlcAltroBufferV3(const char* fileName, IlcAltroMapping *mapping):
IlcAltroBuffer(fileName,mapping),
  fN(0),
  fFECERRA(0),	
  fFECERRB(0),	
  fERRREG2(0),	
  fERRREG3(0),	
  fActiveFECsA(0xffff),
  fActiveFECsB(0xffff),
  fALTROCFG1(0),	
  fALTROCFG2(0),	
  fTSample(0),	
  fL1Phase(0)
{
  // Constructor
  memset(fArray, 0, kMaxWords*sizeof(UShort_t));
}

//_____________________________________________________________________________
IlcAltroBufferV3::~IlcAltroBufferV3()
{
// destructor
}

//_____________________________________________________________________________
IlcAltroBufferV3::IlcAltroBufferV3(const IlcAltroBufferV3& source):
  IlcAltroBuffer(source),
  fN(source.fN),
  fFECERRA(source.fFECERRA),	
  fFECERRB(source.fFECERRB),	
  fERRREG2(source.fERRREG2),	
  fERRREG3(source.fERRREG3),	
  fActiveFECsA(source.fActiveFECsA),
  fActiveFECsB(source.fActiveFECsB),
  fALTROCFG1(source.fALTROCFG1),	
  fALTROCFG2(source.fALTROCFG2),	
  fTSample(source.fTSample),	
  fL1Phase(source.fL1Phase)
{
// Copy Constructor

  Fatal("IlcAltroBufferV3", "copy constructor not implemented");
}

//_____________________________________________________________________________
IlcAltroBufferV3& IlcAltroBufferV3::operator = (const IlcAltroBufferV3& /*source*/)
{
//Assigment operator
#if 0
  fFECERRA	= source.fFECERRA;	
  fFECERRB	= source.fFECERRB;	
  fERRREG2	= source.fERRREG2;	
  fERRREG3	= source.fERRREG3;	
  fActiveFECsA	= source.fActiveFECsA;
  fActiveFECsB	= source.fActiveFECsB;
  fALTROCFG1	= source.fALTROCFG1;	
  fALTROCFG2	= source.fALTROCFG2;	
  fTSample	= source.fTSample;	
  fL1Phase      = source.fL1Phase;
#endif

  Fatal("operator =", "assignment operator not implemented");
  return *this;
}

//_____________________________________________________________________________
void IlcAltroBufferV3::FillBuffer(Int_t val)
{
//Fills the Buffer with 16 ten bits words and write into a file 

  if ((val > 0x3FF) || (val < 0)) {
    Error("FillBuffer", "Value out of range (10 bits): %d", val);
    val = 0x3FF;
  }

  if (fN >= (kMaxWords-1)) {
    Error("FillBuffer","Altro channel can't have more than 1024 10-bit words!");
    return;
  }

  fArray[fN++] = val;
}

//_____________________________________________________________________________
void IlcAltroBufferV3::WriteTrailer(Int_t wordsNumber, Short_t hwAddress)
{
  //Writes a trailer (header) of 32 bits using
  //a given hardware adress
  UInt_t temp = hwAddress & 0xFFF;
  temp |= ((wordsNumber & 0x3FF) << 16);
  temp |= (0x1U << 30);

  fFile->WriteBuffer((char *)(&temp),sizeof(UInt_t));

  ReverseAndWrite();
}

//_____________________________________________________________________________
UInt_t IlcAltroBufferV3::SetField(UInt_t& input, UShort_t start, UInt_t mask, UInt_t val) const
{
  UInt_t out = (mask << start);
  UInt_t fld = (val << start) & out;
  input &= ~out;
  input |= fld;
  return input;
}

//_____________________________________________________________________________
void IlcAltroBufferV3::ReverseAndWrite()
{
  // Reverse the altro data order and
  // write the buffer to the file
  UInt_t temp = 0;
  Int_t shift = 20;
  for(Int_t i = (fN-1); i >= 0; i--) {
    temp |= (fArray[i] << shift);
    shift -= 10;
    if (shift < 0) {
      fFile->WriteBuffer((char *)(&temp),sizeof(UInt_t));
      temp = 0;
      shift = 20;
    }
  }

  if (shift != 20) {
    fFile->WriteBuffer((char *)(&temp),sizeof(UInt_t));
  }

  fN = 0;
}

//_____________________________________________________________________________
UChar_t IlcAltroBufferV3::WriteRCUTrailer(Int_t rcuId)
{
  // Writes the RCU trailer
  // rcuId the is serial number of the corresponding
  // RCU. The basic format of the trailer can be
  // found in the RCU manual.
  // This method should be called at the end of
  // raw data writing.

  UInt_t currentFilePos = fFile->Tellp();
  UInt_t size = currentFilePos-fDataHeaderPos;
  size -= sizeof(IlcRawDataHeader);
  size /= 4;
  
  if (size > 0x3FFFFFF) {
    IlcFatal(Form("The current raw data payload size of %d is bigger than the max possible one ! Can not write the RCU trailer !",size));
    return 2;
  }

  // Now add the the RCU trailer tag
  size |= (1U << 31);
  fFile->WriteBuffer((char *)(&size),sizeof(UInt_t));

  // Now several well defined fields contained
  // in the trailer
  // For details check the RCU manual
  UInt_t buffer;

  buffer  = (0x1U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));

  buffer =  (fERRREG2 & 0x3FFFFFF);
  buffer |= (0x2U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));
  
  buffer =  (fERRREG3 & 0x3FFFFFF);
  buffer |= (0x3U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));

  buffer  = (fActiveFECsA & 0x3FFFFFF);
  buffer |= (0x4U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t)); 

  buffer  = (fActiveFECsB & 0x3FFFFFF);
  buffer |= (0x5U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));

  buffer  = (fALTROCFG1 & 0x3FFFFFF);
  buffer |= (0x6U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));
  
  buffer  = (fALTROCFG2 & 0x3FFFFFF);
  buffer |= (0x7U << 26);
  buffer |= (0x1U << 31);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));
  
  //  Now the RCU identifier and size of the trailer
  buffer = (9 & 0x7F);
  buffer |= ((rcuId & 0x1FF) << 7);
  buffer |= (0x2U << 16);
  buffer |= (0x8U << 26);
  buffer |= (0x3U << 30);
  fFile->WriteBuffer((char *)(&buffer),sizeof(UInt_t));

  return 2;
}
