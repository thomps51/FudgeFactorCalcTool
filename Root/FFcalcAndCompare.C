#include "include/FFcalcAndCompare.h"
#include "include/Config.h"
#include <cfloat>
#include "/home/athomps/FF/ConvToC/newffs_c.c"
#include "/home/athomps/FF/ConvToC/newffs_u.c"

FFcalcAndCompare::FFcalcAndCompare(vector<vector<vector<TH1 *> > > &data_PDFs ,vector<vector<vector<TH1 *> > > &mc_PDFs, string convSt)
{
  dataPDFs   = data_PDFs;
  mcPDFs     = mc_PDFs;
  convStatus = convSt;
}
FFcalcAndCompare::FFcalcAndCompare(string data_PDFs_filename ,string mc_PDFs_filename, string convSt)
{
  convStatus = convSt;
  dataPDFs = getPDFsFromFile(data_PDFs_filename);
  mcPDFs = getPDFsFromFile(mc_PDFs_filename);
}
FFcalcAndCompare::~FFcalcAndCompare()
{
  for(int var = 0;var<Config::Nvars;var++)
  {
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      for(int etaBin=0;etaBin<Config::NetaBins;etaBin++)
      {
        //cout << "test" << endl;
        //delete chiSqPlots[var][etBin][etaBin];
        //delete ;
      }
    }


  }

}
vector<vector<vector<TH1 *> > > FFcalcAndCompare::getPDFsFromFile(string filename)
{
  TFile * f = new TFile(filename.c_str());
  vector<vector<vector<TH1 * > > > PDFs;
  for(int var = 0;var<Config::Nvars;var++)
  {
    vector<vector<TH1 * > > VarPDFs;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      vector<TH1 *> VarEtPDFs;
      for(int etaBin=0;etaBin<Config::NetaBins;etaBin++)
      {
        string histName=Config::varsN[var] + "_" + Config::etNames[etBin]+Config::etaNames[etaBin]+"_"+convStatus+"_smoothed_hist_from_KDE";
        TH1 * hist = (TH1 *) f->Get(histName.c_str());
        if(hist == 0){
          cout << "Error! Hist " << histName << " not found!!!" << endl;

        }
        VarEtPDFs.push_back(hist);
      }
      VarPDFs.push_back(VarEtPDFs);
    }
    PDFs.push_back(VarPDFs);
  }
  return PDFs;
}

float FFcalcAndCompare::getXsq(TH1 * dataHist, TH1* NmcHist, int shift) {
  int lastDataBin=dataHist->GetNbinsX();
  float xSq = 0;
  float dataNormalization = dataHist->Integral(2,lastDataBin-2);
  float mcNormalization =   NmcHist->Integral(2,lastDataBin-2);
  float norm = mcNormalization / dataNormalization;
  for(int bin =3 ; bin<lastDataBin-3;bin++)
  {
    float approxError=sqrt(NmcHist->GetBinContent(bin - shift)+dataHist->GetBinContent(bin) * norm*norm);
    if(approxError == 0) continue;
    xSq = xSq + (dataHist->GetBinContent(bin)*norm - NmcHist->GetBinContent(bin-shift))*(dataHist->GetBinContent(bin) - NmcHist->GetBinContent(bin-shift)) / (approxError*approxError);
  }

  return xSq;
}
float FFcalcAndCompare::GetFFerror(TH1F * chiSqHisto, int shift_min)
{
  int optimalShift = chiSqHisto->GetMinimumBin() + shift_min - 1;
  TF1 *myfit = new TF1("myfit","pol2");
  chiSqHisto->Fit(myfit,"QN","",(optimalShift-5)*chiSqHisto->GetBinWidth(2),(optimalShift+5)*chiSqHisto->GetBinWidth(2));
  Double_t p0 = myfit->GetParameter(0);
  Double_t p1 = myfit->GetParameter(1);
  Double_t p2 = myfit->GetParameter(2);
  Double_t min = -p1/(2*p2);
  Double_t miny = p0+p1*(min)+p2*(min)*(min);
  Double_t xMinPlus= (-p1 + sqrt(p1*p1-4*p2*(p0-(miny+1))) ) / (2 * p2);
  Double_t Error = xMinPlus - min;

  //Double_t trueError = ErrorInBins * dataHist->GetBinWidth(2);
  
  return Error; 
}
FudgeFactor FFcalcAndCompare::GetFF(TH1 * dataHist, TH1 * mcHist, int var, int etBin, int etaBin)
{
  int shift_min = -120;
  int shift_max =  120;

  float mcMax    = mcHist->GetXaxis()->GetXmax();
  float mcMin    = mcHist->GetXaxis()->GetXmin();
  int   mcNbins  = mcHist->GetNbinsX();
  double binWidth = mcHist->GetBinWidth(10);
  
  double xSqPlotMin = shift_min * (mcMax - mcMin) / mcNbins;
  double xSqPlotMax = shift_max * (mcMax - mcMin) / mcNbins;
  string histoName = "Shift_X2_"+Config::varsN[var] +"_"+ Config::etNames[etBin] +"_"+Config::etaNames[etaBin]; 
  TH1F * chiSqHisto = new TH1F(histoName.c_str(),histoName.c_str(), (shift_max - shift_min ) + 1 , xSqPlotMin , xSqPlotMax);
  double minValue = FLT_MAX;
  int minBinShift = -999;
  for(int bin_shift = shift_min; bin_shift <= shift_max ; bin_shift++)
  {
    double Xsq = getXsq(dataHist, mcHist, bin_shift);
    chiSqHisto->SetBinContent( bin_shift - shift_min + 1 , Xsq);
    if(Xsq < minValue)
    {
      minValue    = Xsq;
      minBinShift = bin_shift;
    }
  }

  int optimalBinShift = minBinShift;
  double FFvalue       = binWidth * optimalBinShift; 
  double FFerror       = GetFFerror(chiSqHisto,shift_min);
  FudgeFactor thisFF(chiSqHisto,dataHist,mcHist,optimalBinShift,FFvalue,FFerror);
  return thisFF;
}
void FFcalcAndCompare::GetFFs()
{
  for(int var = 0;var<Config::Nvars;var++)
  {
    vector<vector<FudgeFactor> > VarFFs;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      vector<FudgeFactor> VarEtFFs;
      for(int etaBin=0;etaBin<Config::NetaBins;etaBin++)
      {
        //if (etaBin == 4) VarEtFFs.push_back(999);
        TH1 * dataHist = dataPDFs[var][etBin][etaBin];
        TH1 * mcHist   =   mcPDFs[var][etBin][etaBin];
        TCanvas * c1 = new TCanvas();
        TH1 * NmcHist  = (TH1 *) mcHist->DrawNormalized("hist",dataHist->Integral()); 
        //cout << NmcHist << endl;
        //NmcHist->Print();
        NmcHist = (TH1 *) NmcHist->Clone(); // because the DAMN THING does not stay in memory
        FudgeFactor currFF = GetFF(dataHist,NmcHist,var,etBin,etaBin);
        VarEtFFs.push_back(currFF);
        delete c1;
      }
      VarFFs.push_back(VarEtFFs);
    }
    FudgeFactors.push_back(VarFFs);   
  }
}
void FFcalcAndCompare::Run()
{
  GetFFs();
  //writeFFsToPyFile();
  //writeErrorsToPyFile();
  //writeFFsToCFile();
  //writeErrorsToCFile();
  //writePlotsToFile();
  //writeFFsToRootFile();
  writeFFsCompareToRootFile();
}
void FFcalcAndCompare::writePlotsToFile()
{
  string filename = "output/outFile_FF_" + convStatus + ".root";
  TFile * f = new TFile(filename.c_str(),"recreate");
  for(int var =0 ; var < Config::Nvars; var++ )
  {
      string dirName = Config::varsN[var];
      f->mkdir(dirName.c_str());
      f->cd(dirName.c_str());
      for(int etBin =0; etBin < Config::NetBins ; etBin++)
      {
          for(int etaBin = 0; etaBin < Config::NetaBins;etaBin++ )
          {
              TH1 * data_hist = FudgeFactors[var][etBin][etaBin].GetDataPDF();
              TH1 * mc_hist   = FudgeFactors[var][etBin][etaBin].GetMCPDF();
              TH1 * chiSqPlot = FudgeFactors[var][etBin][etaBin].GetChiSqPlot();
              int   shift     = FudgeFactors[var][etBin][etaBin].GetShift();
              chiSqPlot->Write();
              
              string canvasName = "ph_" + Config::varsN[var]+"_sig_smoothed_hist_from_KDE_" + Config::etNames[etBin]+Config::etaNames[etaBin];
              TCanvas * c2 = new TCanvas(canvasName.c_str());
              c2->SetTitle(canvasName.c_str());
              
              canvasName.append("shift");
              TH1 * shiftedMC = (TH1 *) mc_hist->Clone(canvasName.c_str());
              TH1 * regularMC = (TH1 *) mc_hist->Clone(canvasName.c_str());
              //cout << "test2"<< endl;
              shiftedMC->Reset("M");
              regularMC->Reset("M");
              for(int bin = 1; bin < mc_hist->GetNbinsX(); bin++)
              {
                float content = mc_hist->GetBinContent(bin);
                shiftedMC->SetBinContent(bin+shift,content);
                regularMC->SetBinContent(bin,content);
              }
              data_hist->SetFillStyle(3004);
              data_hist->SetFillColor(kBlack);
              data_hist->SetLineColor(kBlack);
              data_hist->SetTitle("Data PDF");

              regularMC->SetLineColor(kRed);
              regularMC->SetTitle("MC PDF");
             
              shiftedMC->SetLineColor(kBlue);
              shiftedMC->SetTitle("Shifted MC PDF");

              data_hist->DrawNormalized();
              regularMC->DrawNormalized("same");
              shiftedMC->DrawNormalized("same");
              c2->BuildLegend();
              c2->Write();
              delete c2;
          }
      }
  }
  f->Close(); 

}
void FFcalcAndCompare::writeFFsCompareToRootFile()
{
  double (*FF_old[7])[10][8]= {&ff_c_reta_17_chisq,&ff_c_rphi_17_chisq,&ff_c_weta2_17_chisq,&ff_c_w1_17_chisq,&ff_c_wtot_17_chisq,&ff_c_fside_17_chisq,&ff_c_rhad_17_chisq};
    //double (*FF_old[7])[10][8] = {&ff_u_fside_17_chisq,&ff_u_reta_17_chisq,&ff_u_rhad_17_chisq,&ff_u_rphi_17_chisq,&ff_u_w1_17_chisq,&ff_u_weta2_17_chisq,&ff_u_wtot_17_chisq};
  string filename = "output/FF_COMPARE_" + convStatus + ".root";
  TFile * f = new TFile(filename.c_str(),"recreate");
  for(int var =0;var<Config::Nvars;var++)
  {
    string convName=Config::varsN[var]+"_"+convStatus;
    TCanvas * c1 =  new TCanvas(convName.c_str());
    TH2D * plot   = new TH2D(convName.c_str(),convName.c_str(),Config::NetBins,0,Config::NetBins,Config::NetaBins,0,Config::NetaBins);
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        if(etaBin==4) continue;
        double diff = (*FF_old[var])[etBin][etaBin] - FudgeFactors[var][etBin][etaBin].GetValue();
        cout << diff << endl;
        plot->Fill(etBin,etaBin, diff);
      }
    }
    plot->Draw("COLZ");
    c1->Write();
  }
  f->Close();

}

void FFcalcAndCompare::writeFFsToRootFile()
{
  string filename = "output/FF_tool_input_" + convStatus + ".root";
  TFile * f = new TFile(filename.c_str(),"recreate");
  for(int var =0;var<Config::Nvars;var++)
  {
    string convName=Config::varsN[var]+"_"+convStatus;
    TH2D * plot   = new TH2D(convName.c_str(),convName.c_str(),Config::NetBins,0,Config::NetBins,Config::NetaBins,0,Config::NetaBins);
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        if(etaBin==4) continue;
        plot->Fill(etBin,etaBin,FudgeFactors[var][etBin][etaBin].GetValue());
      }
    }
    plot->Write();

  }
  f->Close();

}

void FFcalcAndCompare::writeFFsToPyFile()
{
  ofstream myfile;
  string FFsFileName = "output/FudgeFactors_";
  FFsFileName.append(convStatus);
  FFsFileName.append(".py");
  myfile.open(FFsFileName);
  for(int var =0;var<Config::Nvars;var++)
  {
    myfile << Config::varsN[var] << " = [[" ;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      if(etBin != 0)
      {
          myfile<<"[";
      }
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        myfile << FudgeFactors[var][etBin][etaBin].GetValue();
        if(etaBin != Config::NetaBins-1)
        {
          myfile<<",";
        }
        else if(etBin != Config::NetBins-1)
        {
          myfile<<"],\n";

        }
        else
        {
          myfile<<"]]\n";
        }
      }
    }
  }
  myfile.close();
}
void FFcalcAndCompare::writeErrorsToPyFile()
{
  ofstream myfile;
  string ErrorFileName = "output/ChiSqErrors_";
  ErrorFileName.append(convStatus);
  ErrorFileName.append(".py");
  myfile.open(ErrorFileName);
  for(int var =0;var<Config::Nvars;var++)
  {
    myfile << Config::varsN[var] << " = [[" ;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      if(etBin != 0)
      {
          myfile<<"[";
      }
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        myfile << FudgeFactors[var][etBin][etaBin].GetError();
        if(etaBin != Config::NetaBins-1)
        {
          myfile<<",";
        }
        else if(etBin != Config::NetBins-1)
        {
          myfile<<"],\n";

        }
        else
        {
          myfile<<"]]\n";
        }
      }
    }
  }
  myfile.close();
}
void FFcalcAndCompare::writeFFsToCFile()
{
  ofstream myfile;
  string FFsFileName = "output/FudgeFactors_";
  FFsFileName.append(convStatus);
  FFsFileName.append(".C");
  myfile.open(FFsFileName);
  for(int var =0;var<Config::Nvars;var++)
  {
    myfile <<"float " <<Config::varsN[var]<< "[" << Config::NetBins << "]["<< Config::NetaBins <<"]" << " = {{" ;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      if(etBin != 0)
      {
          myfile<<"{";
      }
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        myfile << FudgeFactors[var][etBin][etaBin].GetValue();
        if(etaBin != Config::NetaBins-1)
        {
          myfile<<",";
        }
        else if(etBin != Config::NetBins-1)
        {
          myfile<<"},\n";

        }
        else
        {
          myfile<<"}};\n";
        }
      }
    }
  }
  myfile.close();
}
void FFcalcAndCompare::writeErrorsToCFile()
{
  ofstream myfile;
  string ErrorFileName = "output/ChiSqErrors_";
  ErrorFileName.append(convStatus);
  ErrorFileName.append(".C");
  myfile.open(ErrorFileName);
  for(int var =0;var<Config::Nvars;var++)
  {
    myfile <<"float " <<Config::varsN[var]<< "[" << Config::NetBins << "]["<< Config::NetaBins <<"]" << " = {{" ;
    for(int etBin=0;etBin<Config::NetBins;etBin++)
    {
      if(etBin != 0)
      {
          myfile<<"{";
      }
      for(int etaBin=0; etaBin<Config::NetaBins;etaBin++)
      {
        myfile << FudgeFactors[var][etBin][etaBin].GetError();
        if(etaBin != Config::NetaBins-1)
        {
          myfile<<",";
        }
        else if(etBin != Config::NetBins-1)
        {
          myfile<<"},\n";

        }
        else
        {
          myfile<<"}};\n";
        }
      }
    }
  }
  myfile.close();
}
