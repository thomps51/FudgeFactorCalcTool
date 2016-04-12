#ifndef CONFIG_H
#define CONFIG_H
#include <string>
using namespace std;
class Config{
  public:
// INPUT TYPE 1: NTUPLES from egammaAODtoNtupleDumper
    static string inputDumpedNTUPpathData    ;    
    static string inputDumpedNTUPpathMC      ;   
    static bool   use_noFF_vars;
    static int    NvarsInputNTUP;
    static string InputVarsNTUP [];
    static string InputVarsNoFFNTUP [];

// INPUT TYPE 2: "Pskimmed files"
    static string inputPskimmedPathDataConv  ;  
    static string inputPskimmedPathDataUnconv;
    static string inputPskimmedPathMCconv    ;
    static string inputPskimmedPathMCunconv  ;
    static string InputVarsPskim [];
    static int    NvarsInputPskim;

    // Variables for running
    static int    Nbins;
    static int    shift_max;
    static int    shift_min;
    static string FFsOutputDir;
    static string histOutputDir;
    static string pdfsOutputDir;
    static string varsN[];
    static float  xmins[];
    static float  xmaxs[];
    static float  KDEFineFactors [];
    static string etNames[];
    static string etaNames[];
    static int    etBins[];
    static float  etaBins[];
    static int    NetBins;
    static int    NetaBins;
    static int    Nvars;

};
#endif
