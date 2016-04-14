#include "getopt.h"
#include "include/FFcalc.h"
using namespace std;


int main(int argc, char **argv)
{
  FFcalc * FFobj_c = new FFcalc("output/pdfs/pdfs_data_c.root","output/pdfs/pdfs_mc_c.root","c");
  FFobj_c->Run();

  FFcalc * FFobj_u = new FFcalc("output/pdfs/pdfs_data_u.root","output/pdfs/pdfs_mc_u.root","u");
  FFobj_u->Run();
}
