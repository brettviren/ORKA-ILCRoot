#ifndef IlcPVECEvalRecPoint_H
#define IlcPVECEvalRecPoint_H

// --- ROOT system ---
#include "TObjArray.h"


#include "IlcPVECCpvRecPoint.h"
class IlcPVECClusterizer;
class IlcPVECRecPoint;
class IlcPVECRecManager;
class TString;

class IlcPVECEvalRecPoint : public IlcPVECCpvRecPoint
{
public:

  IlcPVECEvalRecPoint();
  IlcPVECEvalRecPoint(Bool_t cpv,IlcPVECEvalRecPoint* parent);
  IlcPVECEvalRecPoint(Int_t cluster, Bool_t cpv);
  virtual ~IlcPVECEvalRecPoint() {}

  Bool_t TooClose(IlcPVECRecPoint* pt) const ;
  Bool_t NeedToSplit() const ;

  void MergeClosePoint();
  void SplitMergedShowers();
  Int_t UnfoldLocalMaxima();
  void EvaluatePosition();
  Bool_t KillWeakPoint();

  Bool_t IsEmc(void) const { return fIsEmc; }
  Bool_t IsCPV(void) const { return fIsCpv; }

  void SetLocalPosition(TVector3& pos) { fLocPos=pos; }
  void UpdateWorkingPool();
  void DeleteParent();

  Int_t HasChild(TObjArray& children);

  void MakeJob();

  IlcPVECClusterizer* GetClusterizer();
  IlcPVECRecManager* GetReconstructionManager() const;
  
  void PrintPoint();

  IlcPVECRecPoint* Parent();
  Float_t Chi2Dof() const;

  const TObject* GetWorkingPool();

  void AddToWorkingPool(TObject* obj);
  TObject* GetFromWorkingPool(Int_t i);
  Int_t InWorkingPool();
  void RemoveFromWorkingPool(TObject* obj);
  void PrintWorkingPool();

  enum RecPointType {kEmc,kCpv};

private:

  void Init();
  void InitTwoGam(Float_t* gamma1, Float_t* gamma2);
  void TwoGam(Float_t* gamma1, Float_t* gamma2);
  void UnfoldTwoMergedPoints(Float_t* gamma1, Float_t* gamma2);

private:

  Bool_t fIsEmc;    // true if EMC
  Bool_t fIsCpv;    // true if CPV
  Int_t fParent;    // parent
  Float_t fChi2Dof; // chi^2 per degree of freedom
  TString fEventFolderName; // Folder name

  ClassDef(IlcPVECEvalRecPoint,1)

};

#endif
