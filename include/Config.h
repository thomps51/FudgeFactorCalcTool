#include <string>

class Config{
  public:
    static std::string inputDumpedNTUPpathData    ;    
    static std::string inputDumpedNTUPpathMC      ;   
    static bool use_noFF_vars;
    static std::string inputPskimmedPathDataConv  ;  
    static std::string inputPskimmedPathDataUnconv;
    static std::string inputPskimmedPathMCconv    ;
    static std::string inputPskimmedPathMCunconv  ;
    static int NvarsInputPskim;
    static std::string InputVarsPskim [];


    static int NvarsInputNTUP;
    static std::string InputVarsNTUP [];
    static std::string InputVarsNoFFNTUP [];

    static float KDEFineFactors [];
    static int NetBins;
    static int NetaBins;
    static int Nvars;
    static std::string etNames[];
    static std::string etaNames[];
    static std::string varsN[];
    static std::string  InputVars[];
    static float xmins[];
    static float xmaxs[];
    static int Nbins;
    static int etBins[];
    static float etaBins[];

};
