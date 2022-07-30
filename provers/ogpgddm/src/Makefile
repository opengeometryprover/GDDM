BASE = ogpgddm

CPP = g++

CXX = g++

FLEX = flex
FLEXFLAGS =

BISON = bison
BISONFLAGS = -Wcounterexamples

INSTALLDIR = /usr/local/bin

.PHONY : clean all

# $(BASE)
all: $(BASE)

%.cpp %.hpp: %.yy
	$(BISON) $(BISONFLAGS) -o $*.cpp $<

%.cpp: %.ll
	$(FLEX) $(FLEXFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(BASE): $(BASE).o foftodb.o parser.o scanner.o dbRAM.o prover.o
	$(CXX) -o $@ ogpgddm.cpp foftodb.o parser.o scanner.o prover.o dbRAM.o -lsqlite3

$(BASE).o: parser.hpp
parser.o: parser.hpp
scanner.o: parser.hpp

CLEANFILES = $(BASE) *.o parser.hpp parser.cpp location.hh scanner.cpp

clean:
	rm -f $(CLEANFILES) $(BASE)

install:
	cp $(BASE) $(INSTALLDIR)
