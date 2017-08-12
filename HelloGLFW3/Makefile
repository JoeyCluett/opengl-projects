# Makefile for Linux executable
# HelloGLFW3

GXX  = g++
INC  = -I$(GITHUB_CUSTOM_LIBS)
LIB  = -lglut -lglfw -lGLEW -lGL
BIN  = bin/Debug
OPTS = -std=c++11

all: main

main: main.cpp
	$(GXX) $(INC) -o $(BIN)/main main.cpp $(OPTS) $(LIB)

local: main.cpp
	$(GXX) $(INC) -o main_exec main.cpp $(OPTS) $(LIB)

clean:
	rm $(BIN)/main
