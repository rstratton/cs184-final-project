CC = g++
CFLAGS =
RM = /bin/rm -rf
OBJ_FILES = algebra3.o Fluid.o Particle.o SceneParser.o Simulator.o StaticObject.o

all: main

main: $(OBJ_FILES) main.cpp
	$(CC) $(CFLAGS) main.cpp $(OBJ_FILES) -o main.exe

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.exe
