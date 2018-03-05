EXEC=test
EXEC+=gnuplot2image
EXEC+=data2stems4gp
EXEC+=gpctrparamcustom

test_SRCS=test.cpp Gnuplot.cpp IOFiles.cpp Header.cpp RST.cpp Linux.cpp
gnuplot2image_SRCS=gnuplot2image.cpp Linux.cpp
data2stems4gp_SRCS=data2stems4gp.cpp 
gpctrparamcustom_SRCS=gpctrparamcustom.cpp

JDLIB = ../lib
BUILD = build
#-----------------------------------------------------------------

CXX = g++ -std=c++14

OPTION =
ERRORS = -Wall -Wextra -pedantic

VPATH = $(JDLIB) .
MYLIB = $(addprefix -I,$(VPATH))
-include $(JDLIB)/config.mk

CXXFLAGS = $(MYLIB) $(ERRORS) $(OPTION) $(CONFIG)
LDFLAGS  = $(MYLIB) $(ERRORS) $(OPTION)

all: OPTION += -O3 -DNDEBUG
all:$(EXEC)

debug: OPTION += -ggdb
debug:$(EXEC)

.SECONDEXPANSION:
$(EXEC): $$(patsubst %.cpp, $(BUILD)/%.o, $$($$@_SRCS))
	@echo Links $(notdir $^)
	$(CXX) $^ $(LDFLAGS) -o $@

$(BUILD)/%.o:%.cpp
	@echo Creates $(notdir $@)
	$(CXX) $< $(CXXFLAGS) -MMD -c -o $@

-include $(wildcard $(BUILD)/*.d)

clean:
	rm -f $(BUILD)/* test
