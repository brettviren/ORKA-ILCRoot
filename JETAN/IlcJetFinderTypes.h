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


/* $Id: IlcJetFinderTypes.h 15955 2006-11-22 13:45:46Z morsch $ */

//=========================================================================
// Modified class for JETAN
// Some flags have been changed
// Some are not used for the moment
// Author: magali.estienne@ires.in2p3.fr
//=========================================================================
//  Enumerated types for use in JetFinder classes
//
//*-- Author: Mark Horner (LBL/UCT)
//

#ifndef ILCJETFINDERBGCALCTYPE_T
#define ILCJETFINDERBGCALCTYPE_T

        typedef enum {	kRatio, kCone, kConstant  
	} IlcJetFinderBGCalcType_t;
#endif

#ifndef ILCJETFINDERRESETTYPE_T
#define ILCJETFINDERRESETTYPE_T

        typedef enum {  kResetData, kResetTracks, kResetDigits, kResetParameters,
                        kResetAll, kResetPartons, kResetParticles, kResetJets
        } IlcJetFinderResetType_t;
#endif

#ifndef  ILCJETFINDERTRACKTYPE_T
#define  ILCJETFINDERTRACKTYPE_T
	typedef enum {	kAllP, kEM, kCharged, kNeutral, kHadron, kChargedHadron, kNoTracks, kEMChargedPi0, kNoNeutronNeutrinoKlong
	} IlcJetFinderTrackType_t;
#endif

#ifndef  ILCJETFINDERSMEARINGTYPE_T
#define  ILCJETFINDERSMEARINGTYPE_T
	typedef enum {	kSmear, kEfficiency , kSmearEffic, kPerfectTracks
	} IlcJetFinderSmearingType_t;
#endif
	
#ifndef  ILCJETFINDEREMCALTYPE_T
#define  ILCJETFINDEREMCALTYPE_T
typedef enum {	kHits, kTimeCut,kNoHits, kDigits, kClusters 
	} IlcJetFinderEmcalType_t;
#endif

#ifndef  ILCJETFINDERFILETYPE_T
#define  ILCJETFINDERFILETYPE_T
	typedef enum {	kHijing,kPythia,kData
	} IlcJetFinderFileType_t;
#endif

#ifndef  ILCJETFINDERUA1UNITFLAGTYPE_T
#define  ILCJETFINDERUA1UNITFLAGTYPE_T
 	typedef enum {	kInCurrentJet, kInJet, kOutJet, kBelowMinEt
	} IlcJetFinderUnitFlagType_t;
#endif

#ifndef  ILCJETFINDERUA1UNITCUTFLAGTYPE_T
#define  ILCJETFINDERUA1UNITCUTFLAGTYPE_T
 	typedef enum {	kPtSmaller, kPtHigher
	} IlcJetFinderUnitCutFlagType_t;
#endif

#ifndef  ILCJETFINDERUA1UNITSIGNALFLAGTYPE_T
#define  ILCJETFINDERUA1UNITSIGNALFLAGTYPE_T
 	typedef enum {	kGood, kBad
	} IlcJetFinderUnitSignalFlagType_t;
#endif

#ifndef  ILCJETFINDERUNITDETECTORFLAGTYPE_T
#define  ILCJETFINDERUNITDETECTORFLAGTYPE_T
 	typedef enum {	kTpc, kEmcal, kAll
	} IlcJetFinderUnitDetectorFlagType_t;
#endif
