#include "include/BinnedHistGetter.h"
#include "include/BinnedHistGetterNTUP.h"
#include "getopt.h"
#include "include/PDFgetter.h"
#include "include/FFcalc.h"
#include "include/Config.h"
#include "TMVA/PDF.h"
using namespace std;


int main(int argc, char **argv)
{
  cout << "making data conv PDFs" << endl;
  PDFgetter data_conv_PDFs_obj("output/hists/hists_data_c.root","c","data");
  data_conv_PDFs_obj.Run();

  cout << "making data unconv PDFs" << endl;
  PDFgetter data_unconv_PDFs_obj("output/hists/hists_data_u.root","u","data");
  data_unconv_PDFs_obj.Run();
  
  cout << "making mc conv PDFs" << endl;
  PDFgetter mc_conv_PDFs_obj("output/hists/hists_mc_c.root","c","mc");
  mc_conv_PDFs_obj.Run();

  cout << "making mc unconv PDFs" << endl;
  PDFgetter mc_unconv_PDFs_obj("output/hists/hists_mc_u.root","u","mc");
  mc_unconv_PDFs_obj.Run();


  
  cout << "making conv FFs" << endl;
  FFcalc FFobj_c("output/pdfs/pdfs_data_c.root","output/pdfs/pdfs_mc_c.root","c");
  FFobj_c.Run();
  
  cout << "making unconv FFs" << endl;
  FFcalc FFobj_u("output/pdfs/pdfs_data_u.root","output/pdfs/pdfs_mc_u.root","u");
  FFobj_u.Run();
}
