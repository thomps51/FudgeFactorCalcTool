#include "include/Config.h"
#include "include/NTUPskimmer.h"
using namespace std;


int main(int argc, char **argv)
{
  cout << "making skimmed mc file..." << endl;
  NTUPskimmer skim(Config::inputDumpedNTUPpathMC,"mc",Config::use_noFF_vars);
  skim.Run();
  
  cout << "making skimmed data file..." << endl;
  NTUPskimmer skimD(Config::inputDumpedNTUPpathData,"data",false);
  skimD.Run();

  return 0;
}
