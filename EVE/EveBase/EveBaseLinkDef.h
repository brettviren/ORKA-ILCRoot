// $Id: EveBaseLinkDef.h 58244 2012-08-21 12:57:19Z hristov $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#pragma link off all functions;
#pragma link off all globals;
#pragma link off all classes;

// IlcEveApplication
#pragma link C++ class IlcEveApplication+;

// IlcEveManager
#pragma link C++ class IlcEveManager+;

// IlcEveConfigManager
#pragma link C++ class IlcEveConfigManager+;

// IlcEveEventManager
#pragma link C++ class IlcEveEventManager+;
#pragma link C++ class IlcEveEventManagerEditor+;
#pragma link C++ class IlcEveEventManagerWindow+;

// IlcEveEventSelector
#pragma link C++ class IlcEveEventSelector+;
#pragma link C++ class IlcEveEventSelectorWindow+;

// IlcEveMacro and IlcEveMacroExecutor
#pragma link C++ class IlcEveMacro+;
#pragma link C++ class IlcEveMacroEditor+;
#pragma link C++ class IlcEveMacroExecutor+;
#pragma link C++ class IlcEveMacroExecutorWindow+;

// Special GED editor for VizDB interaction.
#pragma link C++ class IlcEveGedEditor+;
#pragma link C++ class IlcEveGedNameFrame+;

// Various
#pragma link C++ class IlcEveKineTools+;
#pragma link C++ class IlcEveMagField+;
#pragma link C++ class IlcEveVSDCreator+;
#pragma link C++ class IlcEveMultiView+;

// IlcEveTrack
#pragma link C++ class IlcEveTrack+;
#pragma link C++ class IlcEveTracklet+;

// IlcEveTrackcounter
#pragma link C++ class IlcEveTrackCounter+;
#pragma link C++ class IlcEveTrackCounterEditor+;

// IlcEveTrackFitter
#pragma link C++ class IlcEveTrackFitter+;
#pragma link C++ class IlcEveTrackFitterEditor+;

#pragma link C++ class IlcEveCosmicRayFitter+;
#pragma link C++ class IlcEveCosmicRayFitterEditor+;

// IlcEveJetPlane
#pragma link C++ class IlcEveJetPlane+;
#pragma link C++ class IlcEveJetPlaneGL+;
#pragma link C++ class IlcEveJetPlaneEditor+;
#pragma link C++ class IlcEveJetPlaneEditor::StaticDataWindow+;


// IlcEveCascade
#pragma link C++ class IlcEveCascade+;
#pragma link C++ class IlcEveCascadeEditor+;
#pragma link C++ class IlcEveCascadeList+;
#pragma link C++ class IlcEveCascadeListEditor+;

// IlcEveV0
#pragma link C++ class IlcEveV0+;
#pragma link C++ class IlcEveV0List+;
#pragma link C++ class IlcEveV0Editor+;
#pragma link C++ class IlcEveV0ListEditor+;

// IlcEveKink
#pragma link C++ class IlcEveKink+;
#pragma link C++ class IlcEveKinkList+;
#pragma link C++ class IlcEveKinkEditor+;
#pragma link C++ class IlcEveKinkListEditor+;

// IlcEveHF
#pragma link C++ class IlcEveHF+;
#pragma link C++ class IlcEveHFList+;
#pragma link C++ class IlcEveHFEditor+;
#pragma link C++ class IlcEveHFListEditor+;

// IlcEveLego
#pragma link C++ class IlcEveLego+;
#pragma link C++ class IlcEveLegoEditor+;

// IlcEveBeamInfo
// #pragma link C++ class IlcEveBeamsInfo+;
// #pragma link C++ class IlcEveBeamsInfoEditor+;
