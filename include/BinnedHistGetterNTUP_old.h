
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
    float ph_reta;
    float ph_rphi;
    float ph_weta2;
    float ph_w1;
    float ph_wstot;
    float ph_fside;
    float ph_rhad;
    float ph_rhad1;
    float ph_noFF_reta;
    float ph_noFF_rphi;
    float ph_noFF_weta2;
    float ph_noFF_w1;
    float ph_noFF_wstot;
    float ph_noFF_fside;
    float ph_noFF_rhad;
    float ph_noFF_rhad1;
    float ph_weight;
    Bool_t ph_tight;
    Bool_t ph_loose;
    float ph_topoetcone40;
    float ph_topoetcone30;
    float ph_topoetcone20;
    float weight;
    float weightnpu;
    float mc_pu_weight;
    int ph_convFlag;
    Bool_t HLT_g10_loose;
    Bool_t HLT_g25_loose_L1EM15;
    Bool_t HLT_g40_loose_L1EM15;
    Bool_t HLT_g60_loose;
    Bool_t HLT_g70_loose;
    Bool_t HLT_g80_loose;
    Bool_t HLT_g100_loose;
    Bool_t HLT_g120_loose;
    Bool_t HLT_g140_loose;
    UInt_t mc_channel_number;
    Bool_t   ph_tight_mc15;
    Bool_t   ph_loose_mc15;
    Float_t          ph_ptcone20;
    Float_t   mc_gen_weight;

};
