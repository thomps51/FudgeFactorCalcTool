#include "include/BinnedHistGetterNTUP.h"
#include "include/Config.h"
#include "getopt.h"
using namespace std;


int main(int argc, char **argv)
{
  cout << "making data binned histograms" << endl;
  BinnedHistGetterNTUP data(Config::inputDumpedNTUPpathData.c_str(),"data",false);
  data.Run();

  cout << "making mc binned histograms" << endl;
  BinnedHistGetterNTUP mc(Config::inputDumpedNTUPpathMC.c_str(),"mc",Config::use_noFF_vars);
  mc.Run();
}
