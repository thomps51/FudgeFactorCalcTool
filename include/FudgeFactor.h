#include "TH1F.h"
#include "TH1.h"

using namespace std;

class FudgeFactor
{
  public:
    FudgeFactor(): chisqPlot(0), value(0), error(0) {}
    FudgeFactor(TH1F * Chiplot, TH1 * data_hist, TH1 * mc_hist, int bin_shift, float val, float err): 
      chisqPlot(Chiplot),dataHist(data_hist),mcHist(mc_hist), shift(bin_shift), value(val), error(err) {}
   
    double GetValue()     { return value;  }
    double GetError()     { return error;  }
    TH1F * GetChiSqPlot() { return chisqPlot; }
    TH1  * GetDataPDF()   { return dataHist; }
    TH1  * GetMCPDF()     { return mcHist;   }
    int    GetShift()     { return shift;    } 

    TH1F * chisqPlot;
    TH1  * dataHist;
    TH1  * mcHist;
    int    shift;
    double value;
    double error;
};
