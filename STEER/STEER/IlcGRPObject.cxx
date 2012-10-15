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

/* $Id$ */

// **********************
// Class containing the GRP data that have to be stored in the OCDB.
// Data come either from DAQ logbook or from DCS DB.
// Processing of the data can also be performed here.
// **********************

#include <TMap.h>
#include <TObject.h>
#include <TObjString.h>
#include <TGeoGlobalMagField.h>

#include "IlcGRPObject.h"
#include "IlcDCSSensor.h"
#include "IlcLog.h"

ClassImp(IlcGRPObject)

const Double_t kCavernCut = 1.0;           // tolerable difference between cavern pressure sensors
const Double_t kSurfaceDifference = 4.5;   //  offset between surface and cavern pressure sensors
	
const Float_t IlcGRPObject::fgkInvalidFloat = 1E-33; // value to identify invalid data - float
const TString IlcGRPObject::fgkInvalidString = "";  // value to identify invalid data - string
const Char_t IlcGRPObject::fgkInvalidChar = -1;         // value to identify invalid data - uchar
const Int_t IlcGRPObject::fgkInvalidInt = -1;  // value to identify invalid data - uint
const Int_t IlcGRPObject::fgkInvalidUInt = 0;  // value to identify invalid data - uint
const Int_t IlcGRPObject::fgknDCSDPHallProbes = 40;   // number of dcs dps
const char* IlcGRPObject::fgkDCSDataPointsHallProbes[IlcGRPObject::fgknDCSDPHallProbes] = {
		   "L3_BSF17_H1",
		   "L3_BSF17_H2",
		   "L3_BSF17_H3",
		   "L3_BSF17_Temperature",
		   "L3_BSF4_H1",
		   "L3_BSF4_H2",
		   "L3_BSF4_H3",
		   "L3_BSF4_Temperature",
		   "L3_BKF17_H1",
		   "L3_BKF17_H2",
		   "L3_BKF17_H3",
		   "L3_BKF17_Temperature",
		   "L3_BKF4_H1",
		   "L3_BKF4_H2",
		   "L3_BKF4_H3",
		   "L3_BKF4_Temperature",
		   "L3_BSF13_H1",
		   "L3_BSF13_H2",
		   "L3_BSF13_H3",
		   "L3_BSF13_Temperature",
		   "L3_BSF8_H1",
		   "L3_BSF8_H2",
		   "L3_BSF8_H3",
		   "L3_BSF8_Temperature",
		   "L3_BKF13_H1",
		   "L3_BKF13_H2",
		   "L3_BKF13_H3",
		   "L3_BKF13_Temperature",
		   "L3_BKF8_H1",
		   "L3_BKF8_H2",
		   "L3_BKF8_H3",
		   "L3_BKF8_Temperature",
		   "Dipole_Inside_H1",
		   "Dipole_Inside_H2",
		   "Dipole_Inside_H3",
		   "Dipole_Inside_Temperature",
		   "Dipole_Outside_H1",
		   "Dipole_Outside_H2",
		   "Dipole_Outside_H3",
		   "Dipole_Outside_Temperature",
                 };

//-----------------------------------------------------------------------------
IlcGRPObject::IlcGRPObject():
	TObject(),
	fPoints(5),
	fDimension(0),
	fTimeStart((time_t)fgkInvalidFloat),
	fTimeEnd((time_t)fgkInvalidFloat),
	fBeamEnergy(fgkInvalidFloat),
	fBeamType(fgkInvalidString),
	fNumberOfDetectors(fgkInvalidChar),
	fDetectorMask(fgkInvalidUInt),
	fLHCPeriod(fgkInvalidString),
	fRunType(fgkInvalidString),
	fLHCState(fgkInvalidString),
	fL3Polarity(fgkInvalidChar),
	fDipolePolarity(fgkInvalidChar),
	fL3Current(new Float_t[fPoints]),
	fDipoleCurrent(new Float_t[fPoints]),
	fCavernTemperature(new Float_t[fPoints]),
	fCavernAtmosPressure(0x0),
	fCavernAtmosPressure2(0x0),
	fSurfaceAtmosPressure(0x0),
	fHallProbes(0x0),
	fMachineMode(fgkInvalidString),
	fLHCStateArray(0x0),
	fMachineModeArray(0x0),
	fQATrigClasses(0x0),
	fQACloningRequest(0x0),
	fMaxTimeLHCValidity(0),
	fNFalseDataQualityFlag(0),
	fFalseDataQualityFlag(0x0),
	fBeamTypeFromLHC(fgkInvalidString),
	fMagField(0x0)
{

	//
	// IlcGRPObject default ctor
	//

	fDimension = fgknDCSDPHallProbes*fPoints;
	fHallProbes = new Float_t[fDimension];

	for (Int_t nhp=0; nhp< fDimension; nhp++){ // setting to zero values for non working HP 
		if ((nhp >= 0 && nhp <= 1*fPoints-1) || // L3_BSF17_H1
		    (nhp >= 1*fPoints && nhp <= 2*fPoints-1) || // L3_BSF17_H2
		    (nhp >= 2*fPoints && nhp <= 3*fPoints-1) || // L3_BSF17_H3
                    (nhp >= 3*fPoints && nhp <= 4*fPoints-1) || // L3_BSF17_Temperature
                    (nhp >= 6*fPoints && nhp <= 7*fPoints-1) ) { // L3_BSF4_H3
			fHallProbes[nhp] = 0; // setting to zero values for non working HP 
			IlcDebug(2,Form("setting hp[%d] to zero = %f", 
nhp, fHallProbes[nhp]));
		}
		else {
			fHallProbes[nhp] = fgkInvalidFloat;
		}
	}


	for (Int_t i = 0; i < fPoints; i++){

		fL3Current[i] = fgkInvalidFloat;
		fDipoleCurrent[i] = fgkInvalidFloat;
		fCavernTemperature[i] = fgkInvalidFloat;
	}

	for (Int_t ibeamType = 0; ibeamType<2; ibeamType++){
		fSeparateBeamType[ibeamType] = fgkInvalidString;
	}
	
	
	fMagField = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
}

//-----------------------------------------------------------------------------

IlcGRPObject::IlcGRPObject(const IlcGRPObject &obj):
	TObject(obj),
	fPoints(obj.fPoints),
	fDimension(obj.fDimension),
	fTimeStart(obj.fTimeStart),
	fTimeEnd(obj.fTimeEnd),
	fBeamEnergy(obj.fBeamEnergy),
	fBeamType(obj.fBeamType),
	fNumberOfDetectors(obj.fNumberOfDetectors),
	fDetectorMask(obj.fDetectorMask),
	fLHCPeriod(obj.fLHCPeriod),
	fRunType(obj.fRunType),
	fLHCState(obj.fLHCState),
	fL3Polarity(obj.fL3Polarity),
	fDipolePolarity(obj.fDipolePolarity),
	fL3Current(new Float_t[fPoints]),
	fDipoleCurrent(new Float_t[fPoints]),
	fCavernTemperature(new Float_t[fPoints]),
	fCavernAtmosPressure(obj.fCavernAtmosPressure),
	fCavernAtmosPressure2(obj.fCavernAtmosPressure2),
	fSurfaceAtmosPressure(obj.fSurfaceAtmosPressure),
	fHallProbes(0x0),
	fMachineMode(obj.fMachineMode),
	fLHCStateArray(obj.fLHCStateArray),
	fMachineModeArray(obj.fMachineModeArray),
	fQATrigClasses(obj.fQATrigClasses),
	fQACloningRequest(obj.fQACloningRequest),
	fMaxTimeLHCValidity(obj.fMaxTimeLHCValidity),
	fNFalseDataQualityFlag(obj.fNFalseDataQualityFlag),
	fFalseDataQualityFlag(obj.fFalseDataQualityFlag),
	fBeamTypeFromLHC(obj.fBeamTypeFromLHC),
	fMagField(obj.fMagField)

{

	//
	// IlcGRPObject copy ctor
	//

	fHallProbes = new Float_t[fDimension]; 

	for (Int_t nhp=0; nhp< fDimension; nhp++){
		fHallProbes[nhp] = obj.fHallProbes[nhp];
	}

	for (Int_t i = 0; i < fPoints; i++){

		fL3Current[i] = obj.fL3Current[i];
		fDipoleCurrent[i] = obj.fDipoleCurrent[i];
		fCavernTemperature[i] = obj.fCavernTemperature[i];
	}

	for (Int_t ibeamType = 0; ibeamType<2; ibeamType++){
		fSeparateBeamType[ibeamType] = obj.fSeparateBeamType[ibeamType];
	}
}

//-----------------------------------------------------------------------------

IlcGRPObject& IlcGRPObject:: operator=(const IlcGRPObject & obj) 
{

	//
	// IlcGRPObject assignment operator
	//

	if (&obj == this) return *this; 

	TObject::operator=(obj);
	this->fTimeStart = obj.GetTimeStart();
	this->fTimeEnd = obj.GetTimeEnd();
	this->fBeamEnergy = obj.GetBeamEnergy();
	this->fBeamType = obj.GetBeamType();
	this->fNumberOfDetectors = obj.GetNumberOfDetectors();
	this->fDetectorMask = obj.GetDetectorMask();
	this->fLHCPeriod = obj.GetLHCPeriod();
	this->fRunType = obj.GetRunType();
	this->fLHCState = obj.GetLHCState();
	this->fL3Polarity = obj.GetL3Polarity();
	this->fDipolePolarity = obj.GetDipolePolarity();
	this->fCavernAtmosPressure = obj.GetCavernAtmosPressure();
	this->fCavernAtmosPressure2 = obj.GetCavernAtmosPressure2();
	this->fSurfaceAtmosPressure = obj.GetSurfaceAtmosPressure();
	this->fPoints = obj.GetPoints();
	this->fDimension = obj.GetDimension();

	this->fL3Current = new Float_t[fPoints];
	this->fDipoleCurrent = new Float_t[fPoints];
	this->fCavernTemperature = new Float_t[fPoints];
	
	if (this->fHallProbes==NULL) this->fHallProbes = new Float_t[this->fDimension]; 
	for (Int_t nhp=0; nhp< fDimension; nhp++){
		this->fHallProbes[nhp] = obj.GetHallProbes(nhp);

	}
	for (Int_t i = 0; i < fPoints; i++){

		this->fL3Current[i] = obj.GetL3Current((Stats)i);
		this->fDipoleCurrent[i] = obj.GetDipoleCurrent((Stats)i);
		this->fCavernTemperature[i] = obj.GetCavernTemperature((Stats)i);
	}

	this->fMachineMode = obj.fMachineMode;
	this->fLHCStateArray = obj.fLHCStateArray;
	this->fMachineModeArray = obj.fMachineModeArray;
	this->fMaxTimeLHCValidity = obj.fMaxTimeLHCValidity;

	this->fQATrigClasses = obj.fQATrigClasses;
	this->fQACloningRequest = obj.fQACloningRequest;

	for (Int_t ibeamType = 0; ibeamType<2; ibeamType++){
		this->fSeparateBeamType[ibeamType] = obj.fSeparateBeamType[ibeamType];
	}

	this->fNFalseDataQualityFlag = obj.fNFalseDataQualityFlag;
	this->fFalseDataQualityFlag = obj.fFalseDataQualityFlag;
	this->fBeamTypeFromLHC = obj.fBeamTypeFromLHC;

	this->fMagField = obj.fMagField;

	return *this;
}

//-----------------------------------------------------------------------------

IlcGRPObject::~IlcGRPObject() {

	//
	// dtor
	//

	
	delete [] fHallProbes;
	delete [] fL3Current;
	delete [] fDipoleCurrent;
	delete [] fCavernTemperature;

	if (fCavernAtmosPressure){
		delete fCavernAtmosPressure;
		fCavernAtmosPressure = 0x0;
	}
	if (fCavernAtmosPressure2){
		delete fCavernAtmosPressure2;
		fCavernAtmosPressure2 = 0x0;
	}
	if (fSurfaceAtmosPressure){
		delete fSurfaceAtmosPressure;
		fSurfaceAtmosPressure = 0x0;
	}
	if (fLHCStateArray){
		delete fLHCStateArray;
		fLHCStateArray = 0x0;
	}
	if (fMachineModeArray){
		delete fMachineModeArray;
		fMachineModeArray = 0x0;
	}
	if (fQATrigClasses) {
	  delete fQATrigClasses;
	  fQATrigClasses = 0x0;
	}
	if (fQACloningRequest) {
	  delete fQACloningRequest;
	  fQACloningRequest = 0x0;
	}
	if (fFalseDataQualityFlag){
		delete fFalseDataQualityFlag;
		fFalseDataQualityFlag = 0x0;
	}
	if(fMagField){
	  delete fMagField;
	  fMagField = 0x0;
	}
}

//-----------------------------------------------------------------------------
Float_t* IlcGRPObject::GetHallProbesArray(DP_HallProbes hp) const {

	//
	// method to return array of statistical
        // variables for Hall Probe hp
	//

	Float_t* array = new Float_t[fPoints];
	Int_t shift = fPoints*(Int_t)hp; 
	for (Int_t i=0;i<fPoints; i++){

		array[i] = fHallProbes[shift+i];

	}

	return array;
}
//-------------------------------------------------------------------------------

void IlcGRPObject::SetHallProbes(DP_HallProbes hp, const Float_t* hall_probe){

	//
	// method to set hall probe hp 
	// from a given array
	//

	Int_t shift = fPoints*hp;
	for (Int_t i = 0; i< fPoints; i++){

		fHallProbes[i+shift] =  hall_probe[i];
	}
	return;
}

//-------------------------------------------------------------------------------

void IlcGRPObject::ReadValuesFromMap(const TMap* mapGRP){

	//
	// method to set the values of the GRP parameters 
	// reading them from the old format of the GRP 
	// object, i.e. a TMap
	//

	if (mapGRP->GetValue("fIlcStartTime")){
		SetTimeStart((time_t)(((TObjString*)(mapGRP->GetValue("fIlcStartTime")))->GetString()).Atoi());
	}
	else {
		IlcError(Form("No fIlcStartTime value found in GRP map!"));
	}
	if (mapGRP->GetValue("fIlcStopTime")){
		SetTimeEnd((time_t)(((TObjString*)(mapGRP->GetValue("fIlcStopTime")))->GetString()).Atoi());
	}
	
	else { 
		IlcError(Form("No fIlcStopTime value found in GRP map!"));
	}

	if(mapGRP->GetValue("fIlcBeamEnergy")){
	  double be = (((TObjString*)(mapGRP->GetValue("fIlcBeamEnergy")))->GetString()).Atof();
	  if (IsBeamEnergyIsSqrtSHalfGeV()) be/=2;   // old format was storig sqrt(s)
	  SetBeamEnergy(be);
	}
	else { 
		IlcError(Form("No fIlcBeamEnergy value found in GRP map!"));
	}
	if(mapGRP->GetValue("fIlcBeamType")){
		SetBeamType(((TObjString*)(mapGRP->GetValue("fIlcBeamType")))->GetString());
	}
	else { 
		IlcError(Form("No fIlcBeamType value found in GRP map!"));
	}
	if(mapGRP->GetValue("fNumberOfDetectors")){
		SetNumberOfDetectors((Char_t)(((TObjString*)(mapGRP->GetValue("fNumberOfDetectors")))->GetString()).Atoi()); 
		IlcDebug(1, Form("fNumberOfDetectors = %d", fNumberOfDetectors));
	}
	else { 
		IlcError(Form("No fNumberOfDetectors value found in GRP map!"));
	}
	if(mapGRP->GetValue("fDetectorMask")){
		SetDetectorMask((UInt_t)(((TObjString*)(mapGRP->GetValue("fDetectorMask")))->GetString()).Atoi());  
		IlcDebug(1, Form("fDetectorMask = %d",fDetectorMask));
	}
	else { 
		IlcError(Form("No fDetectorMask value found in GRP map!"));
	}
	if(mapGRP->GetValue("fLHCPeriod")){
		SetLHCPeriod(((TObjString*)(mapGRP->GetValue("fLHCPeriod")))->GetString());
	}
	else { 
		IlcError(Form("No fLHCPeriod value found in GRP map!"));
	}
	if(mapGRP->GetValue("fRunType")){
		SetRunType(((TObjString*)(mapGRP->GetValue("fRunType")))->GetString());
	}
	else { 
		IlcError(Form("No fRunType value found in GRP map!"));
	}
	if(mapGRP->GetValue("fLHCState")){
		SetLHCState(((TObjString*)(mapGRP->GetValue("fLHCState")))->GetString());
	}
	else { 
		IlcError(Form("No fLHCState value found in GRP map!"));
	}
	if(mapGRP->GetValue("fLHCluminosity")){
		IlcInfo(Form("fLHCLuminosity found, but not there anymore in the new object"));
	}	
	else { 
		IlcError(Form("No fLHCLuminosity value found in GRP map!"));
	}
	if(mapGRP->GetValue("fBeamIntensity")){
		IlcInfo(Form("fBeamIntensity found, but not there anymore in the new object"));
	}	
	else { 
		IlcError(Form("No fBeamIntensity value found in GRP map!"));
	}
	if(mapGRP->GetValue("fL3Polarity")){
		SetL3Polarity((Char_t)(((TObjString*)(mapGRP->GetValue("fL3Polarity")))->GetString()).Atoi());
	}	
	else { 
		IlcError(Form("No fL3Polarity value found in GRP map!"));
	}
	if(mapGRP->GetValue("fDipolePolarity")){
		SetDipolePolarity((Char_t)(((TObjString*)(mapGRP->GetValue("fDipolePolarity")))->GetString()).Atoi());  
	}	
	else { 
		IlcError(Form("No fDipolePolarity value found in GRP map!"));
	}
	if(mapGRP->GetValue("fL3Current")){
		IlcInfo(Form("fL3Current found, but porting only average to the new object, since the other values are not available in the old object"));
		SetL3Current((Float_t)(((TObjString*)(mapGRP->GetValue("fL3Current")))->GetString()).Atof(),(Stats)0);
	}	
	else { 
		IlcError(Form("No fL3Current value found in GRP map!"));
	}
	if(mapGRP->GetValue("fDipoleCurrent")){
		IlcInfo(Form("fDipoleCurrent found, but porting only average to the new object, since the other values are not available in the old object"));
		SetDipoleCurrent((Float_t)(((TObjString*)(mapGRP->GetValue("fDipoleCurrent")))->GetString()).Atof(),(Stats)0);
	}	
	else { 
		IlcError(Form("No fDipoleCurrent value found in GRP map!"));
	}
	if(mapGRP->GetValue("fCavernTemperature")){
		IlcInfo(Form("fCaverntemperature found, but porting only average to the new object, since the other values are not available in the old object"));
		SetCavernTemperature((Float_t)(((TObjString*)(mapGRP->GetValue("fCavernTemperature")))->GetString()).Atof(),(Stats)0);
	}	
	else { 
		IlcError(Form("No fCavernTemperature value found in GRP map!"));
	}
	if(mapGRP->GetValue("fCavernAtmosPressure")){
		IlcInfo(Form("fCavernAtmosPressure found, but not ported to the new object since of a different type"));
	}	
	else { 
		IlcError(Form("No fCavernAtmosPressure value found in GRP map!"));
	}
	if(mapGRP->GetValue("fP2Pressure")){
		SetSurfaceAtmosPressure((IlcDCSSensor*)((TObjString*)(mapGRP->GetValue("fP2Pressure"))));
	}
	else { 
		IlcError(Form("No fP2Pressure value found in GRP map!"));
	}
	
	return;

}
//-------------------------------------------------------------------------------

Float_t IlcGRPObject::GetBeamEnergy() const {

	//
	// Getting the energy
	// in case of BeamType = A-A, multiplying by 82/208 (for PbPb)
	//

	Float_t energy = fBeamEnergy;
	if (fBeamType=="A-A"){
		energy = energy*82./208;
	}
	return IsBeamEnergyIsSqrtSHalfGeV() ? energy : energy/2;
}

//-------------------------------------------------------------------------------

Double_t IlcGRPObject::EvalCavernPressure(const TTimeStamp& time, Bool_t& inside) const { 
//
//  Return current pressure value from 'best' cavern sensor
//    (default sensor 2. If sensor 1 and sensor 2 are sufficiently different,
//     use sensor most compatible with surface sensor

  return EvalCavernPressure(fCavernAtmosPressure,fCavernAtmosPressure2,
             fSurfaceAtmosPressure,time,inside);
	     			
}

//-------------------------------------------------------------------------------

Double_t IlcGRPObject::EvalCavernPressure(IlcDCSSensor* cavern1, 
	              IlcDCSSensor* cavern2, IlcDCSSensor* surface, 
		      const TTimeStamp& time, Bool_t& inside)  {
//
//  Return current pressure value from 'best' cavern sensor
//    (default sensor 2. If sensor 1 and sensor 2 are sufficiently different,
//     use sensor most compatible with surface sensor

     
    Double_t valueSensor2 = cavern2->Eval(time,inside);
    Double_t valueSensor1 = cavern1->Eval(time,inside);
    if (TMath::Abs(valueSensor2-valueSensor1)<kCavernCut) {
        return valueSensor2;
    } else { 
        Double_t valueSurface = surface->Eval(time,inside);
        Double_t diff1 = TMath::Abs(valueSensor1-valueSurface-kSurfaceDifference);
        Double_t diff2 = TMath::Abs(valueSensor2-valueSurface-kSurfaceDifference);
	if (TMath::Abs(diff1)<TMath::Abs(diff2) ) {
	    return valueSensor1;
	} else {
	    return valueSensor2;
	}
     }
}

//-------------------------------------------------------------------------------

IlcDCSSensor* IlcGRPObject::GetBestCavernAtmosPressure(IlcDCSSensor* cavern1, 
	     IlcDCSSensor* cavern2, IlcDCSSensor* surface, const TTimeStamp& time) {

//
//  Return pointer to 'best' cavern sensor
//    (default sensor 2. If sensor 1 and sensor 2 are sufficiently different,
//     use sensor most compatible with surface sensor
//    Pressure measuread at time specified by TTimestamp


    Bool_t inside; 
    Double_t valueSensor2 = cavern2->Eval(time,inside);
    Double_t valueSensor1 = cavern1->Eval(time,inside);
    if (TMath::Abs(valueSensor2-valueSensor1)<kCavernCut) {
        return cavern2;
    } else { 
        Double_t valueSurface = surface->Eval(time,inside);
        Double_t diff1 = TMath::Abs(valueSensor1-valueSurface-kSurfaceDifference);
        Double_t diff2 = TMath::Abs(valueSensor2-valueSurface-kSurfaceDifference);
	if (TMath::Abs(diff1)<TMath::Abs(diff2) ) {
	    return cavern1;
	} else {
	    return cavern2;
	}
     }
}

//-------------------------------------------------------------------------------

IlcDCSSensor*   IlcGRPObject::GetBestCavernAtmosPressure(const TTimeStamp& time) const {
//
//  Return pointer to 'best' cavern sensor
//    (default sensor 2. If sensor 1 and sensor 2 are sufficiently different,
//     use sensor most compatible with surface sensor
//    Pressure measuread at time specified by TTimestamp


   return GetBestCavernAtmosPressure(fCavernAtmosPressure,
        fCavernAtmosPressure2, fSurfaceAtmosPressure, time);

}
    
//-------------------------------------------------------------------------------

IlcDCSSensor*   IlcGRPObject::GetBestCavernAtmosPressure() const {
//
//  Return pointer to 'best' cavern sensor
//    (default sensor 2. If sensor 1 and sensor 2 are sufficiently different,
//     use sensor most compatible with surface sensor
//    Pressure measuread at start-of-run

   return GetBestCavernAtmosPressure(TTimeStamp(fTimeStart));
}

//-------------------------------------------------------------------------------
void IlcGRPObject::SetFalseDataQualityFlagPeriods(Double_t* falses){
	
	//
	// setting the starts (even positions in the array) and ends (odd positions in the array)
	// of the periods when the Data Quality Flag was set to FALSE
	//
	
	fFalseDataQualityFlag = new TArrayD(fNFalseDataQualityFlag*2,falses);
}

//-------------------------------------------------------------------------------
Double_t IlcGRPObject::GetStartFalseDataQualityFlag(Int_t iperiod) const {
	
	// 
	// returning the start timestamp of the FALSE period "iperiod"
	//
	
	if (iperiod < fNFalseDataQualityFlag){
		return fFalseDataQualityFlag->At(iperiod*2);
	}
	else{
		IlcError(Form("You are looking for FALSE period %d, but the number of FALSE periods was %d - returning -1!", iperiod, fNFalseDataQualityFlag));
	}
	return -1.;
}

//-------------------------------------------------------------------------------
Double_t IlcGRPObject::GetEndFalseDataQualityFlag(Int_t iperiod) const {
	
	// 
	// returning the end timestamp of the FALSE period "iperiod"
	//
	
	if (iperiod < fNFalseDataQualityFlag){
		return fFalseDataQualityFlag->At(iperiod*2+1);
	}
	else{
		IlcError(Form("You are looking for FALSE period %d, but the number of FALSE periods was %d - returning -1!", iperiod, fNFalseDataQualityFlag));
	}
	return -1.;
}
