#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "TH1F.h"
#include "TH2D.h"
#include "TH1.h"
#include "TCanvas.h"
#include "include/FudgeFactor.h"
#include "TF1.h"
using namespace std;

class FFcalcAndCompare
{
  public:
    vector<vector<vector<TH1 *> > > dataPDFs;
    vector<vector<vector<TH1 *> > > mcPDFs;
    vector<vector<vector<FudgeFactor> > > FudgeFactors;

    vector<vector<vector<TH1F *> > > chiSqPlots;
    vector<vector<vector<TCanvas *> > > overlayPDFs;
    
    FFcalcAndCompare(vector<vector<vector<TH1 *> > > &data_PDFs ,vector<vector<vector<TH1 *> > > &mc_PDFs, string convSt);
    FFcalcAndCompare(string data_PDFs_filename ,string mc_PDFs_filename, string convSt);
    ~FFcalcAndCompare();
    vector<vector<vector<TH1 *> > > getPDFsFromFile(string filename);
    // also add constructor for doing it from PDFs file
    FudgeFactor GetFF(TH1 * dataHist, TH1 * mcHist, int var, int etBin, int etaBin);
    void GetFFs();
    float GetFFerror(TH1F * hist, int shift_min);
    float getXsq(TH1 * dataHist, TH1* NmcHist, int shift);
    void Run();
    void writeErrorsToCFile();
    void writeErrorsToPyFile();
    void writeFFsToCFile();
    void writeFFsToPyFile();
    void writePlotsToFile();    
    void writeFFsToRootFile(); 
    void writeFFsCompareToRootFile(); 
    string convStatus;
};
