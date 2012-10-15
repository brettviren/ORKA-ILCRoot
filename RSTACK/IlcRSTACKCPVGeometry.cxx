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

/* $Id: IlcRSTACKCPVGeometry.cxx 14911 2006-08-28 10:01:56Z kharlov $ */

//_________________________________________________________________________
// Geometry class for RSTACK 
// Its data members provide geometry parametrization of RSTACK
// which can be changed in the constructor only.
// Author: Yuri Kharlov (IHEP, Protvino)
// 15 September 2000
//
// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcRSTACKCPVGeometry.h"

ClassImp(IlcRSTACKCPVGeometry)

//____________________________________________________________________________
IlcRSTACKCPVGeometry::IlcRSTACKCPVGeometry()
  : fNumberOfCPVLayers(1),
    fNumberOfCPVPadsPhi(128),
    fNumberOfCPVPadsZ(56),
    fCPVPadSizePhi(1.13),
    fCPVPadSizeZ(2.26),
    fNumberOfCPVChipsPhi(8),
    fNumberOfCPVChipsZ(8),
    fCPVGasThickness(1.3),
    fCPVTextoliteThickness(0.1),
    fCPVCuNiFoilThickness(56e-04)
{

  // Initializes the CPV parameters
  fCPVFrameSize[0]       = 2.5;
  fCPVFrameSize[1]       = 5.1;
  fCPVFrameSize[2]       = 2.5;
  fGassiplexChipSize[0]  = 4.2;
  fGassiplexChipSize[1]  = 0.1;
  fGassiplexChipSize[2]  = 6.0;
  fFTPosition[0]         = 0.7;
  fFTPosition[1]         = 2.2;
  fFTPosition[2]         = 3.6;
  fFTPosition[3]         = 5.1;

  fCPVActiveSize[0]      = fNumberOfCPVPadsPhi * fCPVPadSizePhi;
  fCPVActiveSize[1]      = fNumberOfCPVPadsZ   * fCPVPadSizeZ;
  fCPVBoxSize[0]         = fCPVActiveSize[0] + 2 * fCPVFrameSize[0];
  fCPVBoxSize[1]         = fCPVFrameSize[1] * fNumberOfCPVLayers + 0.1;
  fCPVBoxSize[2]         = fCPVActiveSize[1] + 2 * fCPVFrameSize[2];

}
//____________________________________________________________________________
