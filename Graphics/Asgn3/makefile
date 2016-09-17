# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------
#
# Modified by: Cody Valle
# Date       : 2016-06-03
#
# Changelog :
#   2016-06-03 - added: comments, SRC_EXT macro, mkdir calls
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = AlphaNum

CC       = g++
# compiling flags here
CFLAGS   = -std=c++0x -g -O2 -Wall -Wextra -pedantic -pedantic-errors -fexceptions -Weffc++

LINKER   = g++
# linking flags here
LFLAGS   = -lglut -lGL -lGLU -lGLEW -lm

# change these to set the proper directories where each files should be
SRCDIR   = .
OBJDIR   = obj
BINDIR   = .

SRC_EXT  := .cpp
SOURCES  := $(wildcard $(SRCDIR)/*$(SRC_EXT))
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%$(SRC_EXT)=$(OBJDIR)/%.o)
rm       = rm -f

all: $(SOURCES) $(BINDIR)/$(TARGET)

# link the objects into the target
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(LINKER) -o $@ $(OBJECTS) $(LFLAGS)

# compile the sources into the objects
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%$(SRC_EXT)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(rm) $(OBJECTS) $(BINDIR)/$(TARGET)
