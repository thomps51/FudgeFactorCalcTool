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
  BinnedHistGetter mc_conv(Config::skimOutputDir+"/"+Config::skimMCconvFile,"c","mc");
  mc_conv.Run();
 
  cout << "got conv MC hists" << endl << endl;
  cout << "making MC PDFs...." << endl<< endl;
  
  PDFgetter mc_conv_PDFs_obj(Config::histOutputDir + "/" +Config::histMCconvFile,"c","mc");
  mc_conv_PDFs_obj.Run();

  cout << "making converted data binned histograms" << endl;
  BinnedHistGetter data_conv(Config::skimOutputDir+"/"+Config::skimDataConvFile,"c","data");
  data_conv.Run();
  cout << "got conv Data hists" << endl;
  cout << "making conv Data PDFs...." << endl<< endl;
  
  PDFgetter data_conv_PDFs_obj(Config::histOutputDir + "/" +Config::histDataConvFile,"c","data");
  data_conv_PDFs_obj.Run();
  cout << "making fudge factors for converted photons...." << endl<< endl;

  FFcalc FFobj_c(Config::pdfsOutputDir+"/"+Config::pdfsDataConvFile,Config::pdfsOutputDir+"/"+Config::pdfsMCconvFile,"c"); 
  FFobj_c.Run(); 
  cout << "Converted Fudge Factors are made, check output/ folder" << endl<< endl;
  
  //UNCONVERTED
  // make binned histograms in et and eta of the MC converted photons
  cout << "making unconverted MC binned histograms" << endl;

  BinnedHistGetter mc_unconv(Config::skimOutputDir+"/"+Config::skimMCunconvFile,"u","mc");
  mc_unconv.Run();
  
  cout << "got unconv MC hists" << endl << endl;
  cout << "making unconv MC PDFs...." << endl<< endl;
  
  PDFgetter mc_unconv_PDFs_obj(Config::histOutputDir + "/" +Config::histMCunconvFile,"u","mc");
  mc_unconv_PDFs_obj.Run();

  cout << "getting unconv data hists" << endl << endl;
  BinnedHistGetter data_unconv(Config::skimOutputDir+"/"+Config::skimDataUnconvFile,"u","data");
  data_unconv.Run();

  cout << "got unconv Data hists" << endl;
  cout << "making unconv Data PDFs...." << endl<< endl;
  
  PDFgetter data_unconv_PDFs_obj(Config::histOutputDir + "/" +Config::histDataUnconvFile,"u","data");
  data_unconv_PDFs_obj.Run();

  cout << "making fudge factors for unconverted photons...." << endl;

  FFcalc FFobj_u(Config::pdfsOutputDir+"/"+Config::pdfsDataUnconvFile,Config::pdfsOutputDir+"/"+Config::pdfsMCunconvFile,"u"); 
  FFobj_u.Run(); 
  cout << "Unconverted Fudge Factors are made, check output/ folder" << endl;

}
