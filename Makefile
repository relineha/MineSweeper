
#ALGORITHMS VARIABLES
SDIRALGS = ./Algorithms/src
ODIRALGS = $(SDIRALGS)/obj
IDIRALGS = ./Algorithms/include
CXXFLAGSALGS = -I$(IDIRALGS)
LIBSALGS = -lfftw3 -lm

_DEPSALGS = RunSetup.hh RunCombination.hh RunSweep.hh DBCluster.hh
DEPSALGS = $(patsubst %,$(IDIRALGS)/%,$(_DEPSALGS))

_OBJALGS = RunSetup.o RunCombination.o RunSweep.o DBCluster.o
OBJALGS =  $(patsubst %,$(ODIRALGS)/%,$(_OBJALGS))


#DATA PRODUCTS VARIABLES
SDIRPRODS = ./DataProducts/src
ODIRPRODS = $(SDIRPRODS)/obj
IDIRPRODS = ./DataProducts/include
CXXFLAGSPRODS = -I$(IDIRPRODS)
LIBSPRODS = -lm

_DEPSPRODS = Particle.hh Cluster.hh Grid.hh DateAndTime.hh Picture.hh ParameterSet.hh Geometry.hh
DEPSPRODS = $(patsubst %,$(IDIRPRODS)/%,$(_DEPSPRODS))

_OBJPRODS = Particle.o Cluster.o Grid.o DateAndTime.o Picture.o ParameterSet.o Geometry.o ParameterSetDict.o ParticleDict.o ClusterDict.o GridDict.o DateAndTimeDict.o PictureDict.o GeometryDict.o
OBJPRODS =  $(patsubst %,$(ODIRPRODS)/%,$(_OBJPRODS))



#ANALYSIS VARIABLES
SDIRANA = ./Analysis/src
ODIRANA = $(SDIRANA)/obj
IDIRANA = ./Analysis/include
CXXFLAGSANA = -I$(IDIRANA)
LIBSANA = -lm

_DEPSANA = AnalysisFunctions.hh ScanP2CorrelationAnalysis.hh
DEPSANA = $(patsubst %,$(IDIRANA)/%,$(_DEPSANA))

_OBJANA = AnalysisFunctions.o ScanP2CorrelationAnalysis.o
OBJANA =  $(patsubst %,$(ODIRANA)/%,$(_OBJANA))



#MAIN FILE MACRO
MAINNAME=MineSweeper.cpp


#Trying to generate a dictionary for the different classes. This allows us to integrate our classes with ROOT's I/O capabilities
ParameterSetDict.cpp: ./DataProducts/include/ParameterSet.hh
	rootcint -f $@ -std=c++11 $^

ClusterDict.cpp: ./DataProducts/include/Cluster.hh
	rootcint -f $@ -std=c++11 $^

GridDict.cpp: ./DataProducts/include/Grid.hh
	rootcint -f $@ -std=c++11 $^

ParticleDict.cpp: ./DataProducts/include/Particle.hh
	rootcint -f $@ -std=c++11 $^

DateAndTimeDict.cpp: ./DataProducts/include/DateAndTime.hh
	rootcint -f $@ -std=c++11 $^

PictureDict.cpp: ./DataProducts/include/Picture.hh
	rootcint -f $@ -std=c++11 $^

GeometryDict.cpp: ./DataProducts/include/Geometry.hh
	rootcint -f $@ -std=c++11 $^




#libMyLib.so: MyDict.cxx ./DataProducts/src/ParameterSet.cpp
#	g++ -shared -o$@ `root-config --ldflags` -std=c++11 -I$(ROOTSYS)/include -I$(CXXFLAGSPRODS) $^ 

#libParameterSetLib.so: /Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/DataProducts/include/ParameterSet_rflx.cpp
#g++ `root-config --cflags` `root-config --glibs` -std=c++11 -stdlib=libc++ -Wall -c -o $@ $< $(CXXFLAGSPRODS)

#all: $(ODIRPRODS)/%.o $(ODIRALGS)/%.o $(ODIRANA)/%.o SweepForMines

#This takes the source files (human readable language) and uses them to generate machine-readable language object files for the products code
$(ODIRPRODS)/%.o: $(SDIRPRODS)/%.cpp $(DEPSPRODS)
	g++ `root-config --cflags` `root-config --glibs` -std=c++11 -stdlib=libc++ -Wall -c -o $@ $< $(CXXFLAGSPRODS)


#This takes the source files (human readable language) and uses them to generate machine-readable language object files for the algorithms code
$(ODIRALGS)/%.o: $(SDIRALGS)/%.cpp $(DEPSALGS) $(DEPSPRODS) 
	g++ `root-config --cflags` `root-config --glibs` -std=c++11 -stdlib=libc++ $(LIBSALGS) -Wall -c -o $@ $<  $(CXXFLAGSALGS) $(CXXFLAGSPRODS) 

#This takes the source files (human readable language) and uses them to generate machine-readable language object files for the analysis code
$(ODIRANA)/%.o: $(SDIRANA)/%.cpp $(DEPSANA) $(DEPSALGS) $(DEPSPRODS)
	g++ `root-config --cflags` `root-config --glibs` -std=c++11 -stdlib=libc++ -Wall -c -o $@ $< $(CXXFLAGSANA) $(CXXFLAGSALGS) $(CXXFLAGSPRODS)

#This takes the object files (machine-readable language) of the products, algorithms, and analysis code and uses them to produce an executable
SweepForMines: $(OBJPRODS) $(OBJALGS) $(OBJANA) 
	g++ `root-config --cflags` `root-config --glibs` -std=c++11 -stdlib=libc++ $(LIBSALGS) -Wall -g $^ -o $@ ${MAINNAME} $(CXXFLAGSALGS) $(CXXFLAGSPRODS) ${CXXFLAGSANA}


clean:
	rm -f $(ODIRPRODS)/*.o $(SDIRPRODS)/*~ core $(IDIRPRODS)/*~
	rm -f $(ODIRALGS)/*.o $(SDIRALGS)/*~ core $(IDIRALGS)/*~
	rm -f $(ODIRANA)/*.o $(SDIRANA)/*~ core $(IDIRANA)/*~
