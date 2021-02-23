#Definitions
INCS = cmdline.h expr.h parse.h
SRCS = main.cpp cmdline.cpp expr.cpp parse.cpp tests.cpp

#Path to the sub directory
TESTPATH = ../test_msdscript/
TESTINCS = cmdline.h exec.h test_generator.h
TESTSRCS = main.cpp cmdline.cpp exec.cpp test_generator.cpp

#OBJS = main.o cmdline.o expr.o tests.o parse.o
CXX = c++
CXXFLAGS = --std=c++14 -O2

#The executable "msdscript" depends on 2 object files
msdscript: $(INCS) $(SRCS)
	$(CXX) $(CXXFLAGS) -o msdscript $(SRCS)

#The executable "test_msdscript" depends on 2 object files
test_msdscript: $(TESTPATH/$(TESTINCS)) $(TESTPATH)main.cpp $(TESTPATH)cmdline.cpp $(TESTPATH)exec.cpp $(TESTPATH)test_generator.cpp
	$(CXX) $(CXXFLAGS) -o ../MSDscript/test_msdscript $(TESTPATH)main.cpp $(TESTPATH)cmdline.cpp $(TESTPATH)exec.cpp $(TESTPATH)test_generator.cpp

.PHONY: test
test: msdscript
	./msdscript --test
