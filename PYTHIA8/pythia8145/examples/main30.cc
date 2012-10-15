// main30.cc is a part of the PYTHIA event generator.
// Copyright (C) 2010 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Kaluza-Klein gamma*/Z resonances in TeV-sized extra dimensions.

#include <assert.h>
#include <time.h>
#include <sstream>

#include "Pythia.h"
using namespace Pythia8; 

int main() {

  // Generator.
  Pythia pythia;
  ParticleData& pdt = pythia.particleData;

  // Pick new random number seed for each run, based on clock.
  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = 0");
 
  // Process selection.
  pythia.readString("ExtraDimensionsTEV:ffbar2ffbar = on");

  // Pick KK mass.  
  double newMass = 4000.; // GeV
  cout << "|-------------------------" << endl;
  cout << "| KK mass is: " << newMass << endl;
  cout << "|-------------------------" << endl;
  stringstream strm;
  string sNewMass, sNewWidth, sNewLowBound, sNewHighBound;

  // Manually set the mass and therefore the width 
  // and the phase space for the sampling
  strm.clear();
  strm << newMass;
  strm >> sNewMass;
  strm.clear();
  strm << newMass / pdt.m0(5000023) * pdt.mWidth(5000023);
  strm >> sNewWidth;
  strm.clear();
  strm << newMass/4.;
  strm >> sNewLowBound;
  strm.clear();
  strm << newMass*2.;
  strm >> sNewHighBound;

  // Feed in KK state information and other generation specifics.  
  pythia.readString("5000023:m0 = " + sNewMass);
  pythia.readString("5000023:mWidth = " + sNewWidth);
  pythia.readString("5000023:mMin = " + sNewLowBound);
  pythia.readString("5000023:mMax = " + sNewHighBound);
  pythia.readString("5000023:isResonance = false");
  pythia.readString("ExtraDimensionsTEV:gmZmode = 3"); // 0=(gm+Z), 1=(gm), 2=(Z), 3=(gm+Z+gmKK+ZKK), 4=(m+Z+gmKK), 5=(m+Z+ZKK)
  pythia.readString("ExtraDimensionsTEV:nMax = 100"); // min=0, max=100, default=10
  pythia.readString("ExtraDimensionsTEV:mStar = " + sNewMass);
  pythia.readString("PhaseSpace:mHatMin = " + sNewLowBound);
  pythia.readString("PhaseSpace:mHatMax = " + sNewHighBound);

  // Initialize.
  pythia.init( 2212, 2212, 14000.);

  // Histograms.
  Hist mHatHisto("dN/dmHat", 50, newMass/4., newMass*2.);
  Hist pTmuHisto("(dN/dpT)_mu^-", 50, 1., 2501.);

  vector<int> moms;
  
  // Measure the cpu runtime,
  clock_t start, stop;
  double t = 0.0;
  // Depending on operating system, either of lines below gives warning.
  assert((start = clock()) != -1); // Start timer; clock_t signed.
  //assert((start = clock()) != -1u); // Start timer; clock_t unsigned.
  
  // Begin event loop. Generate event. Skip if error. List first one.
  for (int iEvent=0 ; iEvent<500 ; ++iEvent) {
    if (!pythia.next()) continue;
    if (iEvent < 1) {pythia.info.list(); pythia.event.list();}

    // Begin event analysis.
    bool isZ = false;
    bool ismu = false;
    int iZ = 0;
    int imu = 0;
    for (int i=0 ; i<pythia.event.size() ; ++i) {
      // find the most recent Z
      if (pythia.event[i].id()==5000023) {
        iZ = i;
        isZ = true;
      }
      // find the final muon who's first mother is the Z
      if (pythia.event[i].id()==13 && pythia.event[i].isFinal()) {
        moms.clear();
        moms = pythia.event.motherList(i);
        for (int m=0 ; m<(int)moms.size() ; m++) {
          if( pythia.event[ moms[m] ].id()==5000023 ) {
            imu = i;
            ismu = true;
            break;
          } // end if 5000023
        } // end for moms.size()
      } // end if final muon
    } // end for event.size()
    if(isZ && ismu) {
      mHatHisto.fill( pythia.event[iZ].m() );
      pTmuHisto.fill( pythia.event[imu].pT() );
    }
    if(iEvent%10==0) cout << "Event: " << iEvent << endl << flush;
  } // end for iEvent<500

  // Done. Print results.
  stop = clock(); // Stop timer
  t = (double) (stop-start)/CLOCKS_PER_SEC;

  pythia.statistics();
  cout << mHatHisto;
  cout << pTmuHisto;

  cout << "\n" << "|----------------------------------------|" << endl;
  cout << "| CPU Runtime = " << t << " sec" << endl;
  cout << "|----------------------------------------|" << "\n" << endl;
  
  return 0;
}
