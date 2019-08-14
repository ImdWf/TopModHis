#CC = g++ -Wall -g -std=c++17
CC = g++ -Wall -std=c++17
# LProject = C:/Users/ImdWf/source/repos/Vorbereitung
# LExec = E:/cygwin64/bin
LSOURCE = *.cpp
LHEADER = *.h

all: $(LSOURCE) $(LHEADER)
	$(CC) ./$(LSOURCE) -o tmh
