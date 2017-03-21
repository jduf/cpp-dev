EXEC=jdread jdhtml

jdread_SRCS = jdread.cpp Header.cpp IOFiles.cpp RST.cpp Linux.cpp
jdhtml_SRCS = jdhtml.cpp Header.cpp IOFiles.cpp RST.cpp RSTFile.cpp Linux.cpp

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
	rm -f $(BUILD)/*
