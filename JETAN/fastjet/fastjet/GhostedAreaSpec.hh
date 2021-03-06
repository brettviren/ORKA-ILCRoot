//STARTHEADER
// $Id: GhostedAreaSpec.hh 1273 2008-08-11 18:22:55Z salam $
//
// Copyright (c) 2005-2006, Matteo Cacciari and Gavin Salam
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//ENDHEADER


#ifndef __FASTJET_GHOSTEDAREASPEC_HH__
#define __FASTJET_GHOSTEDAREASPEC_HH__

#include<vector>
#include<string>
#include "fastjet/PseudoJet.hh"
#include "fastjet/internal/BasicRandom.hh"

// 
#define STATIC_GENERATOR 1

namespace fastjet {      // defined in fastjet/internal/base.hh

/// namespace to hold default parameters for the active area spec
namespace gas {
  const double def_ghost_maxrap  = 6.0;
  const int    def_repeat        = 1;
  const double def_ghost_area    = 0.01;
  const double def_grid_scatter  = 1.0;
  const double def_kt_scatter    = 0.1;
  const double def_mean_ghost_kt = 1e-100;
}

//----------------------------------------------------------------------
/// Class that defines the parameters that go into the measurement
/// of active jet areas.
class GhostedAreaSpec {
public:
  /// default constructor
  GhostedAreaSpec(): _ghost_maxrap (gas::def_ghost_maxrap), 
		     _ghost_rap_offset(0.0),
                    _repeat       (gas::def_repeat), 
                    _ghost_area   (gas::def_ghost_area), 
                    _grid_scatter (gas::def_grid_scatter), 
                    _kt_scatter   (gas::def_kt_scatter), 
                    _mean_ghost_kt(gas::def_mean_ghost_kt),
                    _actual_ghost_area(-1.0) {_initialize();};
  
  /// explicit constructor
  explicit GhostedAreaSpec(double ghost_maxrap, 
                          int    repeat        = gas::def_repeat,
                          double ghost_area    = gas::def_ghost_area,   
                          double grid_scatter  = gas::def_grid_scatter, 
                          double kt_scatter    = gas::def_kt_scatter,   
                          double mean_ghost_kt = gas::def_mean_ghost_kt
                          ): 
    _ghost_maxrap(ghost_maxrap), 
    _ghost_rap_offset(0.0),
    _repeat(repeat), 
    _ghost_area(ghost_area), 
    _grid_scatter(grid_scatter),  
    _kt_scatter(kt_scatter), 
    _mean_ghost_kt(mean_ghost_kt),
    _actual_ghost_area(-1.0) {_initialize();};

  /// explicit constructor
  explicit GhostedAreaSpec(double ghost_minrap, 
			   double ghost_maxrap, 
                           int    repeat        = gas::def_repeat,
                           double ghost_area    = gas::def_ghost_area,   
                           double grid_scatter  = gas::def_grid_scatter, 
                           double kt_scatter    = gas::def_kt_scatter,   
                           double mean_ghost_kt = gas::def_mean_ghost_kt
                          ): 
    _ghost_maxrap    (0.5*(ghost_maxrap - ghost_minrap)), 
    _ghost_rap_offset(0.5*(ghost_maxrap + ghost_minrap)),
    _repeat(repeat), 
    _ghost_area(ghost_area), 
    _grid_scatter(grid_scatter),  
    _kt_scatter(kt_scatter), 
    _mean_ghost_kt(mean_ghost_kt),
    _actual_ghost_area(-1.0) {_initialize();};


  /// does the initialization of actual ghost parameters
  void _initialize();

  // for accessing values set by the user
  inline double ghost_etamax() const {return _ghost_maxrap;};
  inline double ghost_maxrap() const {return _ghost_maxrap;};
  inline double ghost_area   () const {return _ghost_area   ;};
  inline double grid_scatter() const {return _grid_scatter;};
  inline double kt_scatter  () const {return _kt_scatter  ;};
  inline double mean_ghost_kt() const {return _mean_ghost_kt  ;};
  inline int    repeat      () const {return _repeat      ;};

  // for accessing values 
  inline double actual_ghost_area() const {return _actual_ghost_area;};
  inline int    n_ghosts()          const {return _n_ghosts;};

  // when explicitly modifying values, sometimes call the initializer
  inline void set_ghost_area   (double val) {_ghost_area    = val; _initialize();};
  inline void set_ghost_etamax(double val) {_ghost_maxrap = val; _initialize();};
  inline void set_ghost_maxrap(double val) {_ghost_maxrap = val; _initialize();};
  inline void set_grid_scatter(double val) {_grid_scatter   = val; };
  inline void set_kt_scatter  (double val) {_kt_scatter     = val; };
  inline void set_mean_ghost_kt(double val){_mean_ghost_kt  = val; };
  inline void set_repeat      (int    val) {_repeat         = val; };

  /// return nphi (ghosts layed out (-nrap, 0..nphi-1), (-nrap+1,0..nphi-1),
  /// ... (nrap,0..nphi-1)
  inline int nphi() const {return _nphi;}
  inline int nrap() const {return _nrap;}

  /// get all relevant information about the status of the 
  /// random number generator, so that it can be reset subsequently
  /// with set_random_status.
  inline void get_random_status(std::vector<int> & __iseed) const {
    _random_generator.get_status(__iseed);}

  /// set the status of the random number generator, as obtained
  /// previously with get_random_status. Note that the random
  /// generator is a static member of the class, i.e. common to all
  /// instances of the class --- so if you modify the random for this
  /// instance, you modify it for all instances.
  inline void set_random_status(const std::vector<int> & __iseed) {
    _random_generator.set_status(__iseed);}
  
  inline void checkpoint_random() {get_random_status(_random_checkpoint);}
  inline void restore_checkpoint_random() {set_random_status(_random_checkpoint);}

  /// for a summary
  std::string description() const;

  /// push the ghost 4-momenta onto the back of the vector of PseudoJets
  void add_ghosts(std::vector<PseudoJet> & ) const;

  /// very deprecated public access to a random number 
  /// from the internal generator
  inline double random_at_own_risk() const {return _our_rand();};
  /// very deprecated public access to the generator itself
  inline BasicRandom<double> & generator_at_own_risk() const {
    return _random_generator;}

private:
  
  // quantities that determine nature and distribution of ghosts
  double _ghost_maxrap;
  double _ghost_rap_offset;
  int    _repeat      ;
  double _ghost_area   ;  
  double _grid_scatter;
  double _kt_scatter  ;
  double _mean_ghost_kt;

  // derived quantities
  double _actual_ghost_area, _dphi, _drap;
  int    _n_ghosts, _nphi, _nrap;


  std::vector<int> _random_checkpoint;
  static BasicRandom<double> _random_generator;
  //mutable BasicRandom<double> _random_generator;

  inline double _our_rand() const {return _random_generator();};
  
};

////----------------------------------------------------------------------
//class 1GhostPassiveAreaSpec : public GhostedAreaSpec {
//public:
//}

} // fastjet namespace 

#endif // __FASTJET_GHOSTEDAREASPEC_HH__
