#include "getopt.h"
#include "include/FFcalcAndCompare.h"
#include "TMVA/PDF.h"
using namespace std;


int main(int argc, char **argv)
{
  FFcalcAndCompare * FFobj_c = new FFcalcAndCompare("output/pdfs/pdfs_data_c.root","output/pdfs/pdfs_mc_c.root","c");
  FFobj_c->Run();


  FFcalcAndCompare * FFobj_u = new FFcalcAndCompare("output/pdfs/pdfs_data_u.root","output/pdfs/pdfs_mc_u.root","u");
  FFobj_u->Run();


}
