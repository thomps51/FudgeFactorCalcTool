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
  BinnedHistGetter mc_conv(Config::inputPskimmedPathMCconv.c_str(),"mc");
  vector<vector<vector<TH1F * > > > mcConvHists = mc_conv.getHists();
  cout << "got conv MC hists" << endl << endl;
  cout << "making MC PDFs...." << endl<< endl;
  PDFgetter mc_conv_PDFs_obj(mcConvHists,"c","mc");
  vector<vector<vector<TH1 * > > > mc_conv_PDFs = mc_conv_PDFs_obj.makeSmoothedPDFs(); 
  mc_conv_PDFs_obj.writeToFile();


  cout << "making converted data binned histograms" << endl;
  BinnedHistGetter data_conv(Config::inputPskimmedPathDataConv.c_str(),"data");
  vector<vector<vector<TH1F * > > > dataConvHists = data_conv.getHists();
  cout << "got conv Data hists" << endl;
  cout << "making conv Data PDFs...." << endl<< endl;
  PDFgetter data_conv_PDFs_obj(dataConvHists,"c","data");
  vector<vector<vector<TH1 * > > > data_conv_PDFs = data_conv_PDFs_obj.makeSmoothedPDFs(); 
  data_conv_PDFs_obj.writeToFile(); 
  cout << "making fudge factors for converted photons...." << endl<< endl;

  FFcalc FFobj_c(data_conv_PDFs,mc_conv_PDFs,"c"); 
  //FFcalc FFobj("output/pdfs_data_c.root","output/pdfs_mc_c.root","c"); 
  FFobj_c.Run(); 
  cout << "Converted Fudge Factors are made, check output/ folder" << endl<< endl;
  
  //UNCONVERTED
  // make binned histograms in et and eta of the MC converted photons
  cout << "making unconverted MC binned histograms" << endl;

  BinnedHistGetter mc_unconv(Config::inputPskimmedPathMCunconv.c_str(),"mc");
  vector<vector<vector<TH1F * > > > mcUnconvHists = mc_unconv.getHists();
  
  cout << "got unconv MC hists" << endl << endl;
  cout << "making unconv MC PDFs...." << endl<< endl;
  
  PDFgetter mc_unconv_PDFs_obj(mcUnconvHists,"u","mc");
  vector<vector<vector<TH1 * > > > mc_unconv_PDFs = mc_unconv_PDFs_obj.makeSmoothedPDFs(); 
  mc_unconv_PDFs_obj.writeToFile(); 
  
  cout << "getting unconv data hists" << endl << endl;
  BinnedHistGetter data_unconv(Config::inputPskimmedPathDataUnconv.c_str(),"data");
  vector<vector<vector<TH1F * > > > dataUnconvHists = data_unconv.getHists();
  
  cout << "got unconv Data hists" << endl;
  cout << "making unconv Data PDFs...." << endl<< endl;
  
  PDFgetter data_unconv_PDFs_obj(dataUnconvHists,"u","data");
  vector<vector<vector<TH1 * > > > data_unconv_PDFs = data_unconv_PDFs_obj.makeSmoothedPDFs(); 
  data_unconv_PDFs_obj.writeToFile(); 
  
  cout << "making fudge factors for unconverted photons...." << endl;

  FFcalc FFobj_u(data_unconv_PDFs,mc_unconv_PDFs,"u"); 
  //FFcalc FFobj("output/pdfs_data_c.root","output/pdfs_mc_c.root","c"); 
  FFobj_u.Run(); 
  cout << "Unconverted Fudge Factors are made, check output/ folder" << endl;

}
