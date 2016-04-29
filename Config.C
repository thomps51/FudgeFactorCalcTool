#include "include/Config.h"
// MUST RE-MAKE AFTER CHANGING!!!!!!!!!

// INPUT: NTUPLES from egammaAODtoNtupleDumper, use by running ./getBinnedHistsFromNTUP, then ./getFFsFromBinnedHists
// DumpedNTUPs are from the egammaAODtoNtupleDumper, take a look at Root/BinnedHistGetterNTUP.C Cut() function to change cuts
string Config::inputDumpedNTUPpathData = "/disk/userdata00/user/athomps/FFnewDump/rawFiles/full2015PhotonData.root"; 
string Config::inputDumpedNTUPpathMC   = "/disk/userdata00/atlas_data2/mistryk/photonid/raw/25ns/mc15b/gamjet.root";
bool   Config::use_noFF_vars           = true;  // choose whether to use noFF variables in dumped MC file 
string Config::InputVarsNTUP[]         = {"ph_reta"     ,"ph_rphi"     ,"ph_weta2"     ,"ph_w1"     ,"ph_wstot"     ,"ph_fside"     ,"ph_rhad"     ,"ph_rhad1"     }; 
string Config::InputVarsNoFFNTUP[]     = {"ph_noFF_reta","ph_noFF_rphi","ph_noFF_weta2","ph_noFF_w1","ph_noFF_wstot","ph_noFF_fside","ph_noFF_rhad","ph_noFF_rhad1"}; 
int    Config::NvarsInputNTUP          = sizeof(InputVarsNTUP) /sizeof(*InputVarsNTUP);
// NOTE: ph_rhad and ph_rhad1 are hard-coded to be combined in BinnedHistGetterNTUP.C (if they are in the list above)

// The Following are used to make the skimmed files and later to make the binned histograms.  Variables must be in same order as above in NTUP vars!!!!
string Config::InputVarsPskim[]            = {"ph_reta" ,"ph_rphi" ,"ph_weta2" ,"ph_w1" ,"ph_wstot" ,"ph_fside" ,"ph_rhad" };
int    Config::NvarsInputPskim             = sizeof(InputVarsPskim) /sizeof(*InputVarsPskim);


// SET THE FOLLOWING BEFORE RUNNING
int    Config::Nbins            = 500;             // number of bins for binned histograms and pdfs
int    Config::shift_min        = -60;             // max number of bins to shift to the left during chi-sq calculation of FFs, affects running time
int    Config::shift_max        =  60;             // max number of bins to shift to the right during chi-sq calculation of FFs

// the following files are made in getSkimFromNTUP and used in getBinnedHistsFromSkim
string Config::skimOutputDir      = "output/skimmedNtups"; // folder will be created at runtime if it doesn't exist
string Config::skimMCconvFile     = "mc_c.root";  // will be inside of the above folder
string Config::skimMCunconvFile   = "mc_u.root";
string Config::skimDataConvFile   = "data_c.root";
string Config::skimDataUnconvFile = "data_u.root";

// the following files are made in getBinnedHistsFromSkim and used in getPDFsFromBinnedHists
string Config::histOutputDir      = "output/hists";  
string Config::histMCconvFile     = "hists_mc_c.root";
string Config::histMCunconvFile   = "hists_mc_u.root";
string Config::histDataConvFile   = "hists_data_c.root";
string Config::histDataUnconvFile = "hists_data_u.root";

// the following files are made in getPDFsFromBinnedHists and used in getFFsFromPDFs
string Config::pdfsOutputDir      = "output/pdfs" ;  
string Config::pdfsMCconvFile     = "pdfs_mc_c.root";
string Config::pdfsMCunconvFile   = "pdfs_mc_u.root";
string Config::pdfsDataConvFile   = "pdfs_data_c.root";
string Config::pdfsDataUnconvFile = "pdfs_data_u.root";

string Config::FFsOutputDir       = "output/FudgeFactors"      ;

string Config::varsN[]          = {"reta" ,"rphi" ,"weta2" ,"w1" ,"wstot" ,"fside" ,"rhad" };  // what you want your output variables to be named.  Use same order as InputVarsPskim!!!
float  Config::xmins[]          = {0.85   ,0.5    ,0.006   ,0.4  ,0.25    ,-0.1    ,-0.04  };  // xmins for binned hists and pdfs, KEEP ORDER 
float  Config::xmaxs[]          = {1.15   ,1.05   ,0.014   ,0.9  ,4.0     , 0.9    , 0.04  };  // xmaxs for binned hists and pdfs, KEEP ORDER
float  Config::KDEFineFactors[] = {1.5    ,3      ,15      ,2    ,0.6     , 1.2    , 2     };  // fine factors for KDE smoothing of PDFs, KEEP ORDER

string Config::etNames[]        = {"et8", "et15", "et20","et25","et30","et40","et50","et60","et80","et100"};  // et bin names, used to name binned histos/pdfs
string Config::etaNames[]       = {"eta0.00","eta0.60","eta0.80","eta1.15","eta1.37","eta1.52","eta1.81","eta2.01"}; //eta bin names, used to name binned histos/pdfs
int    Config::etBins[]         = {8,15,20,25,30,40,50,60,80,100,1000};             // beginning/end of all etBins, size (NetBins+1), used for binning
float  Config::etaBins[]        = {0.00,0.60,0.80,1.15,1.37,1.52,1.81,2.01,2.37};   // beginning/end of all etaBins size (NetaBins+1), used for binning

int    Config::NetBins          = (sizeof(etNames) /sizeof(*etNames));    // number of bins in Et/Pt
int    Config::NetaBins         = (sizeof(etaNames)/sizeof(*etaNames));   // number of bins in eta
int    Config::Nvars            = (sizeof(varsN)   /sizeof(*varsN));      // number of variables
