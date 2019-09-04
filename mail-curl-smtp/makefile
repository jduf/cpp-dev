###############################################
### CHANGE ONLY IF YOU KNOW WHAT YOUR DOING ###
###############################################
EXEC=test

test_SRCS=test.cpp MailCurlSMTP.cpp

JDLIB = ../lib
BUILD = build

####################################
### DO NOT CHANGE ANYTHING BELOW ###
####################################
CXX = g++ -std=c++17

OPTION =
ERRORS = -Wall -Wextra -pedantic
NCURSES= -lcurl

VPATH = $(JDLIB) .
MYLIB = $(addprefix -I,$(VPATH))
-include $(JDLIB)/config.mk

CXXFLAGS = $(MYLIB) $(ERRORS) $(OPTION) $(CONFIG)
LDFLAGS  = $(MYLIB) $(ERRORS) $(OPTION) $(NCURSES)

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
