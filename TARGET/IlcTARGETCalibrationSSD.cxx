/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                         *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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


#include "IlcTARGETCalibrationSSD.h"
//////////////////////////////////////////////////////
//  Calibration class for set:TARGET                   //
//  Specific subdetector implementation             //
//  for silicon strips detectors                    //
//                                                  //
//                                                  //
//////////////////////////////////////////////////////

const Double_t IlcTARGETCalibrationSSD::fgkNoiseNDefault = 625.;
const Double_t IlcTARGETCalibrationSSD::fgkNoisePDefault = 420.;
const Int_t IlcTARGETCalibrationSSD::fgkNParDefault = 6;
const Double_t IlcTARGETCalibrationSSD::fgkSigmaPDefault = 3.;
const Double_t IlcTARGETCalibrationSSD::fgkSigmaNDefault = 2.;

ClassImp(IlcTARGETCalibrationSSD)

//______________________________________________________________________
IlcTARGETCalibrationSSD::IlcTARGETCalibrationSSD(){
    // Default Constructor

    fDetPar = 0;
    fNPar   = 0;
       fNoiseP = 0;
    fNoiseN = 0;
    fSigmaP = 0;
    fSigmaN = 0;
    SetNoiseParam(fgkNoisePDefault,fgkNoiseNDefault);
}
//______________________________________________________________________
IlcTARGETCalibrationSSD::IlcTARGETCalibrationSSD(const char *dataType){
    // constructor

    SetNoiseParam(fgkNoisePDefault,fgkNoiseNDefault);
    SetDataType(dataType);
    SetSigmaSpread(fgkSigmaPDefault,fgkSigmaNDefault);
    SetNDetParam(fgkNParDefault);   // Sets fNPar=6 by default.
    fDetPar = new Double_t[fNPar];
    if (fNPar==6) {
	fDetPar[0]=10.;
	fDetPar[1]=5.;
	fDetPar[2]=0.02;
	fDetPar[3]=0.02;
	fDetPar[4]=0.02;
	fDetPar[5]=0.03;
    } // end if
}
//______________________________________________________________________
IlcTARGETCalibrationSSD::~IlcTARGETCalibrationSSD(){
    // destructor

    delete [] fDetPar;
}
//______________________________________________________________________
IlcTARGETCalibrationSSD& IlcTARGETCalibrationSSD::operator=(const IlcTARGETCalibrationSSD &src) {
    // = operator.

    if(&src == this) return *this;

    this->fNPar      = src.fNPar;
    for(Int_t i=0;i<this->fNPar;i++) this->fDetPar[i] = src.fDetPar[i];
    this->fNoiseP    = src.fNoiseP;
    this->fNoiseN    = src.fNoiseN;
    this->fSigmaP    = src.fSigmaP;
    this->fSigmaN    = src.fSigmaN;
    this->fDataType  = src.fDataType;

    return *this;
}
//_________________________________________________________________________
IlcTARGETCalibrationSSD::IlcTARGETCalibrationSSD(const IlcTARGETCalibrationSSD &src) :
    IlcTARGETCalibration(src) {
    // copy constructor

    *this = src;
}
//______________________________________________________________________
void IlcTARGETCalibrationSSD::SetDetParam(Double_t  *par){
    // set det param
    Int_t i;

    for (i=0; i<fNPar; i++) {
	fDetPar[i]=par[i];
	//printf("\n CompressPar %d %d \n",i,fCPar[i]);    
    } // end for i
}
//______________________________________________________________________
void IlcTARGETCalibrationSSD::GetDetParam(Double_t  *par) const {
    // get det param
    Int_t i;

    for (i=0; i<fNPar; i++) {
	par[i]=fDetPar[i];
    } // end for i
}
