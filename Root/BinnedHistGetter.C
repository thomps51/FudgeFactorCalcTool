#include "include/BinnedHistGetter.h"
#include "include/Config.h"
#include "include/progress_bar.h"
using namespace std;


BinnedHistGetter::BinnedHistGetter(string inputFile,string convSt, string data_type){
  f = TFile::Open(inputFile.c_str());
  inputFileName=f->GetName();
  dataType=data_type;
  if (dataType != "data" && dataType != "mc")
  {
    cerr << "BinnedHistGetter::datatype not set to data or mc, please check arguments!" << endl;
  }
  convStatus=convSt;
  if (convSt != "u" && convSt != "c")
  {
    cerr << "BinnedHistGetter::convStatus not set to c or u, please check arguments!" << endl;
  } 
  for(int i =0; i< Config::NvarsInputPskim ; i++)
  {
    inputVars[Config::InputVarsPskim[i]] = 0;
  }  
}
void BinnedHistGetter::init(){
  ttree = (TTree*)f->Get("tree");
  
  ph_pt=0;    ph_eta=0;   ph_phi=0; 
  ph_weight=0;

  for(int i = 0; i< Config::NvarsInputPskim ; i++)
  {
    string inputvarname = Config::InputVarsPskim[i] ;
    ttree->SetBranchAddress(inputvarname.c_str(), &inputVars[inputvarname] );
  }

  ttree->SetBranchAddress("ph_eta", &ph_eta);
  ttree->SetBranchAddress("ph_phi", &ph_phi);
  ttree->SetBranchAddress("ph_weight", &ph_weight);
  ttree->SetBranchAddress("ph_pt", &ph_pt);


}
void BinnedHistGetter::makeHistos(){
for(int var = 0; var < Config::Nvars; var++)
  {
    vector<vector<TH1F * > > VarHists;
    for(int etBin =0;etBin<Config::NetBins;etBin++)
    {
      vector<TH1F * > VarEtHists;
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        string histName = Config::varsN[var]+"_"+ Config::etNames[etBin] + Config::etaNames[etaBin] + "_" + convStatus ;
        VarEtHists.push_back(new TH1F(histName.c_str(),histName.c_str(),Config::Nbins,Config::xmins[var],Config::xmaxs[var])); 
      }
      VarHists.push_back(VarEtHists);
    }
    hists.push_back(VarHists);
  }
}

void BinnedHistGetter::LoopOverFile(){
  cout << "Looping over file: " << inputFileName << endl;
  long nentries = (long)ttree->GetEntries();
  progress_bar bar(100);
  for (Int_t i=0;i<nentries;i++) {
    ttree->GetEntry(i);
    for(int i=0;i<Config::NetBins;i++)
    {
      for(int j=0;j<Config::NetaBins;j++)
      {

        if( fabs(ph_eta->at(0)) > Config::etaBins[j] && fabs(ph_eta->at(0)) < Config::etaBins[j+1] && ph_pt->at(0)/1000 > Config::etBins[i] && ph_pt->at(0)/1000 < Config::etBins[i+1])
        {
          float weight = 1;
          if (dataType=="mc") weight = ph_weight->at(0);
          
          for(int var = 0; var< Config::Nvars ; var++)
          {
            string inputvarname = Config::InputVarsPskim[var] ;
            float value         = inputVars[inputvarname]->at(0);
            hists[var][i][j]->Fill(value, weight);
          }
        }
      }
    }
    if(i % (nentries/100) == 0 ) bar.tick();
  }
  bar.end();
}
void BinnedHistGetter::writeHistsToFile()
{
  string filename = "output/hists/hists_" + dataType + "_"+convStatus  + ".root";
  TFile f(filename.c_str(),"recreate");
  for(int var =0 ; var < Config::Nvars; var++ )
  {
      for(int etBin =0; etBin < Config::NetBins ; etBin++)
      {
          for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
          {
              hists[var][etBin][etaBin]->Write();
          }
      }
  }
  cout << convStatus << " binned "<< dataType << " hists written to file: " << filename << endl;
  f.Close();
}
