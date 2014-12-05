#                                                                               
# Makefile for Networks lab3                                 
#                           
# Authors: Melissa Galonsky and Helen Woodward
#                                                    

# ----- Make Macros -----                                                       

# OPTFLAGS  =   -O2                                                             
DEFINES   =
INCLUDES  =	
CXXFLAGS  =	-g -std=c++11 -Wall -Wextra -pedantic $(DEFINES) $(OPTFLAGS) $(INCLUDES) 
CXX       =	g++

TARGETS   =	server client
FILETRANSFER_OBJS = 

# ----- Make Rules -----                                                        

all:    $(TARGETS)

server: server.o
	$(CXX) -o server server.o $(CXXFLAGS)

client: client.o library.o
	$(CXX) -o client client.o $(CXXFLAGS)

clean:
	rm -f $(TARGETS) *.o

# ------ Dependences (.cpp -> .o using default Makefile rule) -----             

server.o: server.cpp
client.o: client.cpp
library.o: library.cpp