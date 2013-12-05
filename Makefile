CC = g++
RM = /bin/rm -rf
OBJ_FILES = algebra3.o Fluid.o Particle.o SceneParser.o Simulator.o StaticObject.o Camera.o KeyboardHandlers.o Renderer.o ParticleInspector.o BVH.o Shape.o

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
    CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -I./ -DOSX
    LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-L"./" \
    	-lGL -lGLU -lm -lstdc++ -ltbb
else
	#CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	#LDFLAGS = -lglut -lGLU -lGL
	#CFLAGS = -std=c++0x -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif

all: main

main: $(OBJ_FILES) main.cpp
	$(CC) $(CFLAGS) main.cpp $(OBJ_FILES) -o main.exe $(LDFLAGS)

particle_inspector: $(OBJ_FILES) ParticleInspector.o
	$(CC) $(CFLAGS) ParticleInspectorTest.cpp $(OBJ_FILES) -o ParticleInspectorTest.exe $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.exe *.dSYM
