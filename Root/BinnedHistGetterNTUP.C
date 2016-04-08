#include "include/BinnedHistGetterNTUP.h"
#include "include/Config.h"
#include <cmath>
using namespace std;


BinnedHistGetterNTUP::BinnedHistGetterNTUP(string inputFile, string inputFileType, bool useNoFFvariables){
  f = new TFile(inputFile.c_str());
  inputFileName=f->GetName();
  sumW=0;
  if (inputFileType == "data")  data=true;
  else                          data=false;
  if (useNoFFvariables) noFF=true;
  else                  noFF=false;
  
  for(int i =0; i< Config::NvarsInputNTUP ; i++) 
  {
    if (noFF) inputVars[Config::InputVarsNoFFNTUP[i]] = 0;
    else      inputVars[Config::InputVarsNTUP[i]]     = 0;
  }

}
void BinnedHistGetterNTUP::init(){
  ttree = (TTree*)f->Get("photon");
  
  ph_pt=0;    ph_eta=0;   ph_phi=0;
  
  ph_topoetcone40=0; ph_topoetcone30=0;
  ph_topoetcone20=0;      weight=0;         weightnpu=0;       mc_pu_weight=0; 
  ph_convFlag=0;
  mc_channel_number=0; ph_tight_mc15=0;
  ph_loose_mc15=0; ph_ptcone20=0; mc_gen_weight=0;
 
  for(int i = 0; i< Config::Nvars ; i++)
  {
    string inputvarname = (noFF) ? Config::InputVarsNoFFNTUP[i] : Config::InputVarsNTUP[i] ;
    ttree->SetBranchAddress(inputvarname.c_str(), &inputVars[inputvarname] );
  }
  ttree->SetBranchAddress("ph_pt", &ph_pt);
  ttree->SetBranchAddress("ph_eta", &ph_eta);
  ttree->SetBranchAddress("ph_phi", &ph_phi);
  ttree->SetBranchAddress("ph_topoetcone40", &ph_topoetcone40);
  ttree->SetBranchAddress("ph_topoetcone30", &ph_topoetcone30);
  ttree->SetBranchAddress("ph_topoetcone20", &ph_topoetcone20);
  
  ttree->SetBranchAddress("ph_convFlag", &ph_convFlag);
  if (! data) {
  ttree->SetBranchAddress("mc_channel_number", &mc_channel_number);
  ttree->SetBranchAddress("mc_gen_weight", &mc_gen_weight);
  ttree->SetBranchAddress("mc_pu_weight", &mc_pu_weight);
  }
  ttree->SetBranchAddress("ph_tight_mc15", &ph_tight_mc15);
  ttree->SetBranchAddress("ph_loose_mc15", &ph_loose_mc15);
  ttree->SetBranchAddress("ph_ptcone20", &ph_ptcone20);
}

bool BinnedHistGetterNTUP::Cut(){
    int tight = ph_tight_mc15;   //CHANGE THESE MAYBE
    //int loose = ph_loose_mc15;
    int conv  = ph_convFlag;  ///
    float pt = ph_pt/1000.;
    float looseiso = ph_pt*0.065;
    //float tightiso = pt*0.022  + 2.45; //divide to convert to GeV
    float trackiso = ph_ptcone20/ph_pt;
    //cout << "ph_isoloose" << ph_isoloose << endl;
    //cout << "ph_loose" << ph_loose_mc15 << endl;
    //cout << "ph_isConv" << ph_truth_isConv << endl;
    sumW+=mc_gen_weight;
    // Pt cut
    if(pt < 8. ) return false;
    //if(pt < 8. || pt > 50.) return false;

     //Loose or tight selection
    //if(loose == 1 ) return false; //choose not loose
    //if(loose != 1 ) return false; //choose loose
    if(tight != 1 ) return false; //choose tight

    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/IsolationSelectionTool#Photons
    //if(ph_topoetcone40/1000 > tightiso) return false;
    //if(ph_topoetcone40/1000 < tightiso) return false; //fail tight iso

    if(pt> 25. && (ph_topoetcone20 > looseiso) ) return false;

    //track iso
    if(trackiso > 0.05) return false;
    //if(trackiso > 0.05) return false;



    //conv / unconv
    bool conv_photon   =false;
    bool unconv_photon =false;

    if(conv%10 == 0) unconv_photon=true;
    if(conv%10 != 0) conv_photon=true;




    float wei = 1;
    if(!data)
    {
        if(mc_channel_number == 423099) wei=6109.329;
        else if(mc_channel_number == 423100) wei=721.806;
        else if(mc_channel_number == 423101) wei=51.243;
        else if(mc_channel_number == 423102) wei=11.249;
        else if(mc_channel_number == 423103) wei=4.7426;
        else if(mc_channel_number == 423104) wei=1.0864;
        else if(mc_channel_number == 423105) wei=0.0569;
        else if(mc_channel_number == 423106) wei=0.00413;
        else if(mc_channel_number == 423107) wei=0.00287;
         //JF
        else if(mc_channel_number == 423300) wei=29923.6019;
        else if(mc_channel_number == 423303) wei=1009.5559;
         //zllgamma
        else if(mc_channel_number == 301535) wei=0.09857;
        else if(mc_channel_number == 301536) wei=0.09488;
        else if(mc_channel_number == 361372) wei= mc_gen_weight*3.2*2.2066*pow(10,6)*0.77858/40165.8;
        else if(mc_channel_number == 361373) wei= mc_gen_weight*3.2*2.2054*pow(10,6)*0.14410/21527.1;
        else if(mc_channel_number == 361374) wei= mc_gen_weight*3.2*2.2059*pow(10,6)*0.078658/32271.2;
        else if(mc_channel_number == 361396) wei= mc_gen_weight*3.2*2.2023*pow(10,6)*0.77817/60313.4;
        else if(mc_channel_number == 361397) wei= mc_gen_weight*3.2*2.205*pow(10,6)*0.14183/30322.2;
        else if(mc_channel_number == 361398) wei= mc_gen_weight*3.2*2.2044*pow(10,6)*0.078491/39732.5;
        else  wei = 0;

    }
    float totalw=wei;
    //float puw = 0;
    if(!data){
         totalw *= mc_pu_weight;
         //puw = mc_pu_weight;
    }
    float ph_weight=0;
    if (data) ph_weight=1;
    else      ph_weight=totalw;
    for(int i=0;i<Config::NetBins;i++)
    {
      for(int j=0;j<Config::NetaBins;j++)
      {
        if( fabs(ph_eta) > Config::etaBins[j] && fabs(ph_eta) < Config::etaBins[j+1] && ph_pt/1000 > Config::etBins[i] && ph_pt/1000 < Config::etBins[i+1])
        {
          int count=0;
          for(int var = 0; var< Config::Nvars + 1 ; var++) 
          {
            string inputvarname = (noFF)  ? Config::InputVarsNoFFNTUP[var] : Config::InputVarsNTUP[var] ;
            string rhadname =     (noFF)  ? "ph_noFF_rhad"  : "ph_rhad";
            string rhad1name =     (noFF) ? "ph_noFF_rhad1" : "ph_rhad1";
            float value = inputVars[inputvarname];
            if (inputvarname == rhad1name) continue;  // special case for rhad1, combine with rhad
            if ( inputvarname==rhadname && (fabs(ph_eta) < 0.8 || fabs(ph_eta)>1.37) )
            {
              value=inputVars[rhad1name];
            }
            //cout << "value: " << value << "  weight: " << ph_weight << endl;
            if (conv_photon)   hists_conv[count][i][j]->Fill(value,ph_weight);
            if (unconv_photon) hists_unconv[count][i][j]->Fill(value,ph_weight);
            count++;
          }
        }

      }

    }


    return true;
}


void BinnedHistGetterNTUP::makeHistos(){
for(int var = 0; var < Config::Nvars; var++)
  {
    vector<vector<TH1F * > > VarHists_c;
    vector<vector<TH1F * > > VarHists_u;
    for(int etBin =0;etBin<Config::NetBins;etBin++)
    {
      vector<TH1F * > VarEtHists_c;
      vector<TH1F * > VarEtHists_u;
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        string histNameBase =Config::varsN[var]+"_"+Config::etNames[etBin]+Config::etaNames[etaBin];
        string histName_conv   = histNameBase + "_c";
        string histName_unconv = histNameBase + "_u";
        VarEtHists_c.push_back(new TH1F(histName_conv.c_str()  ,histName_conv.c_str()  ,Config::Nbins,Config::xmins[var],Config::xmaxs[var])); 
        VarEtHists_u.push_back(new TH1F(histName_unconv.c_str(),histName_unconv.c_str(),Config::Nbins,Config::xmins[var],Config::xmaxs[var])); 
      }
      VarHists_c.push_back(VarEtHists_c);
      VarHists_u.push_back(VarEtHists_u);
    }
    hists_conv.push_back(VarHists_c);
    hists_unconv.push_back(VarHists_u);
  }
}

void BinnedHistGetterNTUP::LoopOverFile(){
  cout << "Looping over file: " << inputFileName << endl;
  long nentries = (long)ttree->GetEntries();
  progress_bar bar(100);
  for (Int_t i=0;i<nentries;i++) {

    ttree->GetEntry(i);
    Cut();
    if(i % (nentries/100) == 0 ) bar.tick();
  }
  bar.end();
}
void BinnedHistGetterNTUP::writeHistsToFiles()
{
  string datatype="";
  if(data) datatype="data";
  else     datatype="mc";
  string filename = "output/test/hists_" + datatype + "_c"  + ".root";
  TFile f(filename.c_str(),"recreate");
  for(int var =0 ; var < Config::Nvars; var++ )
  {
      for(int etBin =0; etBin < Config::NetBins ; etBin++)
      {
          for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
          {
              hists_conv[var][etBin][etaBin]->Write();
          }
      }
  }
  cout << "conv binned "<< datatype << " hists written to file: " << filename << endl;
  f.Close();
  filename = "output/test/hists_" + datatype + "_u"  + ".root";
  TFile g(filename.c_str(),"recreate");
  for(int var =0 ; var < Config::Nvars; var++ )
  {
      for(int etBin =0; etBin < Config::NetBins ; etBin++)
      {
          for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
          {
              hists_unconv[var][etBin][etaBin]->Write();
          }
      }
  }
  cout << "unconv binned "<< datatype << " hists written to file: " << filename << endl;
  g.Close();
}
