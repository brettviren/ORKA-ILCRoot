// $Id: IlcEveTrackFitter.h 24810 2008-03-31 16:33:43Z mtadel $
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

#ifndef IlcEveTrackFitter_H
#define IlcEveTrackFitter_H

#include <TEvePointSet.h>
#include <map>

class TGraphErrors;
class TGraph;
class IlcRieman;

class TEveTrackList;

class IlcEveTrackFitter : public TEvePointSet
{
public:
  IlcEveTrackFitter(const Text_t* name = "TrackFitter", Int_t nPoints=0);
  virtual ~IlcEveTrackFitter();

  virtual void DestroyElements();

  virtual void  AddFitPoint(Int_t pointId);  // slot for TEvePointSet::PointSelected() signal

  virtual void  Start();
  virtual void  Stop();
  virtual void  FitTrack();
  virtual void  Reset(Int_t nPoints=0, Int_t nIntIds=0);

  Bool_t        GetConnected() const { return fConnected; }
  IlcRieman*    GetRieman()    const { return fRieman; }

  TGraph*       GetGraphPicked() const { return fGraphPicked; }
  TGraphErrors* GetGraphHelix()  const { return fGraphHelix; }
  void          DrawDebugGraph();


protected:

  struct Point_t
  {
    TEvePointSet   *fPS;   // point set
    Int_t           fIdx;  // id in point set

    Point_t(TEvePointSet* ps=0, Int_t i=0) : fPS(ps), fIdx(i) {}
    Point_t(const Point_t& p) : fPS(p.fPS), fIdx(p.fIdx)  {}

    Point_t& operator=(const Point_t& p)
    {
      fPS = p.fPS; fIdx = p.fIdx; return *this;
    }

    bool operator<(const Point_t& o) const
    {
      if (fPS != o.fPS) return fPS < o.fPS;
      return fIdx < o.fIdx;
    }
  };

  typedef std::map<Point_t, Int_t> PointMap_t;

  Float_t            fAlpha;           // transformation angle to IlcRieman local system (where x>>y)
  IlcRieman*         fRieman;          // rieman fitter

  Bool_t             fConnected;       // connection to the TEvePointSet signal

  PointMap_t         fSPMap;           // map of selected points
  TEveTrackList*     fTrackList;       // list of tracks removed in the destructor

  TGraph            *fGraphPicked;     // graph of selected points debug info
  TGraphErrors      *fGraphHelix;      // graph of fitted points for debug info

private:
  IlcEveTrackFitter(const IlcEveTrackFitter&);            // Not implemented
  IlcEveTrackFitter& operator=(const IlcEveTrackFitter&); // Not implemented

  ClassDef(IlcEveTrackFitter, 0); // Interface of TEvePointSet allowing helix fit.
};

#endif
