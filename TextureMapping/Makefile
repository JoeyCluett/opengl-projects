# TextureMapping

GXX  = g++
INC  = -I$(GITHUB_CUSTOM_LIBS)
LIB  = -lglut -lglfw -lGLEW -lGL -lIL -lILU -lILUT
BIN  = bin/Debug
OPTS = -std=c++11
OPTS += -Wfatal-errors

all: main

main: main.cpp
	$(GXX) $(INC) -o $(BIN)/main main.cpp $(OPTS) $(LIB)

clean:
	rm $(BIN)/main
