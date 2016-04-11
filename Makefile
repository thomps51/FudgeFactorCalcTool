

ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs) -lMinuit -lEG -lTMVA 
CXX = g++

CXXFLAGS  = -I. -I$(ROOTSYS)/include -O -Wall -fopenmp -fPIC -Wno-reorder -g -std=c++1y # -pg
CXXFLAGS += $(ROOTCFLAGS)
LIBS    = $(ROOTLIBS) 

OBJS = libs/Config.o libs/BinnedHistGetter.o libs/BinnedHistGetterNTUP.o libs/PDFgetter.o libs/FFcalc.o 
DEPS = include/Config.h include/progress_bar.h include/BinnedHistGetter.h include/BinnedHistGetterNTUP.h include/PDFgetter.h include/FFcalc.h utils/getFFsFromPskim.C utils/getFFsFromPDFs.C utils/test.C utils/getFFsFromNTUP.C utils/getBinnedHistsFromNTUP.C


all: getFFsFromPskim getFFsFromPDFs test getFFsFromNTUP getBinnedHistsFromNTUP getFFsFromBinnedHists
 

%.o: %.C $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.cxx $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)


libs/Config.o: Config.C $(DEPS)
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/PDFgetter.o: Root/PDFgetter.C $(DEPS)
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)

libs/FFcalc.o: Root/FFcalc.C $(DEPS)
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/FFcalcAndCompare.o: Root/FFcalcAndCompare.C include/Config.h 
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)

libs/BinnedHistGetter.o: Root/BinnedHistGetter.C $(DEPS)
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/BinnedHistGetterNTUP.o: Root/BinnedHistGetterNTUP.C $(DEPS)
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
getFFsFromPskim: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getFFsFromPskim.C 
getFFsFromBinnedHists: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getFFsFromBinnedHists.C 
getFFsFromNTUP: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getFFsFromNTUP.C 
getFFsFromPDFs: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getFFsFromPDFs.C 
getBinnedHistsFromNTUP: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getBinnedHistsFromNTUP.C 
test: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/test.C 
compare: libs/FFcalcAndCompare.o libs/Config.o
	$(CXX) -o $@ libs/FFcalcAndCompare.o libs/Config.o $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/compare.C 



# suffix rule
.cc.o:
	$(CXX) -c $(CXXFLAGS) $(GDBFLAGS) $<

# clean
clean:
	rm -f pskim *~ *.o *.o~ *.so core



