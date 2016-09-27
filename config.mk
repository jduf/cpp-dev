MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CONFIGFILE_PATH := $(dir $(MKFILE_PATH))

#The following macro are required in Linux.hpp.. They need to be set to the
#correct path/executable in order to run
CONFIG=  -D'DEF_CONFIG'
CONFIG+= -D'MY_RST2HTML_STYLESHEET="$(CONFIGFILE_PATH)rst/css/best.css"'
CONFIG+= -D'MY_BIN_RST2HTML="rst2html"'
CONFIG+= -D'MY_BIN_RST2LATEX="rst2latex"'
CONFIG+= -D'MY_BIN_PDFLATEX="pdflatex"'
CONFIG+= -D'MY_BIN_GNUPLOT="gnuplot"'
CONFIG+= -D'MY_BIN_LATEX="latex"'
CONFIG+= -D'MY_BIN_DVIPDF="dvipdf"'
CONFIG+= -D'MY_BIN_PDFCROP="pdfcrop"'
CONFIG+= -D'MY_BIN_CONVERT="convert"'
CONFIG+= -D'MY_BIN_HTMLBROWSER="/usr/bin/firefox"'
