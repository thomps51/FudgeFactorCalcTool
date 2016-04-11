#include "include/Config.h"
#include "include/PDFgetter.h"
#include "TMVA/PDF.h"
#include <thread>
#include <unistd.h>
#include <ctime>
#include "include/progress_bar.h"

PDFgetter::PDFgetter(vector<vector<vector<TH1F *> > > &Hists, string convSt, string data_type)
{
    hists      = Hists;
    convStatus = convSt;
    if (convSt != "u" && convSt != "c")
    {
      cerr << "PDFgetter::convStatus not set to c or u, please check arguments!" << endl;
    }
    datatype   = data_type;
    if (datatype != "data" && datatype != "mc")
    {
      cerr << "PDFgetter::datatype not set to data or mc, please check arguments!" << endl;
    }
}
PDFgetter::PDFgetter(string inputfilename, string convSt, string data_type)
{
  convStatus = convSt;
  hists=getHistsFromFile(inputfilename);
  datatype   = data_type;
}

vector<vector<vector<TH1F *> > > PDFgetter::getHistsFromFile(string filename)
{
  TFile * f = new TFile(filename.c_str());
  vector<vector<vector<TH1F * > > > Hists;
  for(int var = 0;var<Config::Nvars;var++)
  {
    vector<vector<TH1F * > > VarHists;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      vector<TH1F *> VarEtHists;
      for(int etaBin=0;etaBin<Config::NetaBins;etaBin++)
      {
        
        string histName=Config::varsN[var] + "_" + Config::etNames[etBin]+Config::etaNames[etaBin]+"_"+convStatus;
        TH1F * hist = (TH1F *) f->Get(histName.c_str());
        if(hist == 0){
          cout << "Error! Hist " << histName << " not found in file: " << filename<< endl;
        }
        VarEtHists.push_back(hist);
      }
      VarHists.push_back(VarEtHists);
    }
    Hists.push_back(VarHists);
  }
  return Hists;
}
float getScaleFactor(TH1F * href, TH1* h)
{
    float Denom = h->Integral()*href->GetXaxis()->GetBinWidth(1);
    if (Denom == 0) return 1;
    return href->Integral()*h->GetXaxis()->GetBinWidth(1)/Denom;
}
vector<vector<vector<TH1 *> > > PDFgetter::makeSmoothedPDFs()
{
    string convString;
    if(convStatus=="u") convString = "unconverted";
    else            convString = "converted";
    cout << "making smoothed PDFs for " << convString << " "<< datatype << endl;
    pdfs= vector<vector<vector<TH1 *> > >(Config::Nvars);
    std::ofstream lStream( "output/TMVA_PDF_output.txt" );
    std::streambuf* lBufferOld = std::cout.rdbuf();
    
    
    progress_bar bar(Config::Nvars * Config::NetBins); 
    for(int var =0 ; var < Config::Nvars; var++ )
    {
        vector<vector<TH1 * > > VarPDFs(Config::NetBins);
        float kdeFF = Config::KDEFineFactors[var];
        for(int etBin =0; etBin < Config::NetBins ; etBin++)
        {
          vector<TH1 * > VarEtPDFs(Config::NetaBins);
          for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
          {
              TH1F * currentHisto = hists[var][etBin][etaBin];
              
              std::cout.rdbuf( lStream.rdbuf() );  // this is just to supress the output of the PDF maker
              TMVA::PDF * pdf = new TMVA::PDF("test"+Config::varsN[var],currentHisto,TMVA::KDEKernel::kGauss,TMVA::KDEKernel::kAdaptiveKDE,TMVA::KDEKernel::kSampleMirror,kdeFF,0);
              std::cout.rdbuf( lBufferOld );
              //pdf->ValidatePDF(0);
              TH1* KDEsmoothed = pdf->GetPDFHist(); 
              KDEsmoothed->Rebin(10000/Config::Nbins);
              float SF = getScaleFactor(currentHisto,KDEsmoothed);
              KDEsmoothed->Scale(SF);
              VarEtPDFs[etaBin] = KDEsmoothed;
          }
          //VarPDFs.push_back(VarEtPDFs);
          VarPDFs[etBin]=VarEtPDFs;
          bar.tick();
        }
        pdfs[var]=VarPDFs;
    }
    bar.end();
    return pdfs;
}
void PDFgetter::getPDFsForVar(unsigned var)
{
  // start with offset
  usleep(var*100000); // hack for parallel running, the damn TMVA::PDF creation stdouts happening at the same time cause segfaults

  vector<vector<TH1 * > > VarPDFs(Config::NetBins);
  float kdeFF = Config::KDEFineFactors[var];
  for(int etBin =0; etBin < Config::NetBins ; etBin++)
  {
    
    vector<TH1 * > VarEtPDFs(Config::NetaBins);
    for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
    {
        TH1F * currentHisto = hists[var][etBin][etaBin];
        
        TMVA::PDF * pdf = new TMVA::PDF("test"+Config::varsN[var],currentHisto,TMVA::KDEKernel::kGauss,TMVA::KDEKernel::kAdaptiveKDE,TMVA::KDEKernel::kSampleMirror,kdeFF,0);
        TH1* KDEsmoothed = pdf->GetPDFHist(); 
        KDEsmoothed->Rebin(10000/Config::Nbins);
        float SF = getScaleFactor(currentHisto,KDEsmoothed);
        KDEsmoothed->Scale(SF);
        VarEtPDFs[etaBin] = KDEsmoothed;
    }
    VarPDFs[etBin]=VarEtPDFs;
  }
  pdfs[var]=VarPDFs;
}

vector<vector<vector<TH1 *> > > PDFgetter::makeSmoothedPDFsParallel()
{
  string convString;
  if(convStatus=="u") convString = "unconverted";
  else            convString = "converted";
  cout << "making smoothed PDFs for " << convString << " "<< datatype << endl;
  pdfs= vector<vector<vector<TH1 *> > >(Config::Nvars);
  
streambuf *old = cout.rdbuf();
  time_t rawtime = time(0);
  char timestamp[16];
  struct tm *now = localtime(&rawtime);
  strftime(timestamp,16,"%y%m%d_%H%M%S",now);
  string outputLogName = "output/TMVA_log/TMVA_PDF_output.txt";
  std::ofstream lStream( outputLogName );
  cout << "threads being created, waiting finish.  Check output/TMVA_log/ if there are problems" << endl;
  std::cout.rdbuf( lStream.rdbuf() );  // this is just to supress the output of the PDF maker
  std::vector<std::thread> threads(Config::Nvars);
  for(int i = 0; i < Config::Nvars; i++)
  {
    threads[i]=std::thread(&PDFgetter::getPDFsForVar,this,i);
  }
  for(int i = 0; i < Config::Nvars; i++)
  {
    threads[i].join();
  }
  cout.rdbuf (old);
  cout << "pdf creation done" << endl;
  return pdfs; 
    
}
void PDFgetter::writeToFile()
{
     
    string filename = "output/pdfs/pdfs_" + datatype + "_" + convStatus + ".root";
    TFile f(filename.c_str(),"recreate");
    for(int var =0 ; var < Config::Nvars; var++ )
    {
        for(int etBin =0; etBin < Config::NetBins ; etBin++)
        {
            for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
            {
                pdfs[var][etBin][etaBin]->Write();
            }
        }
    }
    string convName = "";
    if (convStatus == "u") convName="unconv";
    else                   convName="conv";
    cout << convName  << " " << datatype <<" PDFs file written to " << filename << endl;
    filename = "output/hists/hists_" + datatype + "_" + convStatus + ".root";
    TFile g(filename.c_str(),"recreate");
    for(int var =0 ; var < Config::Nvars; var++ )
    {
        for(int etBin =0; etBin < Config::NetBins ; etBin++)
        {
            for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
            {
                hists[var][etBin][etaBin]->Write();
            }
        }
    }
   

}

