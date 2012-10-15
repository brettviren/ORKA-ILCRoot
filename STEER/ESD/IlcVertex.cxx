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

//-----------------------------------------------------------------
//           Implementation of the base Vertex class
//           This class contains the Secondary Vertex
//           of a set of tracks
//           And it is the base class for primary vertices
// Origin: F.Prino, Torino, prino@to.infn.it
//-----------------------------------------------------------------

#include "IlcVertex.h"


ClassImp(IlcVertex)

//--------------------------------------------------------------------------
IlcVertex::IlcVertex() :
  IlcVVertex(),
  fSigma(0),
  fNContributors(0),
  fNIndices(0),
  fIndices(0)
{
//
// Default Constructor, set everything to 0
//
  for(Int_t k=0;k<3;k++) fPosition[k]   = 0;
}

//--------------------------------------------------------------------------
IlcVertex::IlcVertex(Double_t position[3],Double_t dispersion,
		     Int_t nContributors):
  IlcVVertex(),
  fSigma(dispersion),
  fNContributors(nContributors),
  fNIndices(0),
  fIndices(0)
{
  //
  // Standard Constructor
  //

  for(Int_t k=0;k<3;k++) fPosition[k]   = position[k];
  SetName("BaseVertex");

}

//--------------------------------------------------------------------------
IlcVertex::IlcVertex(const IlcVertex &source):
  IlcVVertex(source),
  fSigma(source.GetDispersion()),
  fNContributors(source.GetNContributors()),
  fNIndices(source.GetNIndices()),
  fIndices(0x0)
{
  //
  // Copy constructor
  //
  for(Int_t i=0;i<3;i++)fPosition[i] = source.fPosition[i];
  if(source.fNIndices>0) {
    fIndices = new UShort_t[fNIndices];
    memcpy(fIndices,source.fIndices,fNIndices*sizeof(UShort_t));
  }
}

//--------------------------------------------------------------------------
IlcVertex &IlcVertex::operator=(const IlcVertex &source){
  //
  // assignment operator
  //
  if(&source != this){
    IlcVVertex::operator=(source);
    for(Int_t i=0;i<3;i++)fPosition[i] = source.fPosition[i];
    fSigma = source.GetDispersion();
    fNContributors = source.GetNContributors();
    fNIndices = source.GetNIndices();
    if(fIndices)delete [] fIndices;
    fIndices = 0;
    if(fNIndices>0) {
      fIndices = new UShort_t[fNIndices];
      memcpy(fIndices,source.fIndices,fNIndices*sizeof(UShort_t));
    }
  }
  return *this;
}


//--------------------------------------------------------------------------
IlcVertex::~IlcVertex() {
//  
// Default Destructor
//
  delete [] fIndices;
  fIndices = 0;
}

void IlcVertex::Clear(Option_t* option) 
{
    // Delete allocated memory
    delete [] fIndices;
    fIndices = 0;
    IlcVVertex::Clear(option);
}

//--------------------------------------------------------------------------
void IlcVertex::GetXYZ(Double_t position[3]) const {
//
// Return position of the vertex in global frame
//
  position[0] = fPosition[0];
  position[1] = fPosition[1];
  position[2] = fPosition[2];

  return;
}
//--------------------------------------------------------------------------
void IlcVertex::GetCovarianceMatrix(Double_t covmatrix[6]) const {
//
// Fake method (is implmented in IlcESDVertex)
//
  for(Int_t i=0;i<6;i++) covmatrix[i] = -999.;

  return;
}
//--------------------------------------------------------------------------
void IlcVertex::SetIndices(Int_t nindices,UShort_t *indices) {
//
// Set indices of tracks used for vertex determination 
//
  if(fNContributors<1)  { printf("fNContributors<1"); return; }
  fNIndices = nindices;
  delete [] fIndices;
  fIndices = new UShort_t[fNIndices];
  for(Int_t i=0;i<fNIndices;i++) fIndices[i] = indices[i]; 
  return;
}
//--------------------------------------------------------------------------
Bool_t IlcVertex::UsesTrack(Int_t index) const {
//
// checks if a track is used for the vertex 
//
  if(fNIndices<1)  {/* printf("fNIndices<1"); */return kFALSE; }
  for(Int_t i=0;i<fNIndices;i++) {
    if((Int_t)fIndices[i]==index) return kTRUE;
  }
  return kFALSE;
}
//--------------------------------------------------------------------------
void IlcVertex::Print(Option_t* /*option*/) const {
//
// Print out information on all data members
//
  printf("Vertex position:\n");
  printf("   x = %f\n",fPosition[0]);
  printf("   y = %f\n",fPosition[1]);
  printf("   z = %f\n",fPosition[2]);
  printf(" Dispersion = %f\n",fSigma);
  printf(" # tracks = %d\n",fNContributors);

  return;
}




