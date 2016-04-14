#include "getopt.h"
#include "include/FFcalc.h"
using namespace std;


int main(int argc, char **argv)
{
  string pdfs_data_c = Config::pdfsOutputDir+"/"+Config::pdfsDataConvFile;
  string pdfs_mc_c   = Config::pdfsOutputDir+"/"+ Config::pdfsMCconvFile;
  FFcalc * FFobj_c = new FFcalc(pdfs_data_c,pdfs_mc_c,"c");
  FFobj_c->Run();

  string pdfs_data_u = Config::pdfsOutputDir+"/"+Config::pdfsDataUnconvFile;
  string pdfs_mc_u   = Config::pdfsOutputDir+"/"+ Config::pdfsMCunconvFile;
  FFcalc * FFobj_u = new FFcalc(pdfs_data_u,pdfs_mc_u,"u");
  FFobj_u->Run();
}
