#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "TH1F.h"
//#include "globalVars.h"

using namespace std;

class BinnedHistGetter
{
  public:
    BinnedHistGetter(string inputFile,string data_type);
    int main(string file);
    void makeHistos(); 
    void init(); 
    void LoopOverFile();
    string inputFileName; 
    string dataType; // mc or data
    vector<vector<vector<TH1F * > > > getHists(){
      init();
      makeHistos();
      LoopOverFile();
      return hists;
    }
    
    TFile *f;
    TTree *ttree;

    vector<vector<vector<TH1F * > > > hists;  // 3d vector in form [var][etBin][etaBin]
    //vector<vector<string > > histnames;


   // int etBins[11] = {8,15,20,25,30,40,50,60,80,100,1000};
   // float etaBins[9]= {0.00,0.60,0.80,1.15,1.37,1.52,1.81,2.01,2.37};


    vector<float> * ph_pt;
    vector<float> * ph_eta;
    vector<float> *ph_phi;
    vector<float> *ph_reta;
    vector<float> *ph_rphi;
    vector<float> *ph_weta2;
    vector<float> *ph_w1;
    vector<float> *ph_wstot;
    vector<float> *ph_fside;
    vector<float> *ph_rhad;
    vector<float> *ph_rhad1;
    vector<float> *ph_weight;


};
