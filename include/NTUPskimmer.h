#include "include/progress_bar.h"
#include "include/Config.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <string>

using namespace std;

class NTUPskimmer
{
  public:
    NTUPskimmer(string inputFile, string inputFileType, bool useNoFFvariables);
    
    string inputFileName;
    TFile * ntupFile;
    TFile * outputFile_c;
    TFile * outputFile_u;

    TTree * inputTree;
    TTree * outputTree_c;
    TTree * outputTree_u;
    string inputType;
    bool data;
    bool noFF;

    map<string,float> inputVars;
    map<string,float> outputVars;

    void init();
    void LoopOverFile();
    bool Cut();
    void Write();
    void Run(){
      init();
      LoopOverFile();
      Write();
    }
    
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
