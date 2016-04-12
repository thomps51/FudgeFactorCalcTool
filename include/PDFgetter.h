#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "TH1F.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMVA/PDF.h"
#include "include/Config.h"
#include <thread>
#include <unistd.h>
#include "include/progress_bar.h"


using namespace std;

class PDFgetter
{
    public:
    PDFgetter(vector<vector<vector<TH1F *> > > &Hists, string convSt, string data_type);
    PDFgetter(string inputfilename, string convSt, string data_type);
    vector<vector<vector<TH1 *> > > makeSmoothedPDFs();
    vector<vector<vector<TH1 *> > > makeSmoothedPDFsParallel();  // TMVA is unstable parallel-ly, this works 8/10 times 
    void getPDFsForVar(unsigned int var);
    vector<vector<vector<TH1F *> > > getHistsFromFile(string filename);
    void writeToFile();

    void Run() {
      makeSmoothedPDFs();
      writeToFile();
    }

    vector<vector<vector<TH1F *> > > hists;
    vector<vector<vector<TH1 *> > > pdfs;
    string convStatus;
    string datatype;
};
