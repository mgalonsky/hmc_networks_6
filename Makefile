#                                                                               
# Makefile for Networks lab3                                 
#                           
# Authors: Melissa Galonsky and Helen Woodward
#                                                    

# ----- Make Macros -----                                                       

# OPTFLAGS  =   -O2                                                             
DEFINES   =
INCLUDES  =	-lpthread
CXXFLAGS  =	-g -std=c++11 -Wall -Wextra -pedantic $(DEFINES) $(OPTFLAGS) $(INCLUDES) 
CXX       =	g++

TARGETS   =	server client
FILETRANSFER_OBJS = 

# ----- Make Rules -----                                                        

all:    $(TARGETS)

server: server.o utilities.o barrier.o lock.o
	$(CXX) -o server server.o utilities.o barrier.o $(CXXFLAGS)

client: client.o client_library.o utilities.o
	$(CXX) -o client client.o client_library.o utilities.o $(CXXFLAGS)

clean:
	rm -f $(TARGETS) *.o

# ------ Dependences (.cpp -> .o using default Makefile rule) -----             

server.o: server.cpp
client.o: client.cpp
client_library.o: client_library.cpp
utilities.o: utilities.cpp
barrier.o: barrier.cpp
lock.o: lock.cpp
