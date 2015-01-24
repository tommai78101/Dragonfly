#
# Makefile for development of game using Dragonfly
#
# 'make' to build executable
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
#
# Variables of interest:
#   GAMESRC is the source code files for the game being developed
#   GAME is the game main() source
#   DEBUG can be set to -DDEBUG to enable various debug flags
#

CC= g++ -std=c++11 -g
PLATFORM= -DCYGWIN		# uncomment for cygwin
#PLATFORM= -DMACOS  		# uncomment for macos
LDFLAGS= -lncurses -lrt	# uncomment linux/cygwin
#LDFLAGS= -lncurses 		# uncomment for macos
INCPATH= include						# path to engine includes
ENG= lib/libdragonfly-cygwin64.a		# dragonfly engine
GAMESRC= $(foreach d, $(wildcard source/*.cpp), $d)
GAME= platform.cpp
EXECUTABLE= game		
OBJECTS= $(GAMESRC:.cpp=.o)

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(ENG) $(OBJECTS) $(GAME) $(GAMESRC)
	$(CC) $(GAME) $(OBJECTS) $(ENG) -o $@ -I$(INCPATH) $(LDFLAGS) 

.cpp.o: 
	$(CC) -c $(DEBUG) -I$(INCPATH) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core dragonfly.log Makefile.bak *~

depend: 
	makedepend *.cpp 2> /dev/null

# DO NOT DELETE
