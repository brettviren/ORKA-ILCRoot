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

/* $Id: IlcAltroRawStream.cxx 42247 2010-07-12 14:06:37Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to Altro digits in raw data.
///
/// It loops over all Altro digits in the raw data given by the IlcRawReader.
/// The Next method goes to the next digit. If there are no digits left
/// it returns kFALSE.
/// Several getters provide information about the current digit.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcAltroRawStream.h"
#include "IlcRawReader.h"
#include "IlcLog.h"
#include "IlcRawEventHeaderBase.h"

ClassImp(IlcAltroRawStream)


//_____________________________________________________________________________
IlcAltroRawStream::IlcAltroRawStream(IlcRawReader* rawReader) :
  fIsShortDataHeader(kFALSE),
  fDDLNumber(-1),
  fPrevDDLNumber(-1),
  fRCUId(-1),
  fPrevRCUId(-1),
  fHWAddress(-1),
  fPrevHWAddress(-1),
  fTime(-1),
  fPrevTime(-1),
  fSignal(-1),
  fTimeBunch(-1),
  fRawReader(rawReader),
  fData(NULL),
  fPosition(0),
  fCount(0),
  fChannelPayloadSize(-1),
  fBunchLength(0),
  fRCUTrailerData(NULL),
  fRCUTrailerSize(0),
  fFECERRA(0),
  fFECERRB(0),
  fERRREG2(0),
  fERRREG3(0),
  fERRREG4(0),
  fActiveFECsA(0),
  fActiveFECsB(0),
  fAltroCFG1(0),
  fAltroCFG2(0)
{
// create an object to read Altro raw digits
}

//_____________________________________________________________________________
IlcAltroRawStream::IlcAltroRawStream(const IlcAltroRawStream& stream) :
  TObject(stream),
  fIsShortDataHeader(stream.fIsShortDataHeader),
  fDDLNumber(stream.fDDLNumber),
  fPrevDDLNumber(stream.fPrevDDLNumber),
  fRCUId(stream.fRCUId),
  fPrevRCUId(stream.fPrevRCUId),
  fHWAddress(stream.fHWAddress),
  fPrevHWAddress(stream.fPrevHWAddress),
  fTime(stream.fTime),
  fPrevTime(stream.fPrevTime),
  fSignal(stream.fSignal),
  fTimeBunch(stream.fTimeBunch),
  fRawReader(stream.fRawReader),
  fData(stream.fData),
  fPosition(stream.fPosition),
  fCount(stream.fCount),
  fChannelPayloadSize(stream.fChannelPayloadSize),
  fBunchLength(stream.fBunchLength),
  fRCUTrailerData(stream.fRCUTrailerData),
  fRCUTrailerSize(stream.fRCUTrailerSize),
  fFECERRA(stream.fFECERRA),
  fFECERRB(stream.fFECERRB),
  fERRREG2(stream.fERRREG2),
  fERRREG3(stream.fERRREG3),
  fERRREG4(stream.fERRREG4),
  fActiveFECsA(stream.fActiveFECsA),
  fActiveFECsB(stream.fActiveFECsB),
  fAltroCFG1(stream.fAltroCFG1),
  fAltroCFG2(stream.fAltroCFG2)
{
}

//_____________________________________________________________________________
IlcAltroRawStream& IlcAltroRawStream::operator = (const IlcAltroRawStream& stream)
{
  if(&stream == this) return *this;

  fIsShortDataHeader = stream.fIsShortDataHeader;
  fDDLNumber         = stream.fDDLNumber;
  fPrevDDLNumber     = stream.fPrevDDLNumber;
  fRCUId             = stream.fRCUId;
  fPrevRCUId         = stream.fPrevRCUId;
  fHWAddress         = stream.fHWAddress;
  fPrevHWAddress     = stream.fPrevHWAddress;
  fTime              = stream.fTime;
  fPrevTime          = stream.fPrevTime;
  fSignal            = stream.fSignal;
  fTimeBunch         = stream.fTimeBunch;
  fRawReader         = stream.fRawReader;
  fData              = stream.fData;
  fPosition          = stream.fPosition;
  fCount             = stream.fCount;
  fChannelPayloadSize= stream.fChannelPayloadSize;
  fBunchLength       = stream.fBunchLength;
  fRCUTrailerData    = stream.fRCUTrailerData;
  fRCUTrailerSize    = stream.fRCUTrailerSize;
  fFECERRA           = stream.fFECERRA;
  fFECERRB           = stream.fFECERRB;
  fERRREG2           = stream.fERRREG2;
  fERRREG3           = stream.fERRREG3;
  fERRREG4           = stream.fERRREG4;
  fActiveFECsA       = stream.fActiveFECsA;
  fActiveFECsB       = stream.fActiveFECsB;
  fAltroCFG1         = stream.fAltroCFG1;
  fAltroCFG2         = stream.fAltroCFG2;

  return *this;
}

//_____________________________________________________________________________
IlcAltroRawStream::~IlcAltroRawStream()
{
// clean up

}

//_____________________________________________________________________________
void IlcAltroRawStream::Reset()
{
// reset altro raw stream params

  fPosition = fCount = fBunchLength = 0;
  fChannelPayloadSize = -1;

  fRCUTrailerData = NULL;
  fRCUTrailerSize = 0;

  fFECERRA = fFECERRB = fERRREG2 = fERRREG3 = fERRREG4 = fActiveFECsA = fActiveFECsB = fAltroCFG1 = fAltroCFG2 = 0;

  fDDLNumber = fPrevDDLNumber = fRCUId = fPrevRCUId = fHWAddress = fPrevHWAddress = fTime = fPrevTime = fSignal = fTimeBunch = -1;

  if (fRawReader) fRawReader->Reset();
}

//_____________________________________________________________________________
Bool_t IlcAltroRawStream::Next()
{
// read the next raw digit
// returns kFALSE if there is no digit left

  fPrevDDLNumber = fDDLNumber;
  fPrevRCUId = fRCUId;
  fPrevHWAddress = fHWAddress;
  fPrevTime = fTime;

  while (fCount == 0) {  // next trailer
    while (fPosition <= 0) {  // next payload
      do {
	if (!fRawReader->ReadNextData(fData)) return kFALSE;
      } while (fRawReader->GetDataSize() == 0);

      fDDLNumber = fRawReader->GetDDLID();

      fPosition = GetPosition();
    }

    ReadTrailer();

    fBunchLength = 0;
  }

  if (fBunchLength == 0) ReadBunch();
  else fTime--;

  ReadAmplitude();

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcAltroRawStream::NextDDL(UChar_t *data)
{
  if (!data) {
    do {
      if (!fRawReader->ReadNextData(fData)) return kFALSE;
    } while (fRawReader->GetDataSize() == 0);
  }
  else {
    fData = data;
  }

  fDDLNumber = fRawReader->GetDDLID();
  fChannelPayloadSize = -1;
  fPosition = GetPosition();

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcAltroRawStream::NextChannel()
{
  if (fPosition <= 0) return kFALSE;

  ReadTrailer();

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcAltroRawStream::NextBunch(UShort_t *bunchData,
				    Int_t &bunchLength,
				    Int_t &startTimeBin)
{
  if (fCount == 0) return kFALSE;

  ReadBunch();
  bunchLength = fTimeBunch;
  startTimeBin = fTime;

  while (fBunchLength > 0) {
    ReadAmplitude();
    bunchData[bunchLength-fBunchLength-1] = fSignal;
  }

  return kTRUE;
}

//_____________________________________________________________________________
void IlcAltroRawStream::SelectRawData(Int_t detId)
{
  // Select the raw data for specific
  // detector id
  IlcDebug(1,Form("Selecting raw data for detector %d",detId));
  fRawReader->Select(detId);
}

//_____________________________________________________________________________
void IlcAltroRawStream::SelectRawData(const char *detName)
{
  // Select the raw data for specific
  // detector name
  IlcDebug(1,Form("Selecting raw data for detector %s",detName));
  fRawReader->Select(detName);
}

//_____________________________________________________________________________
UShort_t IlcAltroRawStream::GetNextWord()
{
  // Read the next 10 bit word in backward direction
  // The input stream access is given by fData and fPosition

  fPosition--;

  Int_t iBit = fPosition * 10;
  Int_t iByte = iBit / 8;
  Int_t shift = iBit % 8;

  // the raw data is written as integers where the low bits are filled first
  // -> little endian is assumed here !
  Int_t iByteLow = iByte;
  iByte++;
  Int_t iByteHigh  = iByte;
  return ((fData[iByteHigh] * 256 + fData[iByteLow]) >> shift) & 0x03FF;
}

//_____________________________________________________________________________
Bool_t IlcAltroRawStream::ReadTrailer()
{
  //Read a trailer of 40 bits in the backward reading mode
  //First reading filling words
  UShort_t temp;
  Int_t nFillWords = 0;
  while ((temp = GetNextWord()) == 0x2AA) nFillWords++;
  if (nFillWords == 0) {
    fRawReader->AddMajorErrorLog(kAltroTrailerErr,"no 0x2AA");
    //    PrintDebug();
    IlcWarning(Form("Incorrect trailer found ! Expected 0x2AA not found (0x%x != 0x2AA) ! Current position %d, DDL=%d",
		    temp,fPosition,fDDLNumber));
    // trying to recover and find the next bunch
    while ((fPosition > 2) && (temp != 0x2AA)) temp = GetNextWord();
    if (temp != 0x2AA) {
      fCount = fPosition = 0;
      return kFALSE;
    }
    temp = GetNextWord();
  }

  //Then read the trailer
  if (fPosition < 2) {
    fRawReader->AddMajorErrorLog(kAltroTrailerErr,Form("size %d < 5",
						       fPosition));
    //    PrintDebug();
    IlcWarning(Form("Incorrect raw data size ! Expected at least 5 words but found %d !",fPosition));
    fCount = fPosition = 0;
    return kFALSE;
  }

  fCount = (temp << 4) & 0x3FF;
  if ((temp >> 6) != 0xA) {
    fRawReader->AddMajorErrorLog(kAltroTrailerErr,"no 0xA");
    //    PrintDebug();
    IlcWarning(Form("Incorrect trailer found ! Expecting 0xA but found 0x%x !",temp >> 6));
    fCount = 0;
    return kFALSE;
  }

  temp = GetNextWord();
  fHWAddress = (temp & 0x3) << 10;
  if (((temp >> 2) & 0xF) != 0xA) {
    fRawReader->AddMajorErrorLog(kAltroTrailerErr,"no second 0xA");
    //    PrintDebug();
    IlcWarning(Form("Incorrect trailer found ! Expecting second 0xA but found 0x%x !",(temp >> 2) & 0xF));
    fCount = 0;
    return kFALSE;
  }
  fCount |= ((temp & 0x3FF) >> 6);
  fChannelPayloadSize = fCount;

  if (fCount >= fPosition) {
    fRawReader->AddMajorErrorLog(kAltroTrailerErr,"invalid size");
    //    PrintDebug();
    IlcWarning(Form("Incorrect trailer found ! The altro payload size is invalid (%d >= %d) !",fCount,fPosition));
    fCount = 0;
    return kFALSE;
  }
  temp = GetNextWord();
  fHWAddress |= temp;

  fPosition -= (4 - (fCount % 4)) % 4;  // skip fill words

  return kTRUE;
}

//_____________________________________________________________________________
void IlcAltroRawStream::ReadBunch()
{
  // Read altro payload in 
  // backward direction
  if (fCount <= 2) {
    fRawReader->AddMinorErrorLog(kBunchLengthReadErr,"");
    //    PrintDebug();
    IlcWarning(Form("Could not read bunch length and time bin ! Only %d 10-bit words are left !",fCount));
    fBunchLength = fTimeBunch = fCount = 0;
    return;
  }

  fBunchLength = GetNextWord() - 2;
  if (fBunchLength > fCount) {
    fRawReader->AddMinorErrorLog(kBunchLengthReadErr,Form("bl=%d",fBunchLength));
    //    PrintDebug();
    IlcWarning(Form("Could not read bunch length ! Bunch length = %d (>%d)",fBunchLength,fCount));
    fBunchLength = fTimeBunch = fCount = 0;
    return;
  }
  fTimeBunch = fBunchLength;
  fCount--;

  fTime = GetNextWord();
  fCount--;

  return;
}

//_____________________________________________________________________________
void IlcAltroRawStream::ReadAmplitude()
{
  // Read next time bin amplitude
  if (fCount <= 0) {
    fRawReader->AddMinorErrorLog(kAmplitudeReadErr,"");
    //    PrintDebug();
    IlcWarning("Could not read sample amplitude !");
    fCount = fSignal = fBunchLength = 0;
    return;
  }

  fSignal = GetNextWord();

  fCount--;
  fBunchLength--;

  return;
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::GetPosition()
{
  // Sets the position in the
  // input stream
  // Read the RCU trailer
  // This includes the trailer size,
  // RCU identifier and raw data payload.
  // The RCU trailer format is described
  // in details in the RCU manual.

  // We use the last word of the payload
  // in order to decide which RCU firmware
  // was used during the data taking.
  // The firmware v2 adds 0xAAAA as 16
  // most significant bits and since the
  // payload size (firmware v1) can not be
  // that big, we use this as a unique
  // label of the firmware version.

  Int_t index = fRawReader->GetDataSize();
  UInt_t word = Get32bitWord(index);
  if (((word >> 16) == 0xaaaa) || (word == 2)) {
    // This is RCU formware v2
    // The statement word==2 is needed only temporary
    // in order to be able to read previously generated
    // ilcroot raw data

    Int_t trailerSize = 0;
    if (word == 2) {
      IlcInfo("Old simulated raw data is assumed!");
      trailerSize = 2;
      fRCUId = 0;
    }
    else {
      // First read 32-bit word with the
      // trailer size (7 bits), RCU ID (9 bits) and
      // 0xAAA (the rest - 16 bits)
      fRCUId = (Int_t)((word >> 7) & 0x1ff);
      trailerSize = (word & 0x7F);
    }

    // Now read the beginning of the trailer
    // where the payload size is written
    if (trailerSize < 2) {
      fRawReader->AddMajorErrorLog(kRCUTrailerErr,Form("tr=%d bytes",
						       trailerSize*4));
      IlcWarning(Form("Invalid trailer size found (%d bytes) !",
		      trailerSize*4));
    }

    Int_t position = ReadRCUTrailer(index,trailerSize);
    if (fRawReader->GetType() != IlcRawEventHeaderBase::kStartOfData) {
      // The size is specified in a number of 40bits
      // Therefore we need to transform it to number of bytes
      position *= 5;

      // Check the consistency of the header and trailer
      if (((fRawReader->GetDataSize() - trailerSize*4) < position) ||
	  ((fRawReader->GetDataSize() - trailerSize*4) >= (position + 4))) {
	fRawReader->AddMajorErrorLog(kRCUTrailerSizeErr,Form("h=%d tr=%d rcu=%d bytes",
							     fRawReader->GetDataSize(),
							     trailerSize*4,
							     position));
	IlcWarning(Form("Inconsistent raw data size ! Raw data size - %d bytes (from the header), RCU trailer - %d bytes, raw data paylod - %d bytes !",
			fRawReader->GetDataSize(),
			trailerSize*4,
			position));
	position = fRawReader->GetDataSize() - trailerSize*4;
      }

      return position * 8 / 10;
    }
    else {
      // Special RCU payload in case of SOD events
      // The decoding is left to the user code
      // Here we just retrieve the payload size
      return position;
    }
  }
  else {
    // In case of the Old RCU trailer format
    // we have to read just the size of altro payload
    // in units of 40-bit words
    Int_t position = (Int_t)word;

    fRCUId = -1;
    fRCUTrailerSize = 0;
    fRCUTrailerData = NULL;

    // The size is specified in a number of 40bits
    // Therefore we need to transform it to number of bytes
    position *= 5;

    if (!fIsShortDataHeader) {

      // Check the consistency of the header and trailer
      if (((fRawReader->GetDataSize() - 4) < position) ||
          ((fRawReader->GetDataSize() - 4) >= (position + 4))) {
	fRawReader->AddMajorErrorLog(kRCUTrailerSizeErr,Form("h=%d rcu=%d bytes",
							     fRawReader->GetDataSize()-4,
							     position));
	//	PrintDebug();
	IlcWarning(Form("Inconsistent raw data size ! Expected %d bytes (from the header), found %d bytes (in the RCU trailer)!",
			fRawReader->GetDataSize()-4,
			position));
	position = fRawReader->GetDataSize()-4;
      }
    }
    else {
      // Check the consistency of the header and trailer
      // In this case the header is shorter by 4 bytes
      if ((fRawReader->GetDataSize() < position) ||
          (fRawReader->GetDataSize() >= (position + 4))) {
	fRawReader->AddMajorErrorLog(kRCUTrailerSizeErr,Form("h=%d rcu=%d bytes",
							     fRawReader->GetDataSize(),
							     position));
	//	PrintDebug();
	IlcWarning(Form("Inconsistent raw data size ! Expected %d bytes (from the header), found %d bytes (in the RCU trailer)!",
			fRawReader->GetDataSize(),
			position));
	position = fRawReader->GetDataSize();
      }

      // 7 32-bit words Common Data Header
      // therefore we have to shift back by 4 bytes
      // the pointer to the raw data payload
      fData -= 4;
    }
     
    // Return the position in units of 10-bit words
    return position*8/10;
  }
}

//_____________________________________________________________________________
UInt_t IlcAltroRawStream::Get32bitWord(Int_t &index)
{
  // This method returns the 32 bit word at a given
  // position inside the raw data payload.
  // The 'index' points to the beginning of the next word.
  // The method is supposed to be endian (platform)
  // independent.
  if (!fData) {
    PrintDebug();
    IlcFatal("Raw data paylod buffer is not yet initialized !");
  }

  if (index < 4) {
    fRawReader->AddFatalErrorLog(k32bitWordReadErr,Form("pos = %d",index));
    //    PrintDebug();
    IlcWarning(Form("Invalid raw data payload position (%d) !",index));
  }

  UInt_t word = 0;
  word  = fData[--index] << 24;
  word |= fData[--index] << 16;
  word |= fData[--index] << 8;
  word |= fData[--index];

  return word;
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::ReadRCUTrailer(Int_t &index, Int_t trailerSize)
{
  // The method decodes the RCU trailer data
  // according to the RCU fw ver.2 specs
  
  fRCUTrailerSize = trailerSize*4;

  for (trailerSize -= 2; trailerSize > 0; trailerSize--) {
    Int_t word = Get32bitWord(index);
    Int_t parCode = word >> 26;
    Int_t parData = word & 0x3FFFFFF;
    switch (parCode) {
    case 1:
      // ERR_REG1
      fFECERRA = ((parData >> 13) & 0x1FFF) << 7;
      fFECERRB = ((parData & 0x1FFF)) << 7;
      break;
    case 2:
      // ERR_REG2
      fERRREG2 = parData & 0x1FF;
      break;
    case 3:
      // ERR_REG3
      fERRREG3 = parData & 0xFFF;
      break;
    case 4:
      // ERR_REG4
      fERRREG4 = parData & 0xFFF;
      break;
    case 5:
      // FEC_RO_A
      fActiveFECsA = parData & 0xFFFF;
      break;
    case 6:
      // FEC_RO_B
      fActiveFECsB = parData & 0xFFFF;
      break;
    case 7:
      // RDO_CFG1
      fAltroCFG1 = parData & 0xFFFFF;
      break;
    case 8:
      // RDO_CFG2
      fAltroCFG2 = parData & 0x1FFFFFF;
     break;
    default:
      fRawReader->AddMinorErrorLog(kRCUTrailerErr,"undef word");
      IlcWarning(Form("Undefined parameter code %d, ignore it !",
		      parCode));
      break;
    }
  }

  if (index < 4) {
    fRawReader->AddMajorErrorLog(kRCUTrailerErr,Form("tr=%d raw=%d bytes",
						     fRCUTrailerSize,
						     fRawReader->GetDataSize()));
    IlcWarning(Form("Invalid trailer size found (%d bytes) ! The size is bigger than the raw data size (%d bytes)!",
		    fRCUTrailerSize,
		    fRawReader->GetDataSize()));
  }
  fRCUTrailerData = fData + index;

  Int_t position = Get32bitWord(index);


  return position;
}

//_____________________________________________________________________________
Double_t IlcAltroRawStream::GetTSample() const
{
  // Returns the sampling time
  // in seconds. In case the rcu trailer
  // was note read, return an invalid number (0)

  if (!fRCUTrailerData) return 0.;

  const Double_t kLHCTimeSample = 25.0e-9; // LHC clocks runs at 40 MHz
  UChar_t fq = (fAltroCFG2 >> 5) & 0xF;
  Double_t tSample;
  switch (fq) {
  case 0:
    // 20 MHz
    tSample = 2.0*kLHCTimeSample;
    break;
  case 1:
    // 10 Mhz
    tSample = 4.0*kLHCTimeSample;
    break;
  case 2:
    // 5 MHz
    tSample = 8.0*kLHCTimeSample;
    break;
  default:
    IlcWarning(Form("Invalid sampling frequency value %d !",
		      fq));
    tSample = 0.;
    break;
  }

  return tSample;
}

//_____________________________________________________________________________
Double_t IlcAltroRawStream::GetL1Phase() const
{
  // Returns the L1 phase w.r.t to the
  // LHC clock
  if (!fRCUTrailerData) return 0.;

  const Double_t kLHCTimeSample = 25.0e-9; // LHC clocks runs at 40 MHz
  Double_t phase = ((Double_t)(fAltroCFG2 & 0x1F))*kLHCTimeSample;

  Double_t tSample = GetTSample();
  if (phase >= tSample) {
    IlcWarning(Form("Invalid L1 trigger phase (%f >= %f) !",
		    phase,tSample));
    phase = 0.;
  }

  return phase;
}

//_____________________________________________________________________________
void IlcAltroRawStream::PrintRCUTrailer() const
{
  // Prints the contents of
  // the RCU trailer data
  printf("RCU trailer:\n===========\n");
  printf("FECERRA: 0x%x\nFECERRB: 0x%x\n",fFECERRA,fFECERRB);
  printf("ERRREG2: 0x%x\n",fERRREG2);
  printf("#channels skipped due to address mismatch: %d\n",GetNChAddrMismatch());
  printf("#channels skipped due to bad block length: %d\n",GetNChLengthMismatch());
  printf("Active FECs (branch A): 0x%x\nActive FECs (branch B): 0x%x\n",fActiveFECsA,fActiveFECsB);
  printf("Baseline corr: 0x%x\n",GetBaselineCorr());
  printf("Number of presamples: %d\nNumber of postsamples: %d\n",GetNPresamples(),GetNPostsamples());
  printf("Second baseline corr: %d\n",GetSecondBaselineCorr());
  printf("GlitchFilter: %d\n",GetGlitchFilter());
  printf("Number of non-ZS postsamples: %d\nNumber of non-ZS presamples: %d\n",GetNNonZSPostsamples(),GetNNonZSPresamples());
  printf("Number of ALTRO buffers: %d\n",GetNAltroBuffers());
  printf("Number of pretrigger samples: %d\n",GetNPretriggerSamples());
  printf("Number of samples per channel: %d\n",GetNSamplesPerCh());
  printf("Sparse readout: %d\n",GetSparseRO());
  printf("Sampling time: %e s\n",GetTSample());
  printf("L1 Phase: %e s\n",GetL1Phase());
  printf("AltroCFG1: 0x%x\nAltroCFG2: 0x%x\n",GetAltroCFG1(),GetAltroCFG2());
  printf("===========\n");
}

//_____________________________________________________________________________
Bool_t IlcAltroRawStream::GetRCUTrailerData(UChar_t*& data) const
{
  // Return a pointer to the RCU trailer
  // data. Should be called always after
  // the RCU trailer was already processed
  // in the GetPosition() method
  if (!fRCUTrailerSize || !fRCUTrailerData) {
    IlcError("No valid RCU trailer data is found !");
    data = NULL;
    return kFALSE;
  }

  data = fRCUTrailerData;

  return kTRUE;
}

//_____________________________________________________________________________
void IlcAltroRawStream::PrintDebug() const
{
  // The method prints all the available
  // debug information.
  // Its is used in case of decoding errors.

  IlcError("Start of debug printout\n--------------------");

  Dump();
  if (fRawReader) fRawReader->Dump();

  IlcError("End of debug printout\n--------------------");
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::GetBranch() const
{
  // The method provides the RCU branch index (0 or 1)
  // for the current hardware address.
  // In case the hardware address has not been yet
  // initialized, the method returns -1
  if (fHWAddress == -1) return -1;

  return ((fHWAddress >> 11) & 0x1);
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::GetFEC() const
{
  // The method provides the front-end card index
  // for the current hardware address.
  // In case the hardware address has not been yet
  // initialized, the method returns -1
  if (fHWAddress == -1) return -1;

  return ((fHWAddress >> 7) & 0xF);
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::GetAltro() const
{
  // The method provides the altro chip index
  // for the current hardware address.
  // In case the hardware address has not been yet
  // initialized, the method returns -1
  if (fHWAddress == -1) return -1;

  return ((fHWAddress >> 4) & 0x7);
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::GetChannel() const
{
  // The method provides the channel index
  // for the current hardware address.
  // In case the hardware address has not been yet
  // initialized, the method returns -1
  if (fHWAddress == -1) return -1;

  return (fHWAddress & 0xF);
}

//_____________________________________________________________________________
void IlcAltroRawStream::AddMappingErrorLog(const char *message)
{
  // Signal a mapping error
  // The method can be used by the TPC,PHOS,EMCAL,FMD raw stream
  // classes in order to log an error related to bad altro mapping

  if (fRawReader) fRawReader->AddMinorErrorLog(kBadAltroMapping,message);
}

//_____________________________________________________________________________
Int_t IlcAltroRawStream::GetRCUPayloadSizeInSOD() const
{
  // Get the size of the RCU data in case
  // of SOD events
  if (fRawReader) {
    if (fRawReader->GetType() == IlcRawEventHeaderBase::kStartOfData) {
      return fPosition;
    }
  }
  return -1;
}
