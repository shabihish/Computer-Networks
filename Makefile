# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
default: main
# ****************************************************
# Targets needed to bring the executable up to date

main: main.o Network.o Routing.o
	$(CC) $(CFLAGS) -o main main.o Network.o Routing.o

 
main.o: main.cpp Network.h Routing.h
	$(CC) $(CFLAGS) -c main.cpp
 
Network.o: Network.h
 
Routing.o: Routing.h Network.h

clean: 
	$(RM) main *.o *~