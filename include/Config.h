#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <sys/stat.h>
using namespace std;
class Config{
  public:
// INPUT: NTUPLES from egammaAODtoNtupleDumper
    static string inputDumpedNTUPpathData    ;    
    static string inputDumpedNTUPpathMC      ;   
    static bool   use_noFF_vars;
    static int    NvarsInputNTUP;
    static string InputVarsNTUP [];
    static string InputVarsNoFFNTUP [];

    static string InputVarsPskim [];
    static int    NvarsInputPskim;

    // Variables for running
    static int    Nbins;
    static int    shift_max;
    static int    shift_min;
    static string skimOutputDir;
    static string skimMCconvFile;     
    static string skimMCunconvFile;   
    static string skimDataConvFile;   
    static string skimDataUnconvFile;
    
    static string FFsOutputDir;
    static string histOutputDir;
    static string histMCconvFile;
    static string histMCunconvFile;
    static string histDataConvFile;
    static string histDataUnconvFile;
    
    static string pdfsOutputDir;
    static string pdfsMCconvFile;
    static string pdfsMCunconvFile;
    static string pdfsDataConvFile;
    static string pdfsDataUnconvFile;
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
    
    static int    mkpath(std::string s,mode_t mode){
      size_t pre=0,pos;
      std::string dir;
      int mdret=0;

      if(s[s.size()-1]!='/'){
          // force trailing / so we can handle everything in loop
          s+='/';
      }

      while((pos=s.find_first_of('/',pre))!=std::string::npos){
          dir=s.substr(0,pos++);
          pre=pos;
          if(dir.size()==0) continue; // if leading / first time is 0 length
          if((mdret=mkdir(dir.c_str(),mode)) && errno!=EEXIST){
              return mdret;
          }
      }
      return mdret;

    }
};
#endif
