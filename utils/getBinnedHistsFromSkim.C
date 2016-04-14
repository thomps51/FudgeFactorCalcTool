#include "include/BinnedHistGetter.h"
#include "include/Config.h"
#include "getopt.h"
using namespace std;


int main(int argc, char **argv)
{
  cout << "making converted MC binned histograms" << endl;
  BinnedHistGetter mc_conv(Config::skimOutputDir+"/"+ Config::skimMCconvFile,"c","mc");
  mc_conv.Run();


  cout << "got conv MC hists" << endl << endl;
  cout << "making unconverted MC binned histograms" << endl;
  BinnedHistGetter mc_unconv(Config::skimOutputDir+"/"+Config::skimMCunconvFile.c_str(),"u","mc");
  mc_unconv.Run();

  cout << "making converted data binned histograms" << endl;
  BinnedHistGetter data_conv(Config::skimOutputDir+"/"+Config::skimDataConvFile.c_str(),"c","data");
  data_conv.Run();
  cout << "got conv Data hists" << endl;
  cout << "making unconverted data binned histograms" << endl;
  BinnedHistGetter data_unconv(Config::skimOutputDir+"/"+Config::skimDataUnconvFile.c_str(),"u","data");
  
  data_unconv.Run();
  cout << "got unconv Data hists" << endl;
}
