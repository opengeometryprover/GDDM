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
all: $(BASE) ogpgddmStaticLib ogpgddmDynamicLib

%.cpp: %.ll
	$(FLEX) $(FLEXFLAGS) -o $@ $<

%.cpp %.hpp: %.yy
	$(BISON) $(BISONFLAGS) -o $*.cpp $<

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<


$(BASE): $(BASE).o scanner.o parser.o dbRAM.o foftodb.o prover.o strs.o
	$(CPP) -o $@ ogpgddm.cpp foftodb.o parser.o scanner.o prover.o dbRAM.o strs.o -lsqlite3

$(BASE).o: parser.hpp
parser.o: parser.hpp strs.hpp strs.cpp
scanner.o: parser.hpp

CLEANFILES = $(BASE) ogpgddmStaticLib ogpgddmDynamicLib *.a *.so* *.o parser.hpp parser.cpp location.hh scanner.cpp

clean:
	rm -f $(CLEANFILES) $(BASE)

install:
	cp $(BASE) $(INSTALLDIR)

ogpgddmStaticLib: ogpgddm.cpp  staticLib
	$(CPP) -o $@ ogpgddm.cpp -static -logpgddm -L. -I.  -lsqlite3

ogpgddmDynamicLib: ogpgddm.cpp  dynamicLib
	$(CPP)  -o $@ ogpgddm.cpp   -logpgddm -L. -I. -lsqlite3

### To run it is needed to install the libraries or to do
### export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

staticLib:  scanner.o parser.o dbRAM.o foftodb.o prover.o strs.o
	ar cq libogpgddm.a  scanner.o parser.o dbRAM.o foftodb.o prover.o strs.o

dynamicLib:  scanner.cpp parser.cpp dbRAM.cpp foftodb.cpp prover.cpp strs.cpp
	$(CPP)  -c -fPIC   scanner.cpp parser.cpp dbRAM.cpp foftodb.cpp prover.cpp strs.cpp
	$(CPP)  -shared -o libogpgddm.so.1.0  scanner.o parser.o dbRAM.o foftodb.o prover.o strs.o
	ln -sf libogpgddm.so.1.0  libogpgddm.so.1
	ln -sf libogpgddm.so.1.0  libogpgddm.so
