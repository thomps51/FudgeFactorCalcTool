# Slowly getting better at making makefiles... This is still terrible
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs) -lMinuit -lEG -lTMVA  
CXX = g++

CXXFLAGS  = -I. -I$(ROOTSYS)/include -O -Wall -fopenmp -fPIC -Wno-reorder -g
CXXFLAGS += $(ROOTCFLAGS)
LIBS    = $(ROOTLIBS) 

OBJS = libs/Config.o
DEPS = include/Config.h include/progress_bar.h

getSkimFromNTUP_DEPS        = include/NTUPskimmer.h utils/getSkimFromNTUP.C
getBinnedHistsFromSkim_DEPS = include/BinnedHistGetter.h utils/getBinnedHistsFromSkim.C
getPDFsFromBinnedHists_DEPS = include/PDFgetter.h utils/getPDFsFromBinnedHists.C
getFFsFromPDFs_DEPS         = include/FFcalc.h utils/getFFsFromPDFs.C
getFFsFromSkim_DEPS         = $(getBinnedHistsFromSkim_DEPS) $(getPDFsFromBinnedHists_DEPS) $(getFFsFromPDFs_DEPS)

getSkimFromNTUP_OBJS        = libs/NTUPskimmer.o 
getBinnedHistsFromSkim_OBJS = libs/BinnedHistGetter.o
getPDFsFromBinnedHists_OBJS = libs/PDFgetter.o
getFFsFromPDFs_OBJS         = libs/FFcalc.o
getFFsFromSkim_OBJS         = $(getBinnedHistsFromSkim_OBJS) $(getPDFsFromBinnedHists_OBJS) $(getFFsFromPDFs_OBJS)

all: getSkimFromNTUP getBinnedHistsFromSkim getPDFsFromBinnedHists getFFsFromPDFs getFFsFromSkim


%.o: %.C $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.cxx $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)


libs/Config.o: Config.C $(DEPS)
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/PDFgetter.o: Root/PDFgetter.C $(DEPS) libs/Config.o include/PDFgetter.h 
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/NTUPskimmer.o: Root/NTUPskimmer.C $(DEPS) libs/Config.o include/NTUPskimmer.h
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/FFcalc.o: Root/FFcalc.C $(DEPS) libs/Config.o include/FFcalc.h include/FudgeFactor.h
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/FFcalcAndCompare.o: Root/FFcalcAndCompare.C include/Config.h include/FFcalcAndCompare.h
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)
libs/BinnedHistGetter.o: Root/BinnedHistGetter.C $(DEPS) libs/Config.o include/BinnedHistGetter.h
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(ROOTCORE_INCL)

getSkimFromNTUP: $(OBJS) $(getSkimFromNTUP_OBJS) $(DEPS) $(getSkimFromNTUP_DEPS)
	if [ -a getSkimFromNTUP ]; then rm getSkimFromNTUP ; fi; # avoids "undefined" behavior if it is already running
	$(CXX) -o $@ $(OBJS) $(getSkimFromNTUP_OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getSkimFromNTUP.C
getBinnedHistsFromSkim: $(OBJS) $(getBinnedHistsFromSkim_OBJS) $(DEPS) $(getBinnedHistsFromSkim_DEPS)
	if [ -a getBinnedHistsFromSkim ]; then rm getBinnedHistsFromSkim ; fi;	
	$(CXX) -o $@ $(OBJS) $(getBinnedHistsFromSkim_OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getBinnedHistsFromSkim.C 
getPDFsFromBinnedHists: $(OBJS) $(getPDFsFromBinnedHists_OBJS) $(DEPS) $(getPDFsFromBinnedHists_DEPS)
	if [ -a getPDFsFromBinnedHists ]; then rm getPDFsFromBinnedHists ; fi;
	$(CXX) -o $@ $(OBJS) $(getPDFsFromBinnedHists_OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getPDFsFromBinnedHists.C 
getFFsFromPDFs: $(OBJS) $(getFFsFromPDFs_OBJS) $(DEPS) $(getFFsFromPDFs_DEPS)
	if [ -a getFFsFromPDFs ]; then rm getFFsFromPDFs ; fi;
	$(CXX) -o $@ $(OBJS) $(getFFsFromPDFs_OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getFFsFromPDFs.C 
getFFsFromSkim: $(OBJS) $(getFFsFromSkim_OBJS) $(DEPS) $(getFFsFromSkim_DEPS)
	if [ -a getFFsFromSkim ]; then rm getFFsFromSkim ; fi;
	$(CXX) -o $@ $(OBJS) $(getFFsFromSkim_OBJS) $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/getFFsFromSkim.C 

compare: $(OBJS) libs/FFcalcAndCompare.o 
	$(CXX) -o $@ $(OBJS) libs/FFcalcAndCompare.o $(CXXFLAGS) $(ROOTCORE_INCL) $(LIBS) utils/compare.C


# suffix rule
.cc.o:
	$(CXX) -c $(CXXFLAGS) $(GDBFLAGS) $<

# clean
clean:
	rm getSkimFromNTUP getBinnedHistsFromSkim getPDFsFromBinnedHists getFFsFromPDFs libs/*.o 



