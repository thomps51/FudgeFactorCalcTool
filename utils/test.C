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
  //CONVERTED
  // make binned histograms in et and eta of the MC converted photons
  cout << "making converted MC binned histograms" << endl;
  BinnedHistGetter mc_conv(Config::inputPskimmedPathMCconv.c_str(),"c","mc");
  vector<vector<vector<TH1F * > > > mcConvHists = mc_conv.getHists();
  cout << "got conv MC hists" << endl << endl;
  cout << "making MC PDFs...." << endl<< endl;
  PDFgetter mc_conv_PDFs_obj(mcConvHists,"c","mc");
  vector<vector<vector<TH1 * > > > mc_conv_PDFs = mc_conv_PDFs_obj.makeSmoothedPDFsParallel(); 
  //mc_conv_PDFs_obj.writeToFile();

  return 0;

}
