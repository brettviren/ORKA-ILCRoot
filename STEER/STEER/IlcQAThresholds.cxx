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

#include "IlcQAThresholds.h"

#include "IlcDAQ.h"

ClassImp(IlcQAThresholds)

  IlcQAThresholds::IlcQAThresholds(Int_t detId): TNamed(), fThresholds(), fDetectorId(detId)
{
  // constructor

  fThresholds.SetOwner(kTRUE);
}

IlcQAThresholds::~IlcQAThresholds()
{
  // destructor
}

const char* IlcQAThresholds::GetName() const 
{
  return IlcDAQ::OnlineName(fDetectorId);
}

const char* IlcQAThresholds::GetTitle() const 
{
  return IlcDAQ::OnlineName(fDetectorId);
}

Int_t IlcQAThresholds::GetDetectorId()
{
  return fDetectorId;
}

void IlcQAThresholds::SetDetectorId(Int_t i)
{
  fDetectorId = i;
}

void IlcQAThresholds::AddThreshold(TParameter<long>* item)
{
  // Add a threshold at the end of the array of thresholds.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.Add(item);
}
void IlcQAThresholds::AddThreshold(TParameter<int>* item)
{
  // Add a threshold at the end of the array of thresholds.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.Add(item);
}
void IlcQAThresholds::AddThreshold(TParameter<double>* item)
{
  // Add a threshold at the end of the array of thresholds.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.Add(item);
}
void IlcQAThresholds::AddThreshold(TParameter<float>* item)
{
  // Add a threshold at the end of the array of thresholds.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.Add(item);
}

void IlcQAThresholds::AddThresholdAt(TParameter<int>* item, Int_t index)
{
  // Add a threshold at index 'index' in the array of thresholds.
  // If index is larger than the current size of the array, expand the array.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.AddAtAndExpand(item, index);
}
void IlcQAThresholds::AddThresholdAt(TParameter<long>* item, Int_t index)
{
  // Add a threshold at index 'index' in the array of thresholds.
  // If index is larger than the current size of the array, expand the array.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.AddAtAndExpand(item, index);
}
void IlcQAThresholds::AddThresholdAt(TParameter<double>* item, Int_t index)
{
  // Add a threshold at index 'index' in the array of thresholds.
  // If index is larger than the current size of the array, expand the array.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.AddAtAndExpand(item, index);
}
void IlcQAThresholds::AddThresholdAt(TParameter<float>* item, Int_t index)
{
  // Add a threshold at index 'index' in the array of thresholds.
  // If index is larger than the current size of the array, expand the array.
  // Ownership of the object is transfered to IlcQAThresholds.

  fThresholds.AddAtAndExpand(item, index);
}

TObject* IlcQAThresholds::GetThreshold(Int_t i)
{
  // Return the object at position i. Returns 0 if i is out of bounds.

  return fThresholds.At(i);
}

Int_t IlcQAThresholds::GetSize()
{
  // Return the number of elements in the thresholds array. 
  // Beware that it is not the number of thresholds, as some elements of the array can be null. 

  return fThresholds.GetSize();
}
