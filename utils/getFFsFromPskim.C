#include "include/BinnedHistGetter.h"
#include "getopt.h"
#include "include/PDFgetter.h"
#include "include/FFcalc.h"
#include "include/Config.h"
#include "TMVA/PDF.h"
using namespace std;


int main(int argc, char **argv)
{
  //CONVERTED
  //make binned histograms in et and eta of the MC converted photons
  cout << "making converted MC binned histograms" << endl;
  BinnedHistGetter mc_conv(Config::inputPskimmedPathMCconv.c_str(),"c","mc");
  mc_conv.Run();
  
  //vector<vector<vector<TH1F * > > > mc_conv_hists = mc_conv.getHists();

  cout << "got conv MC hists" << endl << endl;
  cout << "making MC PDFs...." << endl<< endl;
  
  PDFgetter mc_conv_PDFs_obj("output/hists/hists_mc_c.root","c","mc");
  //PDFgetter mc_conv_PDFs_obj(mc_conv_hists,"c","mc");
  mc_conv_PDFs_obj.Run();

  cout << "making converted data binned histograms" << endl;
  BinnedHistGetter data_conv(Config::inputPskimmedPathDataConv.c_str(),"c","data");
  data_conv.Run();
  cout << "got conv Data hists" << endl;
  cout << "making conv Data PDFs...." << endl<< endl;
  
  PDFgetter data_conv_PDFs_obj("output/hists/hists_data_c.root","c","data");
  data_conv_PDFs_obj.Run();
  cout << "making fudge factors for converted photons...." << endl<< endl;

  FFcalc FFobj_c("output/pdfs/pdfs_data_c.root","output/pdfs/pdfs_mc_c.root","c"); 
  FFobj_c.Run(); 
  cout << "Converted Fudge Factors are made, check output/ folder" << endl<< endl;
  
  //UNCONVERTED
  // make binned histograms in et and eta of the MC converted photons
  cout << "making unconverted MC binned histograms" << endl;

  BinnedHistGetter mc_unconv(Config::inputPskimmedPathMCunconv.c_str(),"u","mc");
  mc_unconv.Run();
  
  cout << "got unconv MC hists" << endl << endl;
  cout << "making unconv MC PDFs...." << endl<< endl;
  
  PDFgetter mc_unconv_PDFs_obj("output/hists/hists_mc_u.root","u","mc");
  mc_unconv_PDFs_obj.Run();

  cout << "getting unconv data hists" << endl << endl;
  BinnedHistGetter data_unconv(Config::inputPskimmedPathDataUnconv.c_str(),"u","data");
  data_unconv.Run();

  cout << "got unconv Data hists" << endl;
  cout << "making unconv Data PDFs...." << endl<< endl;
  
  PDFgetter data_unconv_PDFs_obj("output/hists/hists_data_u.root","u","data");
  data_unconv_PDFs_obj.Run();

  cout << "making fudge factors for unconverted photons...." << endl;

  FFcalc FFobj_u("output/pdfs/pdfs_data_u.root","output/pdfs/pdfs_mc_u.root","u"); 
  FFobj_u.Run(); 
  cout << "Unconverted Fudge Factors are made, check output/ folder" << endl;

}
