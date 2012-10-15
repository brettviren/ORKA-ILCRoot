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

/* $Id: IlcGenPythiaEventHeader.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcGenPythiaEventHeader.h"
ClassImp(IlcGenPythiaEventHeader)


IlcGenPythiaEventHeader::IlcGenPythiaEventHeader():
    fProcessType(0),
    fTrials(0),
    fNJets(0),
    fNUQJets(0), 
    fXJet(-1.),
    fYJet(-1.),
    fInMediumLength(0.),
    fImpactParameter(-1),
    fPtHard(0.) 
{
// Default Constructor
    for (Int_t i = 0; i < 4; i++) fZquench[i] = 0.;
    for (Int_t i = 0; i < 4; i++)
      for (Int_t j = 0; j < 10; j++) {
	fJets[i][j] = 0.;
	fUQJets[i][j] = 0.;
      }
}

IlcGenPythiaEventHeader::IlcGenPythiaEventHeader(const char* name):
    IlcGenEventHeader(name),
    fProcessType(0),
    fTrials(0),
    fNJets(0),
    fNUQJets(0), 
    fXJet(-1.),
    fYJet(-1.),
    fInMediumLength(0.),
    fImpactParameter(-1),
    fPtHard(0.) 
{
// Constructor
    for (Int_t i = 0; i < 4; i++) fZquench[i] = 0.;
    for (Int_t i = 0; i < 4; i++)
      for (Int_t j = 0; j < 10; j++) {
	fJets[i][j] = 0.;
	fUQJets[i][j] = 0.;
      }
}

void IlcGenPythiaEventHeader::AddJet(Float_t px, Float_t py, Float_t pz, Float_t e)
{
//
//  Add a jet 
//
    if (fNJets < 10) {
	fJets[0][fNJets] = px;
	fJets[1][fNJets] = py;
	fJets[2][fNJets] = pz;
	fJets[3][fNJets] = e;
	fNJets++;
    } else {
	printf("\nWarning: More than 10 jets triggered !!\n");
    }
}

void IlcGenPythiaEventHeader::AddUQJet(Float_t px, Float_t py, Float_t pz, Float_t e)
{
//
//  Add a jet 
//
    if (fNUQJets < 10) {
	fUQJets[0][fNUQJets] = px;
	fUQJets[1][fNUQJets] = py;
	fUQJets[2][fNUQJets] = pz;
	fUQJets[3][fNUQJets] = e;
	fNUQJets++;
    } else {
	printf("\nWarning: More than 10 jets triggered !!\n");
    }
}

void IlcGenPythiaEventHeader::SetZQuench(Double_t z[4])
{
    //
    // Set quenching fraction
    //
    for (Int_t i = 0; i < 4; i++) fZquench[i] = z[i];
}

void IlcGenPythiaEventHeader::GetZQuench(Double_t z[4]) const
{
    //
    // Get quenching fraction
    //
    for (Int_t i = 0; i < 4; i++) z[i] = fZquench[i];
}

void IlcGenPythiaEventHeader::TriggerJet(Int_t i, Float_t p[4])
{
//
// Give back jet #i
//
    if (i >= fNJets) {
	printf("\nWarning: TriggerJet, index out of Range!!\n");
    } else {
	p[0] = fJets[0][i];
	p[1] = fJets[1][i];
	p[2] = fJets[2][i];
	p[3] = fJets[3][i];
    }
}

void IlcGenPythiaEventHeader::UQJet(Int_t i, Float_t p[4])
{
//
// Give back jet #i
//
    if (i >= fNUQJets) {
	printf("\nWarning: Unquenched Jets, index out of Range!!\n");
    } else {
	p[0] = fUQJets[0][i];
	p[1] = fUQJets[1][i];
	p[2] = fUQJets[2][i];
	p[3] = fUQJets[3][i];
    }
}

void IlcGenPythiaEventHeader::SetXYJet(Double_t x, Double_t y)
{

//
//  Add jet production point
//
    fXJet = x; 
    fYJet = y; 
}
