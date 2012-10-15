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


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Calibration class for DCS sensors                                        //
//  Authors: Marian Ivanov and Haavard Helstrup                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCSSensorArray.h"
#include "IlcLog.h"
#include <TMath.h>

ClassImp(IlcDCSSensorArray)

const Double_t kSecInHour = 3600.; // seconds in one hour
const UInt_t   kMinMapTime = 60;   // don't fit maps shorter than one minute

//_____________________________________________________________________________
IlcDCSSensorArray::IlcDCSSensorArray():TNamed(), 
  fMinGraph(10),
  fMinPoints(10),
  fIter(10),
  fMaxDelta(0.0),
  fFitReq(2),
  fValCut(-1),
  fDiffCut(-1),
  fStartTime (2000,1,1,0,0,0),
  fEndTime   (2000,1,1,0,0,0),
  fSensors(0)
{
  //
  // IlcDCSSensorArray default constructor
  //

}
//_____________________________________________________________________________
IlcDCSSensorArray::IlcDCSSensorArray(TClonesArray *arr):TNamed(),
  fMinGraph(10),
  fMinPoints(10),
  fIter(10),
  fMaxDelta(0.0),
  fFitReq(2),
  fValCut(-1),
  fDiffCut(-1),
  fStartTime (2000,1,1,0,0,0),
  fEndTime   (2000,1,1,0,0,0),
  fSensors(arr)
{
  //
  // IlcDCSSensorArray special constructor taking TClonesArray from ReadList
  //

}
//_____________________________________________________________________________
IlcDCSSensorArray::IlcDCSSensorArray(Int_t run, const char* dbEntry) :
  TNamed(),
  fMinGraph(10),
  fMinPoints(10),
  fIter(10),
  fMaxDelta(0.0),
  fFitReq(2),
  fValCut(-1),
  fDiffCut(-1),
  fStartTime (2000,1,1,0,0,0),
  fEndTime   (2000,1,1,0,0,0),
  fSensors(0)
{
  //
  // Read configuration from OCDB
  //

  IlcCDBEntry *entry = IlcCDBManager::Instance()->Get(dbEntry,run);
  if (entry) {
    TTree *tree = (TTree*) entry->GetObject();
    fSensors = IlcDCSSensor::ReadTree(tree);
  } else {
    IlcError("Unable to load configuration from CDB!");
  }
}
//_____________________________________________________________________________
IlcDCSSensorArray::IlcDCSSensorArray(UInt_t startTime, UInt_t endTime,
                       TTree* confTree) :
  TNamed(),
  fMinGraph(10),
  fMinPoints(10),
  fIter(10),
  fMaxDelta(0.0),
  fFitReq(2),
  fValCut(-1),
  fDiffCut(-1),
  fStartTime (2000,1,1,0,0,0),
  fEndTime   (2000,1,1,0,0,0),
  fSensors(0)

{
  //
  // IlcDCSSensorArray constructor for Shuttle preprocessor
  //  (confTree read from OCDB)
  //
  fSensors = IlcDCSSensor::ReadTree(confTree);
  fSensors->BypassStreamer(kFALSE);
  fStartTime = TTimeStamp((time_t)startTime,0);
  fEndTime   = TTimeStamp((time_t)endTime,0);
}


//_____________________________________________________________________________
IlcDCSSensorArray::IlcDCSSensorArray(UInt_t startTime, UInt_t endTime,
                       TClonesArray *sensors) :
  TNamed(),
  fMinGraph(10),
  fMinPoints(10),
  fIter(10),
  fMaxDelta(0.0),
  fFitReq(2),
  fValCut(-1),
  fDiffCut(-1),
  fStartTime (2000,1,1,0,0,0),
  fEndTime   (2000,1,1,0,0,0),
  fSensors(sensors)

{
  //
  // IlcDCSSensorArray constructor for Shuttle preprocessor
  //  (TClonesArray of IlcDCSSensor objects)
  //
  fStartTime = TTimeStamp((time_t)startTime,0);
  fEndTime   = TTimeStamp((time_t)endTime,0);
}

//_____________________________________________________________________________
IlcDCSSensorArray::IlcDCSSensorArray(const IlcDCSSensorArray &c):TNamed(c),
  fMinGraph(c.fMinGraph),
  fMinPoints(c.fMinPoints),
  fIter(c.fIter),
  fMaxDelta(c.fMaxDelta),
  fFitReq(c.fFitReq),
  fValCut(c.fValCut),
  fDiffCut(c.fDiffCut),
  fStartTime (c.fStartTime),
  fEndTime   (c.fEndTime),
  fSensors(0)

{
  //
  // IlcDCSSensorArray copy constructor
  //

  fSensors = (TClonesArray*)c.fSensors->Clone();
}

///_____________________________________________________________________________
IlcDCSSensorArray::~IlcDCSSensorArray()
{
  //
  // IlcDCSSensorArray destructor
  //
  fSensors->Delete();
  delete fSensors;

}

//_____________________________________________________________________________
IlcDCSSensorArray &IlcDCSSensorArray::operator=(const IlcDCSSensorArray &c)
{
  //
  // Assignment operator
  //
  if (this != &c) {
     fSensors->Delete();
     new (this) IlcDCSSensorArray(c);
     fSensors = (TClonesArray*)c.fSensors->Clone();
  }
  return *this;
}

//____________________________________________________________________________

void IlcDCSSensorArray::SetGraph(TMap *map)
{
  //
  // Read graphs from DCS maps
  //
  Int_t nsensors = fSensors->GetEntries();
  for ( Int_t isensor=0; isensor<nsensors; isensor++) {
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
    TString stringID = entry->GetStringID();
    TGraph *gr = (TGraph*)map->GetValue(stringID.Data());
    if ( gr !=0 ) {
       entry->SetGraph((TGraph*)gr->Clone());
    } else {
       entry->SetGraph(0);
    }
  }
}
//_____________________________________________________________________________
void IlcDCSSensorArray::MakeSplineFit(TMap *map, Bool_t keepMap)
{
  //
  // Make spline fits from DCS maps
  //
  Int_t nsensors = fSensors->GetEntries();
  for ( Int_t isensor=0; isensor<nsensors; isensor++) {
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
    TString stringID = entry->GetStringID();
    TGraph *gr = (TGraph*)map->GetValue(stringID.Data());
    if (!gr ) {
      entry->SetFit(0);
      entry->SetGraph(0);
      IlcWarning(Form("sensor %s: no input graph",stringID.Data()));
      continue;
    }
    UInt_t timeDiff = entry->GetEndTime() - entry->GetStartTime();
    if ( timeDiff < kMinMapTime ) {
      IlcWarning(Form("sensor %s: map length < 60 s, DCS graph kept.",stringID.Data()));
      entry->SetGraph((TGraph*)gr->Clone());
    } else {
      IlcSplineFit *fit = new IlcSplineFit();
      fit->SetMinPoints(fMinGraph);
      fit->InitKnots(gr,fMinPoints,fIter,fMaxDelta);
      fit->SplineFit(fFitReq);
      fit->Cleanup();
      if (fit->GetKnots()>0) {
        entry->SetFit(fit);
      } else {
        IlcWarning(Form("sensor %s: no fit performed, DCS graph kept.",stringID.Data()));
        entry->SetGraph((TGraph*)gr->Clone());
      }
    }
    if (keepMap) entry->SetGraph((TGraph*)gr->Clone());
  }
}
//_____________________________________________________________________________
void IlcDCSSensorArray::MakeSplineFitAddPoints(TMap *map)
{
  //
  // Make spline fits from DCS maps
  //
  Int_t nsensors = fSensors->GetEntries();
  for ( Int_t isensor=0; isensor<nsensors; isensor++) {
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);

  // fetch old points from existing graph

    TGraph *gr = entry->GetGraph();
    if (!gr) {
      gr = new TGraph();
      entry->SetGraph(gr);
    } 
    TString stringID = entry->GetStringID();

  // fetch new points from DCS map
  
    TGraph *grAdd = (TGraph*)map->GetValue(stringID.Data());
    if (!grAdd ) return;

  // add new points to end of graph
  
    Int_t nPointsOld=gr->GetN();
    Int_t nPointsAdd=grAdd->GetN();
    gr->Expand(nPointsOld+nPointsAdd);
    gr->Set(nPointsOld+nPointsAdd);
    Double_t *addX=grAdd->GetX();
    Double_t *addY=grAdd->GetY();
    for (Int_t i=0;i<nPointsAdd;i++) {
      gr->SetPoint(nPointsOld+i,addX[i],addY[i]);
    }
 
   // make fit to complete graph
   
    IlcSplineFit *fit = new IlcSplineFit();
    fit->SetMinPoints(fMinGraph);
    fit->InitKnots(gr,fMinPoints,fIter,fMaxDelta);
    fit->SplineFit(fFitReq);
    fit->Cleanup();
    if (fit->GetKnots()>0) {
      IlcSplineFit *oldFit = entry->GetFit();
      if (oldFit) delete oldFit;
      entry->SetFit(fit);
    } else {
      IlcWarning(Form("sensor %s: no new fit performed. If available, old fit kept.",stringID.Data()));
    }
  }
}

//_____________________________________________________________________________
Int_t IlcDCSSensorArray::NumFits() const 
{
 //
 // Return number of sensors where a succesful fit has been made
 //
  Int_t nfit=0;
  Int_t nsensors = fSensors->GetEntries();
  for ( Int_t isensor=0; isensor<nsensors; isensor++) {
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
    if (entry->GetFit()) nfit++;
  }    
  return nfit;
}
//_____________________________________________________________________________
Double_t IlcDCSSensorArray::GetValue(UInt_t timeSec, Int_t sensor) 
{
  //
  // Return sensor value at time timeSec (obtained from fitted function)
  //  timeSec = time in seconds from start of run
  //

  IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(sensor);
  return entry->GetValue(TTimeStamp((time_t)fStartTime.GetSec()+timeSec,0));
}


//_____________________________________________________________________________
TMap* IlcDCSSensorArray::ExtractDCS(TMap *dcsMap, Bool_t keepStart)
{
 //
 // Extract temperature graphs from DCS maps
 //
 TMap *values = new TMap;
 TObjArray * valueSet;
 //
 // Keep global start/end times
 //    to avoid extrapolations, the fits will only be valid from first 
 //    measured point to last measured point. This is consistent with hardware,
 //    as there would be a new measured point if the value changed.
 
 TTimeStamp startTime=fStartTime;
 TTimeStamp endTime=fEndTime;
 
 Int_t nsensors = fSensors->GetEntries();
 for ( Int_t isensor=0; isensor<nsensors; isensor++) {
   IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
   TString stringID = entry->GetStringID();
   TPair *pair = (TPair*)dcsMap->FindObject(stringID.Data());
   if ( pair ) {                            // only try to read values
                                            // if DCS object available
     valueSet = (TObjArray*)pair->Value();
     TGraph *graph = MakeGraph(valueSet,keepStart);   // MakeGraph sets start/end time
                                            // per sensor
     values->Add(new TObjString(stringID.Data()),graph);
     entry->SetStartTime(fStartTime);
     entry->SetEndTime(fEndTime);
   }
 }
 // Reset global start/end time 
 //    ..... yes, I know this won't get a prize for structured programming..:-)

 fStartTime=startTime;
 fEndTime=endTime;
 return values;
}


//_____________________________________________________________________________
TGraph* IlcDCSSensorArray::MakeGraph(TObjArray* valueSet, Bool_t keepStart){
  //
  // Make graph of temperature values read from DCS map
  //   (spline fit parameters will subsequently be obtained from this graph) 
  //
  Int_t nentries = valueSet->GetEntriesFast(); 
  if ( nentries == 0 ) return 0;
  
  Float_t *x = new Float_t[nentries];
  Float_t *y = new Float_t[nentries];
  Int_t time0=0, previousTime=0;
  TTimeStamp firstTime(0);
  TTimeStamp lastTime(0);
  if (keepStart) { 
     firstTime = fStartTime;
     time0 = firstTime.GetSec();
  }
  Int_t out=0;
  Int_t skipped=0;
  IlcDCSValue *val = (IlcDCSValue *)valueSet->At(0);
  IlcDCSValue::Type type = val->GetType();
  if ( type == IlcDCSValue::kInvalid || type == IlcDCSValue::kBool ) {
     delete [] x;
     delete [] y;
     return 0;
  }
  Float_t value;
  for (Int_t i=0; i<nentries; i++){
    val = (IlcDCSValue *)valueSet->At(i);
    if (!val) continue;
    if (time0==0){
      time0=val->GetTimeStamp();
      firstTime= TTimeStamp((time_t)val->GetTimeStamp(),0);
      lastTime=TTimeStamp((time_t)val->GetTimeStamp(),0);
     }
    switch ( type )
    { 
      case IlcDCSValue::kFloat:
        value = val->GetFloat();
        break;
      case IlcDCSValue::kChar:
        value = static_cast<Float_t>(val->GetChar());
	break;
      case IlcDCSValue::kInt:
        value = static_cast<Float_t>(val->GetInt());
	break;
      case IlcDCSValue::kUInt:
        value = static_cast<Float_t>(val->GetUInt());
	break;
      default:
        continue;
    }
    if (fValCut>0 && TMath::Abs(value)>fValCut) continue;   // refuse values greater than cut
    if (fDiffCut>0 ) {
      if ( out>0 && skipped<10 && TMath::Abs(value-y[out-1])>fDiffCut) {
        skipped++;                               // refuse values changing 
        continue;                                // by > cut  in one time step
      }                                          
      skipped=0;
    }					      
    if (val->GetTimeStamp()-time0>1000000) continue;
    if (val->GetTimeStamp()-previousTime < 1 ) continue;   // refuse duplicate recordings
    previousTime=val->GetTimeStamp();
    lastTime=TTimeStamp((time_t)val->GetTimeStamp(),0);
    x[out] = (val->GetTimeStamp()-time0)/kSecInHour; // give times in fractions of hours 
    y[out] = value;
    out++;    
  }
  if (!keepStart) fStartTime=firstTime;
  fEndTime=lastTime;
  TGraph * graph = new TGraph(out,x,y);
  delete [] x;
  delete [] y;
  return graph;
}

//_____________________________________________________________________________
void IlcDCSSensorArray::RemoveGraphDuplicates(Double_t tolerance){
//
//   Remove points with same y value as the previous measured point
//   (to save space for non-fitted graphs -- i.e. last measured point used)
//
  Int_t nsensors = fSensors->GetEntries();
  for ( Int_t isensor=0; isensor<nsensors; isensor++) {
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
    TGraph *graph = entry->GetGraph();
    Double_t x=-999.,y=-999., x0=-999.,y0=-999.;
    if (graph) {
      Int_t npoints=graph->GetN();
      if (npoints>1) {
        for (Int_t i=npoints-1;i>0;i--) {
	   graph->GetPoint(i,x,y);
	   graph->GetPoint(i-1,x0,y0);
	   if ( TMath::Abs(y-y0) < TMath::Abs(tolerance*y0) ) graph->RemovePoint(i);
	 }
      }
    }
   }
}    

  
//_____________________________________________________________________________
IlcDCSSensor* IlcDCSSensorArray::GetSensor(Int_t IdDCS) 
{
 //
 //  Return sensor information for sensor specified by IdDCS
 //
 Int_t nsensors = fSensors->GetEntries();
 for (Int_t isensor=0; isensor<nsensors; isensor++) {
   IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
   if (entry->GetIdDCS() == IdDCS) return entry;
 }
 return 0;
}
//_____________________________________________________________________________
IlcDCSSensor* IlcDCSSensorArray::GetSensor(const TString& stringID)
{
 //
 //  Return sensor information for sensor specified by stringID
 //
 Int_t nsensors = fSensors->GetEntries();
 for (Int_t isensor=0; isensor<nsensors; isensor++) {
   IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
   if (entry->GetStringID() == stringID) return entry;
 }
 return 0;
}
//_____________________________________________________________________________
IlcDCSSensor* IlcDCSSensorArray::GetSensor(Double_t x, Double_t y, Double_t z)
{
 //
 //  Return sensor closest to given position
 //
 Int_t nsensors = fSensors->GetEntries();
 Double_t dist2min=1e99;
 Double_t xs,ys,zs,dist2;
 Int_t ind=-1;
 for (Int_t isensor=0; isensor<nsensors; isensor++) {
   IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
   xs = entry->GetX();
   ys = entry->GetY();
   zs = entry->GetZ();
   dist2 = (x-xs)*(x-xs) + (y-ys)*(y-ys) + (z-zs)*(z-zs);
   if (dist2 < dist2min) {
      ind=isensor;
      dist2min = dist2;
   }
 }
 if ( ind >= 0 ) {
    return (IlcDCSSensor*)fSensors->At(ind);
 } else {
    return 0;
 }
}
//_____________________________________________________________________________
IlcDCSSensor* IlcDCSSensorArray::GetSensorNum(Int_t ind)
{
 //
 //  Return sensor given by array index
 //
 return (IlcDCSSensor*)fSensors->At(ind);
}

//_____________________________________________________________________________
Int_t IlcDCSSensorArray::SetSensor(const TString& stringID,
                          const  IlcDCSSensor& sensor)
{
 //
 //  Update sensor information for sensor specified by stringID
 //
 Int_t nsensors = fSensors->GetEntries();
 for (Int_t isensor=0; isensor<nsensors; isensor++) {
   IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
   if (entry->GetStringID() == stringID) 
     {
      new ((*fSensors)[isensor])IlcDCSSensor(sensor);
      return isensor;
     }
 }
 return -1;
}
//_____________________________________________________________________________
void IlcDCSSensorArray::SetSensorNum(const Int_t ind, const IlcDCSSensor& sensor)
{
 //
 //  Update sensor information for sensor at index ind
 //
   new ((*fSensors)[ind])IlcDCSSensor(sensor);
   return;
}
//_____________________________________________________________________________
void IlcDCSSensorArray::RemoveSensorNum(Int_t ind)
{
 //
 //  Return sensor given by array index
 //

  delete fSensors->RemoveAt(ind);
  fSensors->Compress();
}
//_____________________________________________________________________________
void IlcDCSSensorArray::RemoveSensor(Int_t IdDCS)
{
 //
 //  Deletes Sensor by given IdDCS
 //

  Int_t nsensors = fSensors->GetEntries();
  for (Int_t isensor=0; isensor<nsensors; isensor++) { // loop over sensors
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
    if (entry->GetIdDCS()==IdDCS) {
      delete fSensors->RemoveAt(isensor);
      break;
    }
  }
  fSensors->Compress();
}
//_____________________________________________________________________________
TArrayI IlcDCSSensorArray::OutsideThreshold(Double_t threshold, UInt_t timeSec, Bool_t below) const
{
 //
 // Return sensors with values outside threshold at time defined by second
 // parameter
 // By default sensors with values below threshold are listed, if third
 // parameter is set to kFALSE sensors with values above threshold are listed
 //
  Int_t nsensors = fSensors->GetEntries();
  TArrayI array(nsensors);
  Int_t outside=0;
  for (Int_t isensor=0; isensor<nsensors; isensor++) { // loop over sensors
    IlcDCSSensor *entry = (IlcDCSSensor*)fSensors->At(isensor);
    Double_t val=entry->GetValue(timeSec);
    if (below) {
      if (val<threshold) array[outside++] = entry->GetIdDCS();
    } else {
      if (val>threshold) array[outside++] = entry->GetIdDCS();
    }    
  }
  array.Set(outside);
  return array;
}
 
//_____________________________________________________________________________
Int_t IlcDCSSensorArray::GetFirstIdDCS() const
{
 //
 //  Return DCS Id of first sensor
 //
 if ( fSensors != 0 ) {
    return ((IlcDCSSensor*)fSensors->At(0))->GetIdDCS();
 } else {
    return 0;
 }
}

//_____________________________________________________________________________
Int_t IlcDCSSensorArray::GetLastIdDCS() const 
{
 //
 //  Return DCS Id of last sensor
 //
 if ( fSensors != 0 ) {
    Int_t last = fSensors->GetEntries();
    return ((IlcDCSSensor*)fSensors->At(last-1))->GetIdDCS();
 } else {
    return 0;
 }
}
//_____________________________________________________________________________
void IlcDCSSensorArray::ClearGraph()
{
  //
  // Delete DCS graphs from all sensors in array
  //
   
   Int_t nsensors = fSensors->GetEntries();
   for ( Int_t isensor=0; isensor<nsensors; isensor++) {
     IlcDCSSensor *sensor = (IlcDCSSensor*)fSensors->At(isensor);
     TGraph *gr = sensor->GetGraph();
     if ( gr != 0 ) {
       delete gr;
       gr = 0;
     }
     sensor->SetGraph(0);
   }
}
//_____________________________________________________________________________
void IlcDCSSensorArray::ClearFit()
{
  //
  // Delete spline fits from all sensors in array
  //

   Int_t nsensors = fSensors->GetEntries();
   for ( Int_t isensor=0; isensor<nsensors; isensor++) {
     IlcDCSSensor *sensor = (IlcDCSSensor*)fSensors->At(isensor);
     IlcSplineFit *fit = sensor->GetFit();
     if ( fit != 0 ) {
       delete fit;
       fit = 0;
     }
     sensor->SetFit(0);
   }
}
//_____________________________________________________________________________
void IlcDCSSensorArray::AddSensors(IlcDCSSensorArray *newSensors)
{
  //
  // add sensors from two sensor arrays
  //
  
  Int_t numNew = newSensors->NumSensors();
  Int_t numOld = fSensors->GetEntries();
  fSensors->Expand(numOld+numNew);
  for (Int_t i=0;i<numNew;i++) {
    IlcDCSSensor *sens = newSensors->GetSensorNum(i);
    new ((*fSensors)[numOld+i]) IlcDCSSensor(*sens);
  }
}  
  
