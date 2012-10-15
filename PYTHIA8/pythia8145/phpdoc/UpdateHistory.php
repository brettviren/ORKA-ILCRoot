<html>
<head>
<title>Update History</title>
<link rel="stylesheet" type="text/css" href="pythia.css"/>
<link rel="shortcut icon" href="pythia32.gif"/>
</head>
<body>

<script language=javascript type=text/javascript>
function stopRKey(evt) {
var evt = (evt) ? evt : ((event) ? event : null);
var node = (evt.target) ? evt.target :((evt.srcElement) ? evt.srcElement : null);
if ((evt.keyCode == 13) && (node.type=="text"))
{return false;}
}

document.onkeypress = stopRKey;
</script>
<?php
if($_POST['saved'] == 1) {
if($_POST['filepath'] != "files/") {
echo "<font color='red'>SETTINGS SAVED TO FILE</font><br/><br/>"; }
else {
echo "<font color='red'>NO FILE SELECTED YET.. PLEASE DO SO </font><a href='SaveSettings.php'>HERE</a><br/><br/>"; }
}
?>

<form method='post' action='UpdateHistory.php'>

<h2>Update History</h2>

These update notes describe major updates relative to the baseline 
PYTHIA 8.100 version. However, they are less extensive than the 
corresponding update notes for PYTHIA 6. There are three main 
reasons for this:

<ul>

<li>The manual contained on these html/php pages is kept up to date.
(However, the "Brief Introduction" may not always be.)</li>

<li>8.1 is a quite new code, so there are many minor changes that, 
if all were to be documented, would hide the key ones.</li>

<li>8.1 is not yet used for "mission critical" applications, 
so there is less need to trace changed behaviour.</li>

</ul>

<h3>Main news by version</h3>

<ul>

<li>8.105: 24 February 2008
<ul>

<li>New option to initialize with arbitrary beam directions<br/>
<code>pythia.init( idA, idB, pxA, pyA, pzA, pxB, pyB, pzB)</code></li>

<li>Added capability to set <?php $filepath = $_GET["filepath"];
echo "<a href='BeamParameters.php?filepath=".$filepath."' target='page'>";?>beam energy spread 
and beam vertex</a>.
<br/>
<b>Warning:</b> as a consequence, the old <code>Beams</code> group of 
settings has been moved to <code>BeamRemnants</code>, and 
<code>Beams</code> is now instead used for machine beam parameters. 
Therefore also some <code>Main</code> settings of this character have been 
regrouped, as follows:
<table cellspacing="5">
<tr> <td>8.100 setting  </td> <td>has been moved to </td> </tr>
<tr> <td><code>Main:idA          </code></td>
     <td><code>Beams:idA         </code></td> </tr>
<tr> <td><code>Main:idB          </code></td>
     <td><code>Beams:idB         </code></td> </tr>
<tr> <td><code>Main:inCMframe    </code></td>
     <td>(<code>Beams:frameType</code>) </td> </tr>
<tr> <td><code>Main:eCM          </code></td>
     <td><code>Beams:eCM         </code></td> </tr>
<tr> <td><code>Main:eA           </code></td>
     <td><code>Beams:eA          </code></td> </tr>
<tr> <td><code>Main:eB           </code></td>
     <td><code>Beams:eB          </code></td> </tr>
<tr> <td><code>Main:LHEF         </code></td>
     <td><code>Beams:LHEF        </code></td> </tr>
</table></li>

<li>The <code>forceHadronLevel()</code> method introduced for standalone 
hadronization.</li>

<li><code>main15.cc</code> illustrated how either full hadronization or 
only decays of some particles can be looped over for the rest of the 
event retained.</li>

<li>The <code>LHAevnt</code> and <code>LHAinit</code> classes have been 
joined into a new <code>LHAup</code> one, with new options that allow 
the writing of a Les Houches Event File.</li>

<li>The <code>SusyLesHouches</code> class updated to handle 
SLHA version 2.</li>

<li>Updated HepMC conversion routine.</li>

<li>The static <code>ErrorMsg</code> class has been removed and 
its functionality moved into the non-static <code>Info</code> class,
in the renamed Info file.</li>

<li>Further reduction of the use of static, with related code changes.
This should allow to have several almost independent <code>Pythia</code> 
instances. Some static classes still remain, however, notably for
random number generation and particle properties.</li>

<li>Limited capability to use two different <code>Pythia</code> instances 
for signal + pileup event generation, see <code>main19.cc</code> for an 
example.</li> 

<li>In the <code>Event</code> class the <code>=</code> and 
<code>=+</code> methods have been overloaded to allow the copying 
or appending of event records. Illustrated in <code>main19.cc</code>.</li>

<li>The html and php page formatting improved with 
cascading style sheets.</li>

<li>Several minor improvements and new options, including updated configure 
scripts.</li>

</ul>
</li>

<li>8.108: 1 May 2008
<ul>

<li>Correction in the event record, so that the beam particles in line 
1 and 2 do not have any mother according to the <code>motherList</code>
method. Previously the "system" entry in line 0 was counted as their 
mother, which gave rise to an unexpected extra vertex in the conversion 
to the HepMC format.</li>

<li>Support for HepMC version 1 is removed, to simplify the code and 
reflect the evolution of the field.</li>

<li>Status codes are stored in HepMC only as 1 for existing and 2 for
decayed or fragmented particles (whereas previously the original PYTHIA
codes were used for the latter).</li>

<li>Parton densities are stored in HepMC as <i>xf(x,Q^2)</i> 
rather than the <i>f(x,Q^2)</i> used in (some) previous versions.</li>

<li>The SusyLesHouches class has ben updated so that reading is fully
compatible with the SLHA2 standard. </li>

<li>Improved handling of the Higgs width, relevant for massive and thereby
broad resonance shapes.</li>

<li>The matrix elements for neutralino pair production have now been
completed and checked.</li>

<li>Ensure that <i>alpha_strong</i> does not blow up, by introducing 
a minimal scale somewhat above <i>Lambda_3</i> (roughly where
<i>alpha_strong = 10</i>).</li>

<li>New methods <code>isValence1()</code> and <code>isValence2()</code> 
in the <code>Info</code> class.</li>

<li>Protection against division by zero in calculation of decay vertex
(for zero-mass gluons with zero lifetime, where there should be no
displacement).</li>

<li>A new compilation option <code>-Wshadow</code> is introduced and 
code is rewritten at all places where this option gave warnings.</li>

<li>Minor library correction to allow compilation with gcc 4.3.0.</li>

</ul>
</li>

<li>8.114: 22 October 2008
<ul>

<li>New rescattering description operational (but still experimental) 
for the case that FSR is not interleaved, but saved until after MI, 
ISR and beam remnants have been handled. This involves much new code
in several classes.</li>

<li>A new class <code>PartonSystems</code> is introduced to 
keep track of which partons in the event record belong to which 
subcollision system, plus some further information on each subsystem.
It takes over functionality previously found as part of the 
<code>Event</code> class, but leaves room for future growth.</li>

<li>Add optional model, wherein an increased <i>pT0</i> turnoff scale 
for MI and ISR is used for above-average active events, i.e. events that 
already have several MI's or ISR emissions.</li>

<li>Freeze GRV 94L distribution at small <i>Q^2</i> to avoid blowup.</li>

<li>The <code>pythia.readFile(...)</code> method can now alternatively take 
an <code>istream</code> as argument instead of a <code>filename</code>.</li>

<li>Minor bug correction in <code>PartonLevel.cc</code>; the bug could 
(rarely) give a segmentation fault.</li>

</ul>
</li>

<li>8.120: 10 March 2009
<ul>

<li>New rescattering description further developed, but not yet
recommended for normal usage.</li>

<li>Include new processes for Large Extra Dimensions and Unparticles,
contributed by Stefan Ask. New test program <code>main28.cc</code> 
illustrates.</li>

<li>Include further SUSY processes: neutralino-chargino and 
chargino-chargino pairs. The processes should be valid also 
in the case of non-minimal flavour violation and/or CP violation.
Expanded machinery to keep track of SUSY parameters.</li>

<li>Include backwards evolution of incoming photon as part of the
<code>SpaceShower</code> initial-state radiation description. This
allows you to simulate hard collisions where one of the incoming
partons is a photon. New test program <code>main43.cc</code> 
illustrates.</li>

<li>Allow separate mass and transverse momentum cuts when two hard 
subprocesses are generated in the same event.</li>

<li>The default value for the border between short- and long-lived
paticles has been changed from 1 mm to 10 mm, to better conform with 
LHC standards, see  <?php $filepath = $_GET["filepath"];
echo "<a href='ParticleDecays.php?filepath=".$filepath."' target='page'>";?>here</a>.
The default is still to let all unstable particles decay.</li>

<li>New ISR matrix-element correction to <i>f -&gt; f gamma</i> 
in single <i>W</i> production.</li>

<li>New method <code>Event::statusHepMC</code> returns the status 
code according to the HepMC conventions agreed in February 2009.
The interface to HepMC now writes out status according to this 
convention.</li>

<li>Add capability to link to FastJet, with expanded configure script 
and Makefile, and with <code>main61.cc</code> as new example.</li>

<li>Update of <code>Makefile.msc</code>, with added support for latest 
Visual C++ Express edition and use of regexp to check nmake version.</li>

<li>Update of <code>LHAFortran.h</code> and 
<code>Pythia6Interface.h</code>, to make the interface to Fortran 
routines work also under Windows. (Thanks to Anton Karneyeu.)

<li>Updated and expanded worksheet.</li>

<li>The manual pages in the <code>xmldoc</code> directory, and thereby
also those of the <code>htmldoc</code> and <code>phpdoc</code>
directories, have been significantly updated and expanded. In particular, 
in many places the class of each method is explicitly shown, as well as 
the type of the return value and of the arguments. This upgrade is not 
yet completed, but already covers the more relevant sections. </li>

<li>The unary minus operator in the <code>Vec4()</code> returns a 
reference to a four-vector with all components negated, but leaves 
the original four-vector unchanged. Previously the four-vector itself
was flipped.</li>

<li>The <code>pPlus()</code> and <code>pMinus()</code> methods of a
four-vector and an event-record particle are renamed <code>pPos()</code> 
and <code>pNeg()</code>, respectively.</li>

<li>Include a further loop in <code>ProcessLevel</code>, so that a new
process is generated in case of failures of a less severe nature.</li>

<li>Introduce warning message for unexpected <code>meMode</code> in
<code>ResonanceWidths</code>.</li>

<li>Les Houches event reading framework has been rearranged for
more flexibility. Some bugs corrected. Specifically, when scale 
is not set (<code>scale = -1.</code> in the Les Houches standard),
PYTHIA did not attempt to set this scale itself, which typically 
lead to there not being any ISR or FSR. Now the 
<?php $filepath = $_GET["filepath"];
echo "<a href='CouplingsAndScales.php?filepath=".$filepath."' target='page'>";?>rules for normal 
1-, 2- and 3-body final states</a> are applied, with a trivial
extension of the 3-body rules for higher multiplicities.</li>

<li>Correct bug in the handling of parton densities, whereby it was
not possible to switch to a new set, once a first initialization
had been done.</li>

<li>Correct bugs when several <code>Pythia::init</code> initialization
calls are made in the same run, specifically in the case that pointers
to external processes have been handed in.

<li>Changes in <code>main03.cmnd</code> and <code>main04.cmnd</code> 
so that some nonstandard options are commented ou rather than active. 
Related comments inserted also in some other <code>.cmnd</code> files, 
but there without any change in program execution.</li>

<li>A few further minor bug fixes.</li>

<li>Update year to 2009.</li>

</ul>
</li>

<li>8.125: 16 June 2009
<ul>

<li>Hadronization and timlike-shower parameter default values updated 
according to a tune to LEP1 data by Hendrik Hoeth, using the 
Rivet + Professor framework.</li>

<li>Many further SUSY production processes. SLHA readin expanded to cover 
also masses and decay modes. Example <code>main22.cc</code> updated, while 
<code>main33.cc</code> now superfluous and removed. </li>

<li>Also further processes for extra dimensions.</li>

<li>Stefan Ask joins as coauthor.</li>

<li>The <code>TimeShower::shower(...)</code> method has gained 
a new argument, that forces the shower evolution to stop after
a given number of branchings. A new method 
<code>TimeShower::pTLastInShower()</code> permits the last evolution
<i>pT</i> to be read out. These extensions can be useful 
for matching studies.
</li>

<li>New optional argument <code>isInterleaved</code> added to 
<code>TimeShower::branch(...)</code>. Is false by default, but
true when called from the parton level for interleaved evolution
of several parton systems, optionally also with ISR and MI.</li>

<li>New methods <code>UserHooks::canSetResonanceScale()</code>
and <code>UserHooks:scaleResonance(...)</code> allows an optional 
user choice of the maximum shower scale in resonance decays.</li>

<li>A new method <code>SpaceShower::doRestart()</code> has been 
added, to help distinguish cases when a
<code>SpaceShower::branch(...)</code> failure forces a complete
restart of the evolution from ones where only the intended
current branching has been vetoed.

<li>When multiple interactions are initialized, it is now 
possible to reduce both <i>pT0</i> and <i>pTmin</i>
if necessary to find a valid solution where 
<i>sigma_jet &gt; sigma_nondiffractive</i>. Previously
only the former would be reduced, which could lead to 
infinite loops if too large a <i>pTmin</i> was used.
Thanks to Sami Kama for pointing out the problem.
</li>

<li>The rescattering machinery is now essentially completed, and can
be used also by others than the authors. For now, however, it can only
be recommended for dedicated studies, not e.g. for generic tunes.
</li>

<li>Timelike and spacelike showers, and beam remnant handling, 
are modified to handle rescattering partons. Specifically, a new machinery 
is introduced to trace the recoils from the combination of rescattering 
with showers and primordial <i>kT</i>. Can assign space- or timelike 
virtualities to intermediate particles to have energy and momentum 
conserved locally. This affects the <code>PartonLevel</code>, 
<code>MultipleInteractions</code>, <code>TimeShower</code>, 
<code>SpaceShowe</code>r and <code>BeamRemnants</code> classes. 
Further details to appear in the upcoming article on rescattering. 
(Another change is a reversal to pre-8.114 order for non-interleaved FSR,
wherein also FSR is treated before beam remnants are attached.)
</li>

<li>Four new status codes introduced, as part of the rescattering 
description:
<br/>45 : incoming rescattered parton, with changed kinematics owing
to ISR in the mother system (cf. status 34);
<br/>46 : incoming copy of recoiler when this is a rescattered parton
(cf. status 42);
<br/>54 : copy of a recoiler, when in the initial state of a different 
system from the radiator;
<br/>55 : copy of a recoiler, when in the final state of a different 
system from the radiator.
</li>

<li>New method <code>Info::tooLowPTmin()</code> can tell whether the 
<i>pTmin</i> scales for showers or multiple interactions are too low. 
</li>

<li>Pion beams allowed, both <i>pi^+</i>, <i>pi^-</i> and 
<i>pi^0</i>. New machinery for the latter, where the valence 
flavour content is chosen to be either <i>d dbar</i> or 
<i>u ubar</i> for each new event. One internal pion PDF 
implemented, with others from LHAPDF.</li>

<li>Treatment of Pomeron-proton collisions begun.</li>

<li>Phase-space handling of hard processes and multiple interactions
slightly expanded to better allow for harder PDF's than proton ones,
e.g. for Pomerons.</li>

<li>The program documentation has been expanded with an
alphabetical index of all methods that are described on the 
webpages, see the <?php $filepath = $_GET["filepath"];
echo "<a href='ProgramMethods.php?filepath=".$filepath."' target='page'>";?>Program Methods</a> 
page. Also other sections of the documentation have been
updated and expanded, including the worksheet.</li>

<li>Several <code>list</code> methods have been made 
<code>const</code>. For the listing of events two new methods
have been added, <code>Event::list()</code> and 
<code>Event::list(bool showScaleAndVertex, 
bool showMothersAndDaughters = false)</code>,
that correspond to special cases of the general method.</li>

<li>A new method <code>Pythia::LHAeventSkip(int nSkip)</code>
permits a skip-ahead of the reading of external Les Houches 
Events, without the necessity to generate the intervening 
<code>nSkip</code> events in full. Makes use of the new 
<code>LHAup::skipEvent(int nSkip)</code> method to perform 
the operations. Mainly intended for debug purposes.

<li>The <code>ClusterJet</code> jet finder now saves the last
five clustering scales. Also a minor bug fix. Thanks to Nils 
Lavesson for this contribution.</li>

<li>The <code>Particle::m2()</code> method now returns a negative 
number when the stored mass <i>m</i> is negative, as used to 
indicate spacelike virtualities. Also the 
<code>Particle::eCalc()</code>, <code>Particle::mT()</code> and
<code>Particle::mT2()</code> methods have been modified.
</li>

<li>The <code>&lt;&lt;</code> method to print our a four-vector has 
been expanded with a fifth number, the invariant length, with a minus
sign for spacelike vectors, and provided within brackets to allow
a simple visual distinction.</li>

<li>New methods <code>Rndm::dumpState(string fileName)</code> and
<code>Rndm::readState(string fileName)</code> allows to write or
read the state of the random number generator to or from a binary file.
</li>

<li>New method <code>double GammaReal(double x)</code> returns the 
value of the <i>Gamma</i> function for arbitrary real argument.
Some cross sections for extra-dimensional processes rewritten to
make use of it.
</li>

<li>New example program <code>main29.cc</code> shows how 
to set up a fictitious process of a heavy system decaying
to two particles or partons, with decays traced to stable
particles, as relevant for astroparticle applications.</li>

<li>Main programs that illustrate the HepMC interface have
been updated to use version 2.04, including units and excluding
deprecated output formats.</li>

<li>The <code>main32.cc</code> example extended also to handle 
Les Houches Event Files.</li>

<li>The Makefile has been modified so that "make clean" only
removes the current compilation and library fles, while 
"make distclean" gives a more extensive reset and cleanup.
Thanks to Nils Lavesson for this contribution. Some other
minor Makefile corrections.</li>

<li>Several main programs that use the 
<code>Main:timesToShow</code> mode have been corrected so as 
not to crash if this is set to zero. Also some other cosmetics
changes in main programs that do not affect the running.</li>

<li>Bug correction, in that previously a veto with user hooks 
was not propagated from parton showers inside resonance decays.
</li>

<li>Minor bug fix in <code>TimeShower</code> for kinematics with 
unequal beam-particle masses.</li>

<li>Bug fix so that <code>PartonLevel:MI = off</code> also works for 
minimum-bias events.</li>

<li>Minor bug fix in the impact-parameter selection of multiple 
interactions. Thanks to Sami Kama for pointing it out.</li>

<li>String fragmentation for junction topology protected against 
numerical instability in boost.</li>

<li>Bug correction in the handling of particle decays to partons,
where the scale of the partons was set before the partons had been
added to some arrays, leading to indexation out of bounds. 
Thanks to Vladislav Burylov for discovering this bug.
</li>

<li>Bug correction in the handling of particles with inhibited decay, 
where the decay vertex would be too far displaced, which could lead to
infinite loop. Thanks to Sami Kama for debugging this. </li>

<li>Check to avoid infinite loop in matrix-element handling of 
two-body decays.</li>

<li>Bug correction to avoid infinite loops in Dalitz decay treatment.
Some changes in the decay handling logic to allow a new try when the 
decay of a particle fails.</li>

<li>Minor correction, so the pointer to the <code>Info</code> class
is set also for user-written classes derived from <code>LHAup</code>. 
</li>

<li>Correction for typo in the matrix element of the
<code>Sigma3ff2HchgchgfftWW</code> class, for doubly charged Higgs
production. Thanks to Merlin Kole for spotting it.</li>

<li>Updated colour bookkeeping in junction-antijunction annihilation 
avoids later problems in <code>Pythia::check()</code>. </li>

<li>Minor updates of the <code>Makefile.msc</code> file to work with 
Visual Studio 2008. Thanks to David Bailey for these modifications.</li>

<li>Ensure that <code>nInit</code> in the <code>BeamParticle</code>
class is set also for unresolved lepton beams.</li>

<li>The <code>VetoEvolution</code> class, derived from 
<code>UserHooks</code>, is obsolete and has been removed.</li>

</ul>
</li>

<li>8.130: 15 September 2009
<ul>

<li>New machinery that allows multiple interactions inside diffractive 
systems. Also new optional Pomeron flux factors and Pomeron PDFs.
New page on <?php $filepath = $_GET["filepath"];
echo "<a href='Diffraction.php?filepath=".$filepath."' target='page'>";?>diffraction</a> added, where
further details are collected. Still not tuned, so to be used with 
caution.</li>

<li>Make Peter Skands' "Tune 1" parameters for ISR and MI default.
The older simpler tune is still available as an option, see
<code><?php $filepath = $_GET["filepath"];
echo "<a href='Tunes.php?filepath=".$filepath."' target='page'>";?>Tune:pp</a></code>. 
</li>

<li>New possible choices for a second hard process: charmonium,
bottomonium, top pair and single top.</li>

<li>New code for pair production of generic colour-triplet scalar, 
fermion or vector. Largely written by Johan Bijnens, partly recycling 
existing code.</li>

<li>Add user hooks possibility to veto event after a given number
of multiple interactions.</li>

<li>Add instructions how PYTHIA 8 can be used from inside ROOT.
Thanks to Andreas Morsch for providing the text and Rene Brun 
for a simple example.</li>

<li>The <code>main21.cc</code> example extended with an option
for a single-particle gun.</li>

<li>Improvements and bug fixes in rescattering framework.</li>

<li>New method <code>Hist::table(string fileName)</code> provides a
more direct way to print a two-column table of histogram contents
into a file than the current <code>Hist::table(ostream& os = cout)</code>.
 </li>

<li>Modify reading of external files so that a line only consisting of
control characters counts as empty, the same way as a line only consisting
of blanks already did. This includes carriage return, tabs and a few more, 
the ones represented by <code>\n \t \v \b \r \f \a</code>. Applies to 
Les Houches Event files, settings files and particle data files. </li>

<li>Fix it so that the read-in of a Les Houches Event File for the 
hardest process can be combined with the faciliy to select a specified
second hard process.</li>

<li>New empty base class method <code>LHAup::fileFound()</code> 
allows the derived class <code>LHAupLHEF</code> to signal more
clearly that a failed initialization is caused by a failure to open
the desired file. </li>

<li>Check that a pointer to an <code>LHAup</code> object has been set
in <code>Pythia::LHAeventList()</code> and
<code>Pythia::LHAeventSkip(...)</code> calls.</li>

<li>Updated configure and Makefile, e.g. to build shared libraries 
on Mac OS X.</li>

<li>The options with a direct link to hard-process generation
in PYTHIA 6 has been removed. It is hardly ever used but 
complicates the build structure. (Owing to its usefulness for some 
debug work, it was reinstated in a limited form in version 8.135.
Thus <code>main51.cc</code> now contains the complete interface, 
previously in separate files, and commented-out lines in 
<code>examples/Makefile</code> suggest how PYTHIA 6 could be linked.) 
</li>

<li>New argument to <code>Info::errorMsg(...)</code> allows to 
show all error messages of a specific kind rather than only the 
first one, e.g. for initialization.</li>

<li>Correction in decay table of righthanded Majorana neutrinos.
Thanks to Arnaud Ferrari and Vladimir Savinov.</li>

<li>Correction in expressions in the manual for <i>H^+-</i> couplings 
to an <i>h^0</i>, <i>H^0</i> or <i>A^0</i> and a <i>W^+-</i>.
Thanks to Rikard Enberg.</li>

<li>Fix for accessing uninitialized memory, caused by accessing the 
daughters of the incoming beams before these daughters actually existed.
Thanks to David Bailey and Sami Kama.
</li>

<li>Uninitialized photon PDF inside the proton could give crazy results
for processes with incoming photons. Thanks to Adam Davison.</li>

<li>Bug fix such that finite lifetimes can be set also for particles
produced in the hard process. To exemplify, the bug affected 
<i>tau</i> leptons produced in <i>Z^0</i> and <i>W^+-</i> 
decays. Thanks to Troels Petersen.</li>

<li>Bug fix in <code>TimeShower</code>, that <code>beamOffset</code>
could remain uninitialized. Thanks to Sami Kama.</li>

<li>Minor correction to resonance decays: fail if the allowed mass
range of a Breit-Wigner resonance is a small fraction of the total
area under the resonance curve.</li>

<li>Correction when all three valence quarks were kicked out from a 
proton and could give false messages that beam momentum had been 
used up.</li>

<li>Clarify status codes needed for hadron-level standalone runs in
order to avoid error messages, and modify the <code>main21.cc</code>
example accordingly. </li>

<li>Minor corrections in the processes for extra dimensions.</li>

<li>Some other minor additions to existing facilities
and minor bug fixes.</li>

</ul>
</li>

<li>8.135: 10 January 2010
<ul>

<li>All usage of static member methods inside Pythia8 has now been
eliminated. This simplifies for you to have several simultaneous 
<code>Pythia</code> instances that are run with different conditions.
The three main classes affected by this are the <code>Settings</code> 
and <code>ParticleData</code> databases and the <code>Rndm</code> 
random-number generator. You can no longer address the methods of these
classes directly, but have to address them via the <code>settings</code>,
<code>particleData</code> and <code>rndm</code> instances in the 
respective <code>Pythia</code> object. Also some other smaller pieces
of code are affected, e.g. Standard Model and SUSY couplings 
(the latter in new files). 
<br/><b>Note 1</b>: The documentation has been updated accordingly
on these webpages, but the "A Brief Introduction to PYTHIA 8.1"
still refers to the old behaviour of version 8.100.
<br/><b>Note 2</b>: the interface to the external LHAPDF library remains
static, since LHAPDF is written in Fortran and thus by definition
static.
<br/><b>Note 3:</b> if you want to have momentum smearing in 
<code>CellJet</code> you now need to send in a pointer to a 
random-number generator.  
</li>

<li>Ten new proton PDF sets are made available internally:
MRST LO* (2007), MRST LO** (2008), MSTW 2008 LO (central member),
MSTW 2008 NLO (central member), CTEQ6L, CTEQ6L1, CTEQ6.6 (NLO,
central member), CT09MC1, CT09MC2, and CT09MCS, see 
<?php $filepath = $_GET["filepath"];
echo "<a href='PDFSelection.php?filepath=".$filepath."' target='page'>";?>PDF Selection</a>. The Pomeron PDF data 
files have been renamed for consistency. Thanks to Tomas Kasemets 
for help with this [<a href="Bibliography.php" target="page">Kas10</a>].
</li>

<li>New parameters <code>TimeShower:pTmaxFudgeMI</code> and 
<code>SpaceShower:pTmaxFudgeMI</code> introduced, to give the same
functionality for multiple interactions that 
<code>TimeShower:pTmaxFudge</code> and <code>SpaceShower:pTmaxFudge</code>  
do for the hardest.</li>

<li>A few extensions of the <code>UserHooks</code> framework.
New methods <code>UserHooks:canVetoISREmission()</code> and
<code>UserHooks::doVetoISREmission(...)</code> allows the latest
initial-state emission to be studied before being finalized, 
with the possibility to veto it. 
Similarly <code>Userhooks:canVetoFSREmission()</code> and
<code>UserHooks::doVetoFSREmission(...)</code> can be used to
veto the latest final-state emission.</li>

<li>A number of loop counters have been introduced in the 
<code>Info</code> class, that offers some further information on
the progress of the event generation, for use e.g. in conjunction
with the <code>UserHooks</code> facility.</li>

<li>The <code>Pythia::initTunes(...)</code> method is made public, so that
it can be called before the normal call from <code>Pythia::init(...)</code>.
That way it is possible to start out from a given tune and change a few
of the parameters.
</li>

<li>Bug corrected in <code>LHAFortran.h</code> for hard-process input
from Fortran commonblock. This did not work properly when the input
was used in combination with a second hard process generated internally.
Thanks to Mikhail Kirsanov and Roberto Chierici.

<li>Insertion of missing initialization of <code>isInit</code> in the
<code>Settings</code> and <code>ParticleData</code> constructors, and 
<code>nInit</code> for <code>BeamParticle</code>.
Thanks to Leif L&ouml;nnblad.</li>

<li>Updated <code>Makefile.msc</code> for Windows users.</li>

<li>Fix of some non-optimal use of booleans, that give warnings on
a Windows compiler. Thanks to Anton Karneyeu.</li>

<li>New options for the <?php $filepath = $_GET["filepath"];
echo "<a href='SUSYLesHouchesAccord.php?filepath=".$filepath."' target='page'>";?>
SUSY Les Houches Accord</a> such that, by default,
particle and decay data are not overwritten for known Standard Model 
particles (including <i>Z^0</i>, <i>W^+-</i> and <i>t</i>,
but excluding the Higgs).</li>

<li>Bug fix in <code>SusyLesHouches</code>, where the reading of SLHA
information embedded in an LHEF would not stop at the end of the header
section.</li> 
 
<li>Bug correction for undefined secondary widths where decay products 
together are heavier than the mother. Also unit default secondary width 
values in the <code>DecayChannel</code> constructor.</li>
 
<li>Documentation updated, including change of current year to 2010.</li>

</ul>
</li>

<li>8.140: 16 July 2010
<ul>

<li>Four new draft <?php $filepath = $_GET["filepath"];
echo "<a href='Tunes.php?filepath=".$filepath."' target='page'>";?>tunes</a> available.</li>

<li>Introduction of a new scenario for production of Hidden-Valley
particles, and interleaved showering in the QCD and HV sectors, see
the new <?php $filepath = $_GET["filepath"];
echo "<a href='HiddenValleyProcesses.php?filepath=".$filepath."' target='page'>";?>Hidden Valleys</a> 
description. A longer physics writeup is available [<a href="Bibliography.php" target="page">Car10</a>].
</li>

<li>Implementation of <i>2 -> 3</i> phase space selection intended 
for QCD processes with massless partons. A new set of matching 
<?php $filepath = $_GET["filepath"];
echo "<a href='PhaseSpaceCuts.php?filepath=".$filepath."' target='page'>";?>phase space cut parameters</a>. 
The <?php $filepath = $_GET["filepath"];
echo "<a href='QCDProcesses.php?filepath=".$filepath."' target='page'>";?>ten different QCD <i>2 -> 3</i> 
processes</a> have been implemented making use of this new 
possibility, so far without a complete handling of possible colour 
flows, however.</li> 

<li>New processes have been added for 
<?php $filepath = $_GET["filepath"];
echo "<a href='CompositenessProcesses.php?filepath=".$filepath."' target='page'>";?>contact interactions</a> in 
<i>q q -> q q</i> and <i>q qbar -> q qbar</i> scattering.</li>

<li>A process has been added for TeV^-1 Sized 
<?php $filepath = $_GET["filepath"];
echo "<a href='ExtraDimensionalProcesses.php?filepath=".$filepath."' target='page'>";?>Extra Dimensions</a>, which 
involves the electroweak KK gauge bosons, i.e. <i>gamma_{KK}/Z_{KK}</i>,
in one TeV^-1 sized extra dimension; see <code>main30.cc</code> for 
an example. This scenario is described in [<a href="Bibliography.php" target="page">Bel10</a>]. Thanks to 
Noam Hod and coworkers for contributing this code.</li>

<li>In the Randall-Sundrum extra-dimensional scenario a new option has 
been added where SM fields can exist in the bulk rather than only on
a brane. (Still under development.) Furthermore production of a 
Kaluza-Klein gluon state has been added, and the <code> main28.cc</code> 
test program extended.</li> 

<li>The scenario for monojets in Large Extra Dimensions has been expanded
with an alternative for scalar graviton exchange instead of tensor one.
</li> 

<li>New parameters for maximum scale of 
<?php $filepath = $_GET["filepath"];
echo "<a href='TimelikeShowers.php?filepath=".$filepath."' target='page'>";?>timelike showers</a>,
<code>TimeShower:pTmaxMatch</code>, and the dampening of hard radiation,
<code>TimeShower:pTdampMatch</code> and <code>TimeShower:pTdampFudge</code>,
by analogy with corresponding ones for spacelike showers. Also new method
<code>TimeShower:limitPTmax(...)</code> to implement alternative procedures.
For dipoles stretched to the beam the new switch 
<code>TimeShower:dampenBeamRecoil</code> allows to dampen radiation
close to the beam direction, with a changed default behaviour.</li>  

<li>Azimuthal anisotropies from coherence arguments have been introduced 
for the spacelike parton showers, see <code>SpaceShower:phiIntAsym</code>
and <code>SpaceShower:strengthIntAsym</code>.
Also azimuthal anisotropies from gluon polarization have been introduced 
for the spacelike parton showers, see <code>SpaceShower:phiPolAsym</code>,
and updated for timelike parton showers, see 
<code>TimeShower:phiPolAsym</code>.</li>

<li>Improvements for the matching to POWHEG LHEF-style input illustrated
by the new <code>main71.cc</code> example. See also [<a href="Bibliography.php" target="page">Cor10</a>].</li>

<li>A set of new processes <i>gamma gamma -> f fbar</i>, with <i>f</i>
quarks or leptons. Code for equivalent photon flux around an unresolved 
proton, with more to come. Thanks to Oystein Alvestad.
</li>

<li>A new option has been included to dampen the growth of the diffractive
cross sections, see <?php $filepath = $_GET["filepath"];
echo "<a href='TotalCrossSections.php?filepath=".$filepath."' target='page'>";?>Total Cross 
Sections</a>.</li>

<li>A new method <code>virtual int SigmaProcess::idSChannel()</code>
has been introduced. If overloaded to return a nonzero value then a 
<i>2 -> n</i> process will appear listed as a <i>2 -> 1 -> n</i> one.
That is, an intermediate resonance with the requested identity will be 
inserted in the event record, even without appearing in the calculation 
of the matrix element proper. Thanks to Noam Hod for idea and code.</li> 

<li>A new method <code>SigmaProcess::convertM2()</code> has been introduced
to optionally allow the <code>SigmaProcess::sigmaHat()</code> to return
the squared matrix element rather than <i>d(sigmaHat)/d(tHat)</i> for
<i> 2 -> 2 </i> processes. Furthermore kinematics is stored in the new
<code>mME</code> and <code>pME</code> vectors for alternative cross
section encodings.</li> 

<li>Different encoding of the <i>f fbar -> Z W</i> cross section,
contributed by Merlin Kole, based on the cross section of
Brown, Sahdev and Mikaelian. Gives a distinctly different cross section
than the previous based on EHLQ (including their bug fix).
In particular the problem with negative cross sections is now fixed.</li>

<li>Minor updates: pass <code>xmlPath</code> to new MSTW and CTEQ PDFs;
reset beams earlier to give cleaner documentation.</li>

<li>The H1 Fit B LO parametrization to the Pomeron PDF has been included, 
and made new default for Pomerons. Thanks to Paul Newman for providing 
the data files.</li>

<li>Three changes, in principle unrelated, but with the common objective
to make the generation of a given event depend only on the values
determined during the initialization stage and on the state of the 
random-number generator when the event is begun. The new default 
should ensure a reproducible stop-and-restart behaviour, convenient 
for debug purposes. Thanks to Michael Schmelling for stressing the 
desire for such a behaviour.
<br/>(i) Introduction of a new option <code><?php $filepath = $_GET["filepath"];
echo "<a href='PhaseSpaceCuts.php?filepath=".$filepath."' target='page'>";?>
PhaseSpace:increaseMaximum</a></code> that allows to switch between
two strategies for handling the (hopefully rare) cases when the 
assumed maximum of the cross section function is exceeded during the
event generation, with a changed default behaviour. In the old 
default the maximum could be increased if it was exceeded during the run,
thereby introducing a memory of the previously generated events.
<br/>(ii) Updated handling of random numbers with Gaussian distributions.
The <code>gauss()</code> method now only generates one value at a time.
Instead the new <code>gauss2()</code> method returns a pair of Gauss
numbers, with related time savings. In the old approach one Gaussian 
number could be buffered, which introduced a memory.
<br/>(iii) Minor bug fix in <code>MiniStringFragmentation</code>, where 
the popcorn baryon state was not reset for each new system, again
giving a (flawed) memory.</li> 

<li>New directory <code>rootexample</code> with a simple code example
how to use ROOT for histogramming in PYTHIA runs. See
<?php $filepath = $_GET["filepath"];
echo "<a href='ROOTusage.php?filepath=".$filepath."' target='page'>";?>ROOT Usage</a> for details. Thanks to Rene Brun.
</li>

<li>In the <code>HepMCinterface</code> the checks for unhadronized
quarks and gluons are not performed if hadronization has been switched 
off.</li>

<li>The <code>UserHooks::initPtr(...)</code> has been expanded so that
pointers to more classes (beam particles, random number, couplings, etc.)
are sent in, thereby increasing the scope of possible user-written code.
Also, the <code>UserHooks::doVetoProcessLevel(Event& process)</code>
now allows the <code>process</code> event record to be modified, 
even if it is not recommended. The new <code>Info::pTnow()</code>
method returns the current <i>pT</i> scale of the combined MI, ISR 
and FSR evolution, which can be useful for some user hooks decisions.
</li>

<li>Histograms of the <code>Hist</code> class can now be booked 
with up to 1000 bins, instead of the previous maximum 100. All bins
can be written to file, but the line-printer style printing will join
nearby bins so at most 100 are printed, as before.  </li>

<li>Updated <code>configure</code> and <code>Makefile</code> 
to allow 64-bit compilation and more options. Thanks to Mikhail Kirsanov,
Rene Brun and Fons Rademakers.</li>

<li>Bug fix in <code>ProcessLevel</code> for colour flow checks of 
junctions. Enables the decay of a neutralino to three quarks, e.g.
Thanks to Nils-Erik Bomark.</li>

<li>Bug fixes in <code>TimeShower</code>, where recoil partners in resonance
decays of coloured particles, and recoil partners in QED dipoles when beams
are not allowed to take a recoil, might not be correctly identified. </li>

<li>Bug fix in <code>SpaceShower</code>: when used with a fixed 
<i>alpha_strong</i>, the threshold enhancement factor of 
<i>g -> Q Qbar</i> became undefined. Thanks to Stefan Prestel.</li>

<li>Bug fix in initialization of resonance widths, where the 
<code>minWidth</code> parameter could be used uninitialized,
occasionally resulting in strange initialization problems when
kinematics needs to force at least one resonance to be off-shell.</li>

<li>Minor updates of <code>main06.cc</code>, <code>main10.cc</code>
and <code>main23.cc</code>.</li>

<li>Bug fix in mother pointer of multiple interactions in 
diffractive systems.</li>

</ul>
</li>

<li>8.142: 15 August 2010
<ul>

<li>The strategy for setting up tune values has been changed.
Previously the <code>Tune:ee</code> and <code>Tune:pp</code>
settings were only interpreted during the 
<code>Pythia::init(...)</code> stage. Now they are interpreted
as soon as they are read. Specifically this means that it is 
possible to override any of the tune parameters by putting new
commands below <code>Tune:ee</code> and <code>Tune:pp</code>
in the <code>Pythia::readFile(...)</code> configuration file 
or in the list of <code>Pythia::readString(...)</code> commands.
<br/>As a consequence of this change, the public 
<code>Pythia::initTunes(...)</code> method has been replaced by
two private <code>Settings::initTuneEE(...)</code> and
<code>Settings::initTunePP(...)</code> methods. 
<br/>If <code>Tune:ee</code> or <code>Tune:pp</code> are
nonvanishing by default, then the corresponding tune variables
are set also from the <code>Pythia</code> constructor, before any
user changes are possible. Currently this is not relevant.
</li>

<li>Bug fix in the setup of the <code>Tune:pp = 5</code> and 
<code>= 6</code> options, which meant that diffractive cross 
sections were not reduced as advertised.</li>

<li>The default value of <code>MultipleInteractions:pT0Ref</code>
has been slightly reduced so as to give a somewhat improved
default description. It is not intended as a replacement for the 
specific tunes, however.</li>

<li>Some comparisons between tunes and data, obtained with the 
Rivet package, have been posted on
<a href="http://home.thep.lu.se/~richard/pythia81/">   
http://home.thep.lu.se/~richard/pythia81/</a>.
Further improvements can be expected from future Professor-based
tunes. The current set of 2C, 2M, 3C and 3M "draft tunes" have
deliberately been chosen different also to explore a range of 
possibilities. One not visible in the plots is the fraction
of single and double diffractive events in the inelastic cross 
section. This is 32% in 2C and 2M, while it was reduced to
21% in 3C and 3M, and a recent ATLAS study (ATLAS-CONF-2010-048)
would suggest 28+-4%.</li>

<li>Minor changes in <code>TimeShower</code> and
<code>SpaceShower</code>, as protection if the <i>c</i> or
<i>b</i> masses are set small.</li>

<li>Bug fix in <code>SpaceShower</code>, for case when <i>phi</i> 
angle selection is biased to take into account interference effects.
</li>

<li>The <code>SigmaProcess::convertM2()</code> method now has
been implemented also for <i>2 -> 1</i> processes, so that it is
possible to provide the squared matrix element instead of
<i>sigmaHat</i> for semi-internal processes. A fixed-width 
Breit-Wigner is also inserted, since the matrix element is supposed 
not to include it.</li> 

<li>New method <code>SigmaProcess::setupForME()</code> does an
extended conversion from the internal kinematics to an almost
equivalent one, better adapted to the mass conventions of matrix-element 
caclulation programs, such as massive incoming <i>b</i> quarks.
New flags allow to determine whether the <i>c</i>, <i>b</i>, 
<i>mu</i> and <i>tau</i> should be considered massless or not 
in the calculations.</li>

<li>Two new friend methods <code>table</code> of the <code>Hist</code> 
class allow to print a table with three columns, the first for the 
(common!) <i>x</i> values and the other two for the respective 
histogram values. A new method <code>takeSqrt</code> for the square
root of histogram bin contents. 
</li>

</ul>
</li>

<li>8.145: 10 November 2010
<ul>

<li><code>Couplings</code> is defined in <code>StandardModel.h</code>
as a derived class of <code>coupSM</code> and has only one extra flag: 
<code>isSUSY</code> to check presence of extra couplings.Changed all 
pointers <code>CoupSM*</code> to <code>Couplings*</code> and removed 
explicit references to <code>CoupSUSY*</code>. The <code>coupSUSY</code>
object is only initialised if SUSY couplings are present. The new pointer 
<code>couplingsPtr</code> points either to only SM couplings or SM+SUSY 
couplings based on SLHA data.</li>

<li>New files <code>SusyResonanceWidths.h/cc</code> contains the 
<code>SusyResonanceWidths</code> class, which inherits from
<code>ResonanceWidths</code> but typecasts the <code>couplingsPtr</code> 
to <code>(CoupSUSY*) coupSUSYPtr</code>.  This is the base class for 
all SUSY resonances. It contains the <code>ResonanceSquark</code> class 
for all squark decays. A new flag <code>SLHA:useDecayTable</code> 
to check if internal widths should be overwritten by those read in 
via SLHA.</li>

<li>Added new functionality to <code>SusyLesHouches</code> for read-in 
of generic user blocks in the SLHA format, along with methods to 
extract parameters from them with typecasting controlled by the user. 
Intended for use with user-written semi-internal processes.</li>

<li>Added <code>Sigma1qq2antisquark</code> cross section.

<li>Some new flags and modes in the <code>SUSY</code> and 
<code>SLHA</code> series offer further functionality.</li>

<li>Several further changes as a consequence of the upgrade of the SUSY
machinery.</li> 

<li>Bug/typo fixes in rotation matrices for SUSY couplings and for the
processes <code>qqbar2squarkantisquark</code>,
<code>qq2squarksquark</code> and more.

<li>Improved handling of colour junctions. Added new example program 
<code>main72.cc</code>, to illustrate read-in of color junction 
structures via LHEF (<code>main72.lhe</code>). The example used is SUSY 
with RPV.</li>

<li>New Tune 4C introduced as <code>Tune:pp = 5</code>. The more crude
(non-)tunes 3C and 3M are removed.

<li>New methods <code>Settings::getFlagMap(...)</code>,
<code>Settings::getModeMap(...)</code>, 
<code>Settings::getParmMap(...)</code> and
<code>Settings::getWordMap(...)</code> allows to return a map of all
settings of the respective type that contain a specific string 
in its name.
</li>

<li>Improved description of excited <i>g^*/KK-gluon^*</i> production
in the <code>Sigma1qqbar2KKgluonStar</code> and
<code>ResonanceKKgluon</code> classes.

<li>Possibility added to let Hidden-Valley <i>gamma_v</i> have 
a nonero mass and decay by mixing with the ordinary photon. 
Still experimental.</li>

<li>Minor bug fix in handling of three-body phase space.</li>

<li>Minor correction in <code>Sigma2ffbar2TEVffbar</code> class.</li>

<li>Bug fix for decays of <i>W'</i> to a pair of heavy fermions when 
<i>gV != gA</i>. Thanks to M. Chizhov, see arXiv:0705.3944. </li>

<li>Restore the older EHLQ-based encoding of the <i>f fbar -> Z W</i> 
cross section, which was changed in version 8.140, since comparisons 
with Madgraph gives much better agreement with it than with the 
expression of Brown, Sahdev and Mikaelian.</li>

<li>The <code>HepMCInterface</code> now also stores colour flow 
information for coloured particles.

<li>Pointer to <code>Couplings</code> made available for particle decays.

<li>Minor update in <code>main61.cc</code> for more elegant usage 
of FastJet, as suggested by Gregory Soyez.</li>

</ul>
</li>

</ul>

</body>
</html>

<!-- Copyright (C) 2010 Torbjorn Sjostrand -->
