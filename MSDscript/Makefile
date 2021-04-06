#Definitions
INCS = cmdline.h expr.h parse.h val.h pointer.h env.h step.h cont.h
SRCS = main.cpp cmdline.cpp expr.cpp parse.cpp tests.cpp val.cpp env.cpp step.cpp cont.cpp

#Path to the sub directory
TESTPATH = ../test_msdscript
TESTINCS = $(TESTPATH)/cmdline.h $(TESTPATH)/exec.h $(TESTPATH)/test_generator.h
TESTSRCS = $(TESTPATH)/main.cpp $(TESTPATH)/cmdline.cpp $(TESTPATH)/exec.cpp $(TESTPATH)/test_generator.cpp

#OBJS = main.o cmdline.o expr.o tests.o parse.o
CXX = c++
CXXFLAGS = --std=c++14 -O2

#The executable "msdscript" depends on 2 object files
msdscript: $(INCS) $(SRCS)
	$(CXX) $(CXXFLAGS) -o msdscript $(SRCS)

#The executable "test_msdscript" depends on 2 object files
test_msdscript: $(TESTINCS) $(TESTSRCS)
	$(CXX) $(CXXFLAGS) -o test_msdscript $(TESTSRCS)

.PHONY: test
test: msdscript
	./msdscript --test
