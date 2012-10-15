/**************************************************************************
 * Author: Panos Christakoglou.                                           *
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

/* $Id: IlcDetectorTagCuts.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-----------------------------------------------------------------
//                   IlcDetectorTagCuts class
//   This is the class to deal with the Detector tag level cuts
//   Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-----------------------------------------------------------------

class IlcLog;

#include "IlcDetectorTag.h"
#include "IlcDetectorTagCuts.h"

#include "TObjString.h"
#include "TString.h"

ClassImp(IlcDetectorTagCuts)

//___________________________________________________________________________
IlcDetectorTagCuts::IlcDetectorTagCuts() :
  TObject(),
  fDetectorsReco(0),
  fDetectorsDAQ(0),
  fDetectorsFlag(kFALSE),
  fDetectorValidityMatch(),
  fDetectorValidityFlag()
{
  //Default constructor which calls the Reset method.
  for (int iter = 0; iter<IlcDAQ::kHLTId; iter++) {
    fDetectorValidityMatch[iter] = 0;
    fDetectorValidityFlag[iter] = 0;
  }
}

//___________________________________________________________________________
IlcDetectorTagCuts::~IlcDetectorTagCuts() {  
  //Defaut destructor.
}

//___________________________________________________________________________
Bool_t IlcDetectorTagCuts::IsAccepted(IlcDetectorTag *detTag) const {
  //Returns true if the event is accepted otherwise false.
  if (fDetectorsFlag) {
    Bool_t daqsel = (detTag->GetIntDetectorMaskDAQ() & fDetectorsDAQ) > 0;
    Bool_t recsel = (detTag->GetIntDetectorMaskReco() & fDetectorsReco) > 0;
    Bool_t valsel = kTRUE;
    for (int iter=0;  iter<IlcDAQ::kHLTId; iter++) {
      if (fDetectorValidityFlag[iter])
	if (!(fDetectorValidityMatch[iter] == detTag->GetDetectorValidityRange(iter)))
	  valsel = kFALSE;
    }
    return (daqsel && recsel && valsel);
  }
  return true;

//   if(fDetectorsFlag){
//     TString detStr = fDetectors;
//     TObjArray *activeDetectors = detTag->GetDetectorMask();
//     for (Int_t iDet = 0; iDet < activeDetectors->GetEntries(); iDet++) {
//       TObjString *detectorString = (TObjString *)activeDetectors->At(iDet);
//       if (!IsSelected(detectorString->GetString(), detStr))return kFALSE;
//     }
//   }
//   return kTRUE;
}
  
void IlcDetectorTagCuts::SetDetectorValidityValue(TString det, UShort_t val)
{
  // Set Validity requiement for detector

  Short_t detid = IlcDAQ::DetectorID(det.Data());
  if (detid >= 0) {
    fDetectorValidityMatch[detid] = val;
    fDetectorsFlag = kTRUE;
  }
}

//___________________________________________________________________________
// Bool_t IlcDetectorTagCuts::IsSelected(TString detName, TString& detectors) const {
//   //Returns true if the detector is included
//   if ((detectors.CompareTo("ALL") == 0) ||
//       detectors.BeginsWith("ALL ") ||
//       detectors.EndsWith(" ALL") ||
//       detectors.Contains(" ALL ")) {
//     detectors = "ALL";
//     return kTRUE;
//   }
  
//   // search for the given detector
//   Bool_t result = kFALSE;
//   if ((detectors.CompareTo(detName) == 0) ||
//       detectors.BeginsWith(detName+" ") ||
//       detectors.EndsWith(" "+detName) ||
//       detectors.Contains(" "+detName+" ")) {
//     detectors.ReplaceAll(detName, "");
//     result = kTRUE;
//   }

//   // clean up the detectors string
//   while (detectors.Contains("  ")) detectors.ReplaceAll("  ", " ");
//   while (detectors.BeginsWith(" ")) detectors.Remove(0, 1);
//   while (detectors.EndsWith(" ")) detectors.Remove(detectors.Length()-1, 1);
 
//   return result;
// }
