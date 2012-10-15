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



//
// Class to handle the AOD tracks with good HMPID data 
// Author: Levente Molnar
// levente.molnar@cern.ch , March 2012
// 

#include "IlcAODHMPIDrings.h"

ClassImp(IlcAODHMPIDrings)

//________________________________________________________________________________________________________________________________________________________

IlcAODHMPIDrings::IlcAODHMPIDrings()
                   :TObject(),
                    fHmpidAODtrkId(0),
                    fHmpidAODqn(0),
                    fHmpidAODcluIdx(0),
                    fHmpidAODtrkTheta(0),
                    fHmpidAODtrkPhi(0),
                    fHmpidAODsignal(0),
                    fHmpidAODocc(0),
                    fHmpidAODchi2(0),
                    fHmpidAODtrkX(0),
                    fHmpidAODtrkY(0),
                    fHmpidAODmipX(0),
                    fHmpidAODmipY(0)

{
  //default ctor 
   for(Int_t isp = 0 ; isp <IlcPID::kSPECIES; isp++)   fHmpidAODpid[isp] = 0;
       for ( Int_t ico = 0 ; ico < 3; ico++) fHMPIDmom[ico] = 0;
}

//________________________________________________________________________________________________________________________________________________________
IlcAODHMPIDrings::IlcAODHMPIDrings(
                    Int_t trkId,
                    Int_t qn, 
                    Int_t cluIdx,
                    Double32_t  trkTheta,
                    Double32_t trkPhi,
                    Double32_t signal,
                    Double32_t occ,
                    Double32_t chi2,
                    Double32_t trkX,
                    Double32_t trkY,
                    Double32_t mipX,
                    Double32_t mipY,
                    Double32_t *pid,
                    Double32_t *p         ):
                    TObject(),
                    
                    fHmpidAODtrkId(trkId),
                    fHmpidAODqn(qn),
                    fHmpidAODcluIdx(cluIdx),
                    fHmpidAODtrkTheta(trkTheta),
                    fHmpidAODtrkPhi(trkPhi),
                    fHmpidAODsignal(signal),
                    fHmpidAODocc(occ),
                    fHmpidAODchi2(chi2),
                    fHmpidAODtrkX(trkX),
                    fHmpidAODtrkY(trkY),
                    fHmpidAODmipX(mipX),
                    fHmpidAODmipY(mipY)

                    
{
       //             
       for(Int_t isp = 0 ; isp <IlcPID::kSPECIES; isp++)   fHmpidAODpid[isp] = pid[isp];
       for ( Int_t ico = 0 ; ico < 3; ico++) fHMPIDmom[ico] = p[ico];
                           
}
//________________________________________________________________________________________________________________________________________________________
IlcAODHMPIDrings::IlcAODHMPIDrings(const IlcAODHMPIDrings& hmpidAOD):
    
                    TObject(hmpidAOD),
                    fHmpidAODtrkId(hmpidAOD.fHmpidAODtrkId),
                    fHmpidAODqn(hmpidAOD.fHmpidAODqn),
                    fHmpidAODcluIdx(hmpidAOD.fHmpidAODcluIdx),
                    fHmpidAODtrkTheta(hmpidAOD.fHmpidAODtrkTheta),
                    fHmpidAODtrkPhi(hmpidAOD.fHmpidAODtrkPhi),
                    fHmpidAODsignal(hmpidAOD.fHmpidAODsignal),
                    fHmpidAODocc(hmpidAOD.fHmpidAODocc),
                    fHmpidAODchi2(hmpidAOD.fHmpidAODchi2),
                    fHmpidAODtrkX(hmpidAOD.fHmpidAODtrkX),
                    fHmpidAODtrkY(hmpidAOD.fHmpidAODtrkY),
                    fHmpidAODmipX(hmpidAOD.fHmpidAODmipX),
                    fHmpidAODmipY(hmpidAOD.fHmpidAODmipY)

                    
{
       //             
       for(Int_t isp = 0 ; isp <IlcPID::kSPECIES; isp++)   fHmpidAODpid[isp] = hmpidAOD.fHmpidAODpid[isp];
       for ( Int_t ico = 0 ; ico < 3; ico++) fHMPIDmom[ico] = hmpidAOD.fHMPIDmom[ico];
                           
}

//________________________________________________________________________________________________________________________________________________________
IlcAODHMPIDrings& IlcAODHMPIDrings::operator=(const IlcAODHMPIDrings& hmpidAOD)
{
     if (this!=&hmpidAOD) {   
                    IlcAODHMPIDrings::operator=(hmpidAOD);  
                    fHmpidAODtrkId = hmpidAOD.fHmpidAODtrkId;        
                    fHmpidAODqn = hmpidAOD.fHmpidAODqn;
                    fHmpidAODcluIdx = hmpidAOD.fHmpidAODcluIdx;
                    fHmpidAODtrkTheta = hmpidAOD.fHmpidAODtrkTheta;
                    fHmpidAODtrkPhi = hmpidAOD.fHmpidAODtrkPhi;
                    fHmpidAODsignal = hmpidAOD.fHmpidAODsignal;
                    fHmpidAODocc = hmpidAOD.fHmpidAODocc;
                    fHmpidAODchi2 = hmpidAOD.fHmpidAODchi2;
                    fHmpidAODtrkX = hmpidAOD.fHmpidAODtrkX;
                    fHmpidAODtrkY = hmpidAOD.fHmpidAODtrkY;
                    fHmpidAODmipX = hmpidAOD.fHmpidAODmipX;
                    fHmpidAODmipY = hmpidAOD.fHmpidAODmipY;
                    
                    for(Int_t isp = 0 ; isp <IlcPID::kSPECIES; isp++)   fHmpidAODpid[isp] = hmpidAOD.fHmpidAODpid[isp];
                    for ( Int_t ico = 0 ; ico < 3; ico++) fHMPIDmom[ico] = hmpidAOD.fHMPIDmom[ico];

     }
     
     return *this;
                                  
}
//________________________________________________________________________________________________________________________________________________________
void IlcAODHMPIDrings::GetHmpPidProbs(Double32_t *pid)
{
  // Gets probabilities of each particle type (in HMPID)
  for (Int_t i=0; i<IlcPID::kSPECIES; i++) pid[i]=fHmpidAODpid[i];
}
//________________________________________________________________________________________________________________________________________________________
void  IlcAODHMPIDrings::GetHmpMom(Double32_t *mom)
{
  for( Int_t ico = 0 ; ico < 3; ico++) mom[ico] = fHMPIDmom[ico];
}
//________________________________________________________________________________________________________________________________________________________
void IlcAODHMPIDrings::SetHmpPidProbs(Double32_t *pid)
{
  // Gets probabilities of each particle type (in HMPID)
  for (Int_t i=0; i<IlcPID::kSPECIES; i++) fHmpidAODpid[i] = pid[i];
}

//________________________________________________________________________________________________________________________________________________________
void  IlcAODHMPIDrings::SetHmpMom(Double32_t *mom)
{
  for( Int_t ico = 0 ; ico < 3; ico++) fHMPIDmom[ico] = mom[ico];  
}
//________________________________________________________________________________________________________________________________________________________

