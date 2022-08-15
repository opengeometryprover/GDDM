BASE = ogpgddm
CPP = g++
CPPFLAGS = 
CXX = g++
FLEX = flex
FLEXFLAGS =
BISON = bison
BISONFLAGS = -Wcounterexamples
INSTALLDIR = /usr/local/bin

.PHONY : clean all

# $(BASE)
all: $(BASE)

%.cpp: %.ll
	$(FLEX) $(FLEXFLAGS) -o $@ $<

%.cpp %.hpp: %.yy
	$(BISON) $(BISONFLAGS) -o $*.cpp $<

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<


$(BASE): $(BASE).o scanner.o parser.o dbRAM.o foftodb.o prover.o points.o
	$(CPP) -o $@ ogpgddm.cpp foftodb.o parser.o scanner.o prover.o dbRAM.o points.o -lsqlite3

$(BASE).o: parser.hpp
parser.o: parser.hpp
scanner.o: parser.hpp

CLEANFILES = $(BASE) *.o parser.hpp parser.cpp location.hh scanner.cpp

clean:
	rm -f $(CLEANFILES) $(BASE)

install:
	cp $(BASE) $(INSTALLDIR)
