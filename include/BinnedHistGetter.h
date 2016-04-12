#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "TH1F.h"
#include "include/Config.h"
#include "include/progress_bar.h"

using namespace std;

class BinnedHistGetter
{
  public:
    BinnedHistGetter(string inputFile,string convSt, string data_type);
    int main(string file);
    void makeHistos(); 
    void init(); 
    void LoopOverFile();
    void writeHistsToFile();
    string inputFileName; 
    string dataType; // mc or data
    string convStatus; // u or c
    vector<vector<vector<TH1F * > > > getHists(){
      init();
      makeHistos();
      LoopOverFile();
      return hists;
    }
    void Run()
    {
      init();
      makeHistos();
      LoopOverFile();
      writeHistsToFile();
    }
    
    TFile *f;
    TTree *ttree;

    vector<vector<vector<TH1F * > > > hists;  // 3d vector in form [var][etBin][etaBin]
    map<string,vector<float> * > inputVars;

    vector<float> * ph_pt;
    vector<float> * ph_eta;
    vector<float> *ph_phi;
    vector<float> *ph_weight;
};
