#Definitions
INCS = cmdline.h
OBJS = main.o cmdline.o
CXX = c++
CXXFLAGS = --std=c++14 -O2
#The executable "msdscript" depends on 2 object files
msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)
#Build main.o requires main.cpp and cmdline.h to exist
main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c main.cpp
#Build cmdline.o requires cmdline.cpp and cmdline.h to exist
cmdline.o: cmdline.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c cmdline.cpp
