#ifndef ILCTARGETLOADER_H
#define ILCTARGETLOADER_H
//////////////////////////////////////////////////////////
// Loader class for TARGET                                 //
//////////////////////////////////////////////////////////
#include <IlcLoader.h>
#include <IlcESDVertex.h>
#include <IlcTARGETgeom.h>
#include <IlcTARGETpidESD.h>
class IlcTARGETdigit;
class TObjArray;

class IlcTARGETLoader: public IlcLoader{
  public:
    IlcTARGETLoader();
    IlcTARGETLoader(const Char_t *name,const Char_t *topfoldername);
    IlcTARGETLoader(const Char_t *name,TFolder *topfolder);
// // // //     IlcTARGETLoader(const IlcTARGETLoader &ob); // copy constructor
    IlcTARGETLoader& operator=(const IlcTARGETLoader & /* source */); // ass.


    virtual ~IlcTARGETLoader();

    void           MakeTree(Option_t* opt);
    virtual void   SetupDigits(TObjArray *digPerDet,Int_t n,
                                const Char_t **digclass); // Sets up digits
    // Gets the IlcTARGETdigit for a given module and a specific digit in that
    // module. Array of digits stored in IlcTARGET (must use 
    // SetupDigits(IlcTARGET *its)).
    // virtual IlcTARGETdigit* GetDigit(IlcTARGET *its,Int_t module,Int_t digit);
    // Gets the IlcTARGETdigit for a given module and a specific digit in that
    // module. Array of digits stored in a user defined TObjArray digPerDet
    virtual IlcTARGETdigit* GetDigit(TObjArray *digPerDet,Int_t module,Int_t digit);

    //Raw Clusters
    IlcDataLoader* GetRawClLoader() {return GetDataLoader("Raw Clusters");}
    virtual void   CleanRawClusters() {
        GetRawClLoader()->GetBaseLoader(0)->Clean();}
    Int_t          LoadRawClusters(Option_t* opt=""){
        return GetRawClLoader()->GetBaseLoader(0)->Load(opt);}
    void           SetRawClustersFileName(const TString& fname){
        GetRawClLoader()->SetFileName(fname);}
    // returns a pointer to the tree of  RawClusters
    TTree*         TreeC(){ return GetRawClLoader()->Tree();} 
    void           UnloadRawClusters(){
        GetRawClLoader()->GetBaseLoader(0)->Unload();}
    virtual Int_t  WriteRawClusters(Option_t* opt=""){
        return GetRawClLoader()->GetBaseLoader(0)->WriteData(opt);}

    //Vertices
    IlcDataLoader* GetVertexDataLoader() {
        return GetDataLoader("Primary Vertices");}
    virtual void   CleanVertices() {
        GetVertexDataLoader()->GetBaseLoader(0)->Clean();}
    Int_t          LoadVertices(Option_t* opt=""){
        return GetVertexDataLoader()->GetBaseLoader(0)->Load(opt);}
    void           SetVerticesFileName(const TString& fname){
        GetVertexDataLoader()->SetFileName(fname);}
    void           UnloadVertices(){
        GetVertexDataLoader()->GetBaseLoader(0)->Unload();}
    virtual Int_t  WriteVertices(Option_t* opt=""){
        return GetVertexDataLoader()->GetBaseLoader(0)->WriteData(opt);}
    virtual Int_t PostVertex(IlcESDVertex *ptr){
        return GetVertexDataLoader()->GetBaseLoader(0)->Post(ptr);}
    //    virtual void SetVerticesContName(const char *name){
    //       GetVertexDataLoader()->GetBaseLoader(0)->SetName(name);}
    IlcESDVertex *GetVertex(){
        return static_cast <IlcESDVertex*>(GetVertexDataLoader()->
                                           GetBaseLoader(0)->Get());}
    //V0s
    IlcDataLoader* GetV0DataLoader() {return GetDataLoader("V0 Vertices");}
    virtual void   CleanV0s() {GetV0DataLoader()->GetBaseLoader(0)->Clean();}
    Int_t          LoadV0s(Option_t* opt=""){
        return GetV0DataLoader()->GetBaseLoader(0)->Load(opt);}
    void           SetV0FileName(const TString& fname){
        GetV0DataLoader()->SetFileName(fname);}
    void           UnloadV0s(){GetV0DataLoader()->GetBaseLoader(0)->Unload();}
    virtual Int_t  WriteV0s(Option_t* opt=""){
        return GetV0DataLoader()->GetBaseLoader(0)->WriteData(opt);}
    TTree*         TreeV0(){ return GetV0DataLoader()->Tree();}

    //Cascades
    IlcDataLoader* GetCascadeDataLoader() {return GetDataLoader("Cascades");}
    virtual void   CleanCascades() {
        GetCascadeDataLoader()->GetBaseLoader(0)->Clean();}
    Int_t          LoadCascades(Option_t* opt=""){
        return GetCascadeDataLoader()->GetBaseLoader(0)->Load(opt);}
    void           SetCascadeFileName(const TString& fname){
        GetCascadeDataLoader()->SetFileName(fname);}
    void           UnloadCascades(){
        GetCascadeDataLoader()->GetBaseLoader(0)->Unload();}
    virtual Int_t  WriteCascades(Option_t* opt=""){
        return GetCascadeDataLoader()->GetBaseLoader(0)->WriteData(opt);}
    TTree*         TreeX(){ return GetCascadeDataLoader()->Tree();}

    //Back Propagated Tracks
    IlcDataLoader* GetBackTracksDataLoader() {
        return GetDataLoader("Back Propagated Tracks");}
    virtual void   CleanBackTracks() {
        GetBackTracksDataLoader()->GetBaseLoader(0)->Clean();}
    Int_t          LoadBackTracks(Option_t* opt=""){
        return GetBackTracksDataLoader()->GetBaseLoader(0)->Load(opt);}
    void           SetBackTracksFileName(const TString& fname){
        GetBackTracksDataLoader()->SetFileName(fname);}
     // returns a pointer to the tree of  BackTracks
    TTree*         TreeB(){ return GetBackTracksDataLoader()->Tree();}
    void           UnloadBackTracks(){
        GetBackTracksDataLoader()->GetBaseLoader(0)->Unload();}
    virtual Int_t  WriteBackTracks(Option_t* opt=""){
        return GetBackTracksDataLoader()->GetBaseLoader(0)->WriteData(opt);}

    // Geometry. Geom is read from file, unless already loaded
    // readout from file can be forced if force=kTRUE
    IlcTARGETgeom* GetTARGETgeom(Bool_t force=kFALSE); 
    void SetTARGETgeom(IlcTARGETgeom* g){fGeom=g;}
    // PID
    IlcTARGETpidESD* GetTARGETpid() const {return fTARGETpid;}
    void  AdoptTARGETpid(IlcTARGETpidESD* pid) {fTARGETpid=pid;}
  protected:

    // METHODS
    virtual void   MakeRawClustersContainer() {GetRawClLoader()->MakeTree();}
    Int_t          PostRawClusters(){
        return GetRawClLoader()->GetBaseLoader(0)->Post();}

    virtual void   MakeBackTracksContainer() {
        GetBackTracksDataLoader()->MakeTree();}
    Int_t          PostBackTracks(){
        return GetBackTracksDataLoader()->GetBaseLoader(0)->Post();}
    virtual void   MakeV0Container() {GetV0DataLoader()->MakeTree();}
    Int_t          PostV0s(){
        return GetV0DataLoader()->GetBaseLoader(0)->Post();}

    virtual void   MakeCascadeContainer() {GetCascadeDataLoader()->MakeTree();}
    Int_t          PostCascades(){
        return GetCascadeDataLoader()->GetBaseLoader(0)->Post();}

    // DATA
    static const TString fgkDefaultRawClustersContainerName;  //default for Raw Clusters container name
    static const TString fgkDefaultBackTracksContainerName;   //default for Back propag. tracks container name
    static const TString fgkDefaultVerticesContainerName;     //default for primary vertices container name
    static const TString fgkDefaultV0ContainerName;           //default for V0 container name
    static const TString fgkDefaultCascadeContainerName;      //default fo cascade container name
    IlcTARGETpidESD* fTARGETpid; //! pointer for TARGET pid
    IlcTARGETgeom *fGeom;     //! pointer to the TARGET geometry class
    ClassDef(IlcTARGETLoader,5) // Loader for additional TARGET specific trees.
};
 
#endif
