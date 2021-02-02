#Definitions
INCS = cmdline.h expr.h
OBJS = main.o cmdline.o expr.o expr_tests.o
CXX = c++
CXXFLAGS = --std=c++14 -O2
#The executable "msdscript" depends on 2 object files
msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

.PHONY: test
test: msdscript
	./msdscript --test
	
#Build main.o requires main.cpp and cmdline.h to exist
main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c main.cpp
#Build cmdline.o requires cmdline.cpp and cmdline.h to exist
cmdline.o: cmdline.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp
#Build expr.o requires expr.cpp and expr.h to exist
expr.o: expr.cpp expr.h
	$(CXX) $(CXXFLAGS) -c expr.cpp
#Build expr_tests.o requires expr_tests.cpp and expr.h to exist
expr_tests.o: expr_tests.cpp expr.h
	$(CXX) $(CXXFLAGS) -c expr_tests.cpp
