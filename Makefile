WORKSPACE=$(shell pwd)/
BOOST=lib/boost/boost_1_55_0
BOOST_PROCESS=lib/boost-process/
CXX=g++-4.9

TARGET=$(WORKSPACE)fab

CPPFILES=$(shell find $(WORKSPACE)src -name "*.cpp")
BOOST_IOSTREAMS_FILES=$(shell find $(BOOST)/libs/iostreams -name "*.cpp" | grep -v "/example/" | grep -v "/test/" | grep -v bzip2 | grep -v zlib | grep -v gzip )
BOOST_SYSTEM_FILES=$(shell find $(BOOST)/libs/system -name "*.cpp" | grep -v "/example/" | grep -v "/doc/" | grep -v "/test/")
BOOST_FILESYSTEM_FILES=$(shell find $(BOOST)/libs/filesystem -name "*.cpp" | grep -v "/example/" | grep -v "/doc/" | grep -v "/test/")
BOOST_PROGRAM_OPTIONS_FILES=$(shell find $(BOOST)/libs/program_options -name "*.cpp" | grep -v "/example/" | grep -v "/test/")
CPPFILES+=$(BOOST_IOSTREAMS_FILES)
CPPFILES+=$(BOOST_SYSTEM_FILES)
CPPFILES+=$(BOOST_FILESYSTEM_FILES)
CPPFILES+=$(BOOST_PROGRAM_OPTIONS_FILES)
OFILES=$(CPPFILES:.cpp=.o)

CXXFLAGS+=-std=c++11 
CXXFLAGS+=-g
CXXFLAGS+=-MD
CXXFLAGS+=-I$(BOOST)/include
CXXFLAGS+=-I$(BOOST_PROCESS)


#LFLAGS+=-L$(BOOST)/lib
#LFLAGS+=-lboost_filesystem-mt
#LFLAGS+=-lboost_system
#LFLAGS+=-lboost_program_options

all : $(TARGET)

clean :
	-rm -f $(TARGET)
	-rm -f $(OFILES)

$(TARGET) : $(OFILES)
	$(CXX) $(LFLAGS) -o $@ $+

test : $(TARGET)
	touch $(WORKSPACE)src/main/build_target.cpp
	$(TARGET) --version
	$(TARGET)
	ls fab.exe


-include $(CPPFILES:.cpp=.d) $(TEST_MODULES:.cpp=.d)
