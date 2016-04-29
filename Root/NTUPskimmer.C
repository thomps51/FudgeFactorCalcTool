#include "include/NTUPskimmer.h"
//#include "include/commonFunctions.h"



/*
 * Constructor for NTUPskimmer.  Opens input file for reading, intializes the variables to
 * be read from the input file and written to the output file.  Creates output files and
 * TTrees.
 *
 * Uses the noFF variables defined in Config.C if passed true for useNoFFvariables
 */

NTUPskimmer::NTUPskimmer(string inputFile, string inputFileType, bool useNoFFvariables)
{
  ntupFile = TFile::Open(inputFile.c_str());
  inputFileName=ntupFile->GetName();
  inputType=inputFileType;
  if (inputFileType == "data")  data=true;
  else                          data=false;
  if (useNoFFvariables) noFF=true;
  else                  noFF=false;
  for(int i =0; i< Config::NvarsInputNTUP ; i++)
  {
    if (noFF) inputVars[Config::InputVarsNoFFNTUP[i]] = 0;
    else      inputVars[Config::InputVarsNTUP[i]]     = 0;
  }
  for(int i =0; i< Config::NvarsInputPskim ; i++)
  {
    outputVars[Config::InputVarsPskim[i]] = 0;
  }
  inputTree = (TTree*)ntupFile->Get("photon");
 
  Config::mkpath(Config::skimOutputDir,0775);

  string outName_c= Config::skimOutputDir+"/";
  string outName_u= Config::skimOutputDir+"/";
  if(data)
  {
    outName_c += Config::skimDataConvFile;
    outName_u += Config::skimDataUnconvFile;
  }
  else
  {
    outName_c += Config::skimMCconvFile;
    outName_u += Config::skimMCunconvFile;
  }

  outputFile_c = TFile::Open(outName_c.c_str(),"recreate"); 
  outputTree_c = new TTree("tree", "tree");

  outputFile_u = TFile::Open(outName_u.c_str(),"recreate");
  outputTree_u = new TTree("tree", "tree");
}
/*
 * init() : initializes the branch addresses for the input and output trees
 * 
 *
 *
 *
 *
 */

void NTUPskimmer::init()
{
  ph_pt=0;    ph_eta=0;   ph_phi=0; ph_topoetcone40=0; ph_topoetcone30=0;
  ph_topoetcone20=0;      weight=0;         weightnpu=0;       mc_pu_weight=0;
  ph_convFlag=0;
  mc_channel_number=0; ph_tight_mc15=0;
  sumW=0;
  ph_loose_mc15=0; ph_ptcone20=0; mc_gen_weight=0;

  for(int i = 0; i< Config::NvarsInputNTUP ; i++)
  {
    string inputvarname = (noFF) ? Config::InputVarsNoFFNTUP[i] : Config::InputVarsNTUP[i] ;
    inputTree->SetBranchAddress(inputvarname.c_str(), &inputVars[inputvarname] );
  }
  inputTree->SetBranchAddress("ph_pt", &ph_pt);
  inputTree->SetBranchAddress("ph_eta", &ph_eta);
  inputTree->SetBranchAddress("ph_phi", &ph_phi);
  inputTree->SetBranchAddress("ph_topoetcone40", &ph_topoetcone40);
  inputTree->SetBranchAddress("ph_topoetcone30", &ph_topoetcone30);
  inputTree->SetBranchAddress("ph_topoetcone20", &ph_topoetcone20);

  inputTree->SetBranchAddress("ph_convFlag", &ph_convFlag);
  if (! data) {
  inputTree->SetBranchAddress("mc_channel_number", &mc_channel_number);
  inputTree->SetBranchAddress("mc_gen_weight", &mc_gen_weight);
  inputTree->SetBranchAddress("mc_pu_weight", &mc_pu_weight);
  }
  inputTree->SetBranchAddress("ph_tight_mc15", &ph_tight_mc15);
  inputTree->SetBranchAddress("ph_loose_mc15", &ph_loose_mc15);
  inputTree->SetBranchAddress("ph_ptcone20", &ph_ptcone20);

  for(int i = 0; i< Config::NvarsInputPskim ; i++)
  {
    string outputvarname = Config::InputVarsPskim[i] ;
    // the following is dangerous, but we'll only fill one at a time
    outputTree_c->Branch(outputvarname.c_str(), &outputVars[outputvarname] );
    outputTree_u->Branch(outputvarname.c_str(), &outputVars[outputvarname] );
  }
  ph_weight = 0; 
  outputTree_u->Branch("ph_convFlag", &ph_convFlag);
  outputTree_u->Branch("ph_pt", &ph_pt);
  outputTree_u->Branch("ph_eta", &ph_eta);
  outputTree_u->Branch("ph_phi", &ph_phi);
  outputTree_u->Branch("ph_weight", &ph_weight);
  outputTree_c->Branch("ph_convFlag", &ph_convFlag);
  outputTree_c->Branch("ph_pt", &ph_pt);
  outputTree_c->Branch("ph_eta", &ph_eta);
  outputTree_c->Branch("ph_phi", &ph_phi);
  outputTree_c->Branch("ph_weight", &ph_weight);
}
void NTUPskimmer::LoopOverFile()
{
  cout << "Looping over file: " << inputFileName << endl;
  long nentries = (long)inputTree->GetEntries();
  long count =0;
  long Nconv  =0;
  long Nunconv=0;
  progress_bar bar(100);
  for (Int_t i=0;i<nentries;i++) {

    inputTree->GetEntry(i);
    if (Cut() ) 
    {
      count++;
      if(ph_convFlag%10 == 0) Nunconv++;
      if(ph_convFlag%10 != 0) Nconv++;
    }
    if(i % (nentries/100) == 0 ) bar.tick();
    //if( (ph_convFlag%10 == 0) && i ==0) cout <<"YUP"<< endl;
  }
  bar.end();
  cout << "total events passed  : "<< count << endl;
  cout << "conv events passed   : "<< Nconv << endl;
  cout << "unconv events passed : "<< Nunconv << endl;
}
bool NTUPskimmer::Cut(){
    int tight = ph_tight_mc15;   //CHANGE THESE MAYBE
    //int loose = ph_loose_mc15;
    int conv  = ph_convFlag;  ///
    float pt = ph_pt/1000.;
    float looseiso = ph_pt*0.065;
    //float tightiso = pt*0.022  + 2.45; //divide to convert to GeV
    float trackiso = ph_ptcone20/ph_pt;
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
    // hard coded MC weights.....
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
    if(!data){
         totalw *= mc_pu_weight;
    }
    if (data) ph_weight=1;
    else      ph_weight=totalw;
    int count=0;
    for(int var = 0; var< Config::NvarsInputNTUP ; var++)
    {
      string inputvarname = (noFF)  ? Config::InputVarsNoFFNTUP[var] : Config::InputVarsNTUP[var] ;
      string rhadname =     (noFF)  ? "ph_noFF_rhad"  : "ph_rhad";
      string rhad1name =     (noFF) ? "ph_noFF_rhad1" : "ph_rhad1";
      if (inputvarname == rhad1name) continue;  // special case for rhad1, combine with rhad
      
      
      
      // confusing line ahead! It sets the output tree variable to the value in the input tree variable
      // They have different iterators because of the rhad1 case when it is combined with rhad,
      // so there are more input variables than output variables.  
      string outputvarname= Config::InputVarsPskim[count];
      outputVars[outputvarname] =  inputVars[inputvarname];    
      
      
      
      
      if ( inputvarname==rhadname && (fabs(ph_eta) < 0.8 || fabs(ph_eta)>1.37) )
      {
        outputVars[rhadname] =  inputVars[rhad1name];
      }
      count ++;
    }
    if (conv_photon)   outputTree_c->Fill();
    if (unconv_photon) outputTree_u->Fill();
    return true;
}

void NTUPskimmer::Write()
{
  outputFile_c->cd();
  outputTree_c->Write(0,TObject::kOverwrite);
  outputFile_c->Close();

  outputFile_u->cd();
  outputTree_u->Write(0,TObject::kOverwrite);
  outputFile_u->Close();
}
