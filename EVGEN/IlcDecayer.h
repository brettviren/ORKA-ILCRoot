#ifndef ILC_DECAYER__H
#define ILC_DECAYER__H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDecayer.h 56668 2012-05-24 13:14:33Z hristov $ */

#include "RVersion.h"
#include "TVirtualMCDecayer.h"

typedef TVirtualMCDecayer IlcDecayer;

#if ROOT_VERSION_CODE >= 197633  //Corresponds to Root v3-04-01
typedef enum 
{
    kBSemiElectronic, kSemiElectronic, kDiElectron, kBSemiMuonic, kSemiMuonic, kDiMuon, kJpsiDiMuon,
    kBJpsiDiMuon, kBJpsiDiElectron, 
    kBPsiPrimeDiMuon, kBPsiPrimeDiElectron, kPiToMu, kKaToMu, 
    kNoDecay, kHadronicD, kHadronicDWithout4Bodies, kOmega, kLambda, kPhiKK, 
    kAll, kNoDecayHeavy, kHardMuons, kBJpsi,
    kWToMuon,kWToCharm, kWToCharmToMuon, kZDiMuon, kZDiElectron, kNeutralPion, kAllMuonic,
    kChiToJpsiGammaToMuonMuon, kChiToJpsiGammaToElectronElectron, kNoDecayBeauty, kPsiPrimeJpsiDiElectron,
    kElectronEM, kGammaEM, kDiElectronEM, kBeautyUpgrade
} Decay_t;
#endif

#endif //ILC_DECAYER__H

