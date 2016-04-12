#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "TH1F.h"
#include "include/progress_bar.h"
#include "include/Config.h"
#include <cmath>

using namespace std;

class BinnedHistGetterNTUP
{
  public:
    BinnedHistGetterNTUP(string inputFile,string inputFileType, bool useNoFFvariables);
    void makeHistos(); 
    void init();  // initializae branches 
    void LoopOverFile(); // main loop
    bool Cut();   // cuts are here!
    void writeHistsToFiles();
    string inputFileName; 
    bool data;    // true if data file
    bool noFF;    // determines when to use noFF variables
    void Run(){   // main function that will be run
      init();
      makeHistos();
      LoopOverFile();
      writeHistsToFiles();
    }
    
    TFile *f;
    TTree *ttree;

    vector<vector<vector<TH1F * > > > hists_conv;  // 3d vector in form [var][etBin][etaBin]
    vector<vector<vector<TH1F * > > > hists_unconv;  // 3d vector in form [var][etBin][etaBin]

    map<string,float> inputVars; // map for clever ROOT branch access
    
    // variables from ntuple
    float   sumW;
    float   ph_pt;
    float   ph_eta;
    float   ph_phi;
    float   ph_weight;
    float   ph_topoetcone40;
    float   ph_topoetcone30;
    float   ph_topoetcone20;
    float   weight;
    float   weightnpu;
    float   mc_pu_weight;
    int     ph_convFlag;
    UInt_t  mc_channel_number;
    Bool_t  ph_tight_mc15;
    Bool_t  ph_loose_mc15;
    Float_t ph_ptcone20;
    Float_t mc_gen_weight;

};
