#include "getopt.h"
#include "include/PDFgetter.h"
#include "include/Config.h"
using namespace std;


int main(int argc, char **argv)
{
  cout << "making data conv PDFs" << endl;
  string hists_data_c = Config::histOutputDir + "/" + Config::histDataConvFile; 
  PDFgetter data_conv_PDFs_obj(hists_data_c,"c","data");
  data_conv_PDFs_obj.Run();

  cout << "making data unconv PDFs" << endl;
  string hists_data_u = Config::histOutputDir + "/" + Config::histDataUnconvFile;
  PDFgetter data_unconv_PDFs_obj(hists_data_u,"u","data");
  data_unconv_PDFs_obj.Run();
  
  cout << "making mc conv PDFs" << endl;
  string hists_mc_c = Config::histOutputDir + "/" +Config::histMCconvFile;
  PDFgetter mc_conv_PDFs_obj(hists_mc_c,"c","mc");
  mc_conv_PDFs_obj.Run();

  cout << "making mc unconv PDFs" << endl;
  string hists_mc_u = Config::histOutputDir + "/" +Config::histMCunconvFile;
  PDFgetter mc_unconv_PDFs_obj(hists_mc_u,"u","mc");
  mc_unconv_PDFs_obj.Run();
}
