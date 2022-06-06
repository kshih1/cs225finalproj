# .PHONY: all test clean
# COMP_CMD = clang++ -std=c++14 -stdlib=libc++ -O3 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -MMD -MP -g -c

# main:
#	@mkdir -p objs
#	$(COMP_CMD) graph.cpp -o objs/graph.o
#	$(COMP_CMD) Airlines.cpp -o objs/Airlines.o
#	$(COMP_CMD) main.cpp -o objs/main.o
#	clang++ objs/graph.o objs/Airlines.o objs/main.o -std=c++14 -stdlib=libc++ -lc++abi -o airport

EXENAME = final_proj
OBJS = project.o main.o 

CXX = clang++
CXXFLAGS = $(CS225) -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

# Custom Clang version enforcement logic:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu") 
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else
ifneq ($(strip $(SKIP_EWS_CHECK)),True)
CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif
endif

.PHONY: all test clean output_msg

all : $(EXENAME)

output_msg: ; $(CLANG_VERSION_MSG)

$(EXENAME) : output_msg $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

project.o: graph.cpp graph.h Airlines.cpp Airlines.h
	$(CXX) $(CXXFLAGS) graph.cpp Airlines.cpp
	ld -r -o project.o graph.o Airlines.o

main.o : main.cpp graph.h Airlines.h
	$(CXX) $(CXXFLAGS) main.cpp

test: output_msg catchmain.o tests.o project.o
	$(LD) catchmain.o tests.o project.o $(LDFLAGS) -o test

catchmain.o : cs225/catch/catchmain.cpp cs225/catch/catch.hpp
	$(CXX) $(CXXFLAGS) cs225/catch/catchmain.cpp

tests.o : tests/tests.cpp cs225/catch/catch.hpp 
	$(CXX) $(CXXFLAGS) tests/tests.cpp

clean :
	-rm -f *.o $(EXENAME) test

