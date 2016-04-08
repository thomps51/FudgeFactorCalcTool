#include "include/Config.h"
#include <string>
using namespace std;
// DumpedNTUPs are from the egammaAODtoNtupleDumper, take a look at Root/BinnedHistGetterNTUP.C to change cuts
string Config::inputDumpedNTUPpathData     = "/disk/userdata00/user/athomps/FFnewDump/rawFiles/full2015PhotonData.root"; 
string Config::inputDumpedNTUPpathMC       = "/disk/userdata00/atlas_data2/mistryk/photonid/raw/25ns/mc15b/gamjet.root";
bool   Config::use_noFF_vars               = true;  // choose whether to use noFF variables in dumped MC file 
int     Config::NvarsInputNTUP             = 8;       // number of variables in input ntups, if >Nvars then you must add combining case to BinnedHistGetterNTUP.C  rhad1
string Config::InputVarsNTUP[]             = {"ph_reta" ,"ph_rphi" ,"ph_weta2" ,"ph_w1" ,"ph_wstot" ,"ph_fside" ,"ph_rhad" , "ph_rhad1"};  // changing this may break BinnedHistGetter, be careful
string Config::InputVarsNoFFNTUP[]         = {"ph_noFF_reta" ,"ph_noFF_rphi" ,"ph_noFF_weta2" ,"ph_noFF_w1" ,"ph_noFF_wstot" ,"ph_noFF_fside" ,"ph_noFF_rhad","ph_noFF_rhad1" };  // changing this may break BinnedHistGetter, be careful

// Pskimmed files are pre-cut, pre- conv/unconv selected, ntuples with vectors of length 1.  Holdover from old pskim method used for egammacore 
string Config::inputPskimmedPathDataConv   = "/disk/userdata00/atlas_data2/mistryk/photonid/tonytest/data_looseiso_tightid_conv.root";
string Config::inputPskimmedPathDataUnconv = "/disk/userdata00/atlas_data2/mistryk/photonid/tonytest/data_looseiso_tightid_unconv.root";
string Config::inputPskimmedPathMCconv     = "/disk/userdata00/atlas_data2/mistryk/photonid/tonytest/mc_looseiso_tightId_conv.root";
string Config::inputPskimmedPathMCunconv   = "/disk/userdata00/atlas_data2/mistryk/photonid/tonytest/mc_looseiso_tightId_unconv.root";


int Config::NetBins                = 10;      // number of bins in Et/Pt
int Config::NetaBins               = 8;       // number of bins in eta
int Config::Nvars                  = 7;       // number of variables, changing this may break BinnedHistGetter, be careful
int Config::Nbins                  = 500;     // number of bins for binned histograms and pdfs

std::string Config::varsN[]        = {"reta" ,"rphi" ,"weta2" ,"w1" ,"wstot" ,"fside" ,"rhad" };  // changing this may break BinnedHistGetter, be careful
float Config::xmins[]              = {0.85   ,0.5    ,0.006   ,0.4  ,0.25    ,-0.1    ,-0.04  };  // xmins for binned hists and pdfs 
float Config::xmaxs[]              = {1.15   ,1.05   ,0.014   ,0.9  ,4.0     , 0.9    , 0.04  };  // xmaxs for binned hists and pdfs

std::string Config::etNames[]      = {"et8", "et15", "et20","et25","et30","et40","et50","et60","et80","et100"};  // et bin names, used to name binned histos/pdfs
std::string Config::etaNames[]     = {"eta0.00","eta0.60","eta0.80","eta1.15","eta1.37","eta1.52","eta1.81","eta2.01"}; //eta bin names, used to name binned histos/pdfs
int   Config::etBins[]             = {8,15,20,25,30,40,50,60,80,100,1000};             // beginning/end of all etBins, size (NetBins+1), used for binning
float Config::etaBins[]            = {0.00,0.60,0.80,1.15,1.37,1.52,1.81,2.01,2.37};   // beginning/end of all etaBins size (NetaBins+1), used for binning
