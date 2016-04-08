#include "include/BinnedHistGetter.h"
#include "include/Config.h"
#include "include/progress_bar.h"
using namespace std;


BinnedHistGetter::BinnedHistGetter(string inputFile,string data_type){
  f = new TFile(inputFile.c_str());
  inputFileName=f->GetName();
  dataType=data_type;
}
void BinnedHistGetter::init(){
  //f = new TFile(inputFileName.c_str());
  ttree = (TTree*)f->Get("tree");
  
  ph_pt=0;    ph_eta=0;   ph_phi=0;   ph_reta=0;
  ph_rphi=0;  ph_weta2=0; ph_w1=0;    ph_wstot=0;
  ph_fside=0; ph_rhad=0;  ph_rhad1=0; ph_weight=0;

  ttree->SetBranchAddress("ph_eta", &ph_eta);
  ttree->SetBranchAddress("ph_phi", &ph_phi);
  ttree->SetBranchAddress("ph_reta", &ph_reta);
  ttree->SetBranchAddress("ph_rphi", &ph_rphi);
  ttree->SetBranchAddress("ph_weta2", &ph_weta2);
  ttree->SetBranchAddress("ph_w1", &ph_w1);
  ttree->SetBranchAddress("ph_wstot", &ph_wstot);
  ttree->SetBranchAddress("ph_fside", &ph_fside);
  ttree->SetBranchAddress("ph_rhad", &ph_rhad);
  //ttree->SetBranchAddress("ph_rhad1", &ph_rhad1);
  ttree->SetBranchAddress("ph_weight", &ph_weight);
  ttree->SetBranchAddress("ph_pt", &ph_pt);


}
void BinnedHistGetter::makeHistos(){
for(int var = 0; var < Config::Nvars; var++)
  {
    vector<vector<TH1F * > > VarHists;
    //vector<string> VarHistNames;
    for(int etBin =0;etBin<Config::NetBins;etBin++)
    {
      vector<TH1F * > VarEtHists;
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        string histName = Config::varsN[var]+"_";
        histName.append(Config::etNames[etBin]);
        histName.append(Config::etaNames[etaBin]);
        //VarHistNames.push_back(histName);
        //VarHists.push_back(new TH1F(histName.c_str(),histName.c_str(),Nbins,xmins[var],xmaxs[var]));
        VarEtHists.push_back(new TH1F(histName.c_str(),histName.c_str(),Config::Nbins,Config::xmins[var],Config::xmaxs[var])); 
      }
      VarHists.push_back(VarEtHists);
    }
    hists.push_back(VarHists);
    //histnames.push_back(VarHistNames);
  }
}

void BinnedHistGetter::LoopOverFile(){
  cout << "Looping over file: " << inputFileName << endl;
  long nentries = (long)ttree->GetEntries();
  int Nvars = Config::Nvars;
  //float progress=0.0;
  progress_bar bar(100);
  for (Int_t i=0;i<nentries;i++) {
    ttree->GetEntry(i);
    for(int i=0;i<Config::NetBins;i++)
    {
      if(ph_pt->at(0) < Config::etBins[i]) // just to speed up
        continue;
      for(int j=0;j<Config::NetaBins;j++)
      {

        if( fabs(ph_eta->at(0)) > Config::etaBins[j] && fabs(ph_eta->at(0)) < Config::etaBins[j+1] && ph_pt->at(0)/1000 > Config::etBins[i] && ph_pt->at(0)/1000 < Config::etBins[i+1])
        {
          float weight = 1;
          if (dataType=="mc") weight = ph_weight->at(0);
          hists[0][i][j]->Fill(ph_reta->at(0)     ,weight);
          if(Nvars < 2) continue;
          hists[1][i][j]->Fill(ph_rphi->at(0)     ,weight);
          if(Nvars < 3) continue;
          hists[2][i][j]->Fill(ph_weta2->at(0)    ,weight);
          if(Nvars < 4) continue;
          hists[3][i][j]->Fill(ph_w1->at(0)       ,weight);
          if(Nvars < 5) continue;
          hists[4][i][j]->Fill(ph_wstot->at(0)    ,weight);
          if(Nvars < 6) continue;
          hists[5][i][j]->Fill(ph_fside->at(0)    ,weight);
          if(Nvars < 7) continue;
          hists[6][i][j]->Fill(ph_rhad->at(0)     ,weight);
          //hists[7][i][j]->Fill(ph_rhad1->at(0)    );// ,ph_weight->at(0));
          //hists[8][i][j]->Fill(ph_pt->at(0)/1000  );// ,ph_weight->at(0));
          //hists[9][i][j]->Fill(ph_eta->at(0)      );// ,ph_weight->at(0));
        }
      }
    }
    //if(i % (nentries/100) == 0 ) progress += 0.01;
    if(i % (nentries/100) == 0 ) bar.tick();
  }
  bar.end();
  //int barWidth=70;
  //std::cout << "[";
  //int pos = barWidth * 1.0;
  //for (int i = 0; i < barWidth; ++i) {
  //    if (i < pos) std::cout << "=";
  //    else if (i == pos) std::cout << ">";
  //    else std::cout << " ";
  //} 
  //std::cout << "] " << int(100.0) << " %\r";
  //std::cout.flush(); 
  //std::cout << endl;
}





int BinnedHistGetter::main(string file)
{
  std::cout << "help me" << std::endl;



  return 1;
}

