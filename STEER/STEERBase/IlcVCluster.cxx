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

/* $Id: IlcVCluster.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-----------------------------------------------------------------
//
//   Virtual class to access calorimeter 
//   (EMCAL, PHOS, PMD, FMD) cluster data
//   Author: Gustavo Conesa Balbastre LPSC-Grenoble
//
//-----------------------------------------------------------------

#include "IlcVCluster.h"

ClassImp(IlcVCluster)
  
  IlcVCluster::IlcVCluster(const IlcVCluster& vclus) :
    TObject(vclus) { ; } // Copy constructor

IlcVCluster& IlcVCluster::operator=(const IlcVCluster& vclus)
{ 
  //Assignment operator
  if (this!=&vclus) { 
    TObject::operator=(vclus); 
  }
  
  return *this; 
}


