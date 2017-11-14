#EXEC=analyse_test
#EXEC+=analyse_tp
EXEC=analyse_class

BASE=IOFiles.cpp RST.cpp Header.cpp Linux.cpp Parseur.cpp Gnuplot.cpp Directory.cpp
CLASS=Class.cpp Note.cpp Examen.cpp TP.cpp VectorOfStrings.cpp ArrayOfStrings.cpp Latex.cpp

analyse_test_SRCS=analyse_test.cpp $(BASE) $(CLASS)
analyse_tp_SRCS=analyse_tp.cpp $(BASE) $(CLASS)
analyse_class_SRCS=analyse_class.cpp $(BASE) $(CLASS)

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
	rm -f $(BUILD)/* $(EXEC)
