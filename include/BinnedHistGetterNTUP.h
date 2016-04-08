
#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "TH1F.h"
#include "include/progress_bar.h"
//#include "globalVars.h"

using namespace std;

class BinnedHistGetterNTUP
{
  public:
    BinnedHistGetterNTUP(string inputFile,string inputFileType, bool useNoFFvariables);
    void makeHistos(); 
    void init(); 
    void LoopOverFile();
    bool Cut();
    void writeHistsToFiles();
    string inputFileName; 
    bool data;
    bool noFF;
    void Run(){
      init();
      makeHistos();
      LoopOverFile();
      writeHistsToFiles();
    }
    
    TFile *f;
    TTree *ttree;

    vector<vector<vector<TH1F * > > > hists_conv;  // 3d vector in form [var][etBin][etaBin]
    vector<vector<vector<TH1F * > > > hists_unconv;  // 3d vector in form [var][etBin][etaBin]
    //vector<vector<string > > histnames;

    map<string,float> inputVars;

   // int etBins[11] = {8,15,20,25,30,40,50,60,80,100,1000};
   // float etaBins[9]= {0.00,0.60,0.80,1.15,1.37,1.52,1.81,2.01,2.37};

    float sumW;

    float  ph_pt;
    float  ph_eta;
    float ph_phi;
    float ph_weight;
    float ph_topoetcone40;
    float ph_topoetcone30;
    float ph_topoetcone20;
    float weight;
    float weightnpu;
    float mc_pu_weight;
    int ph_convFlag;
    UInt_t mc_channel_number;
    Bool_t   ph_tight_mc15;
    Bool_t   ph_loose_mc15;
    Float_t          ph_ptcone20;
    Float_t   mc_gen_weight;

};
