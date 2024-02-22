INCLUDES = -Ilib/glad/include/ -Ilib/glfw-3.3.9/include/ -Ilib/ -Isrc/includes
LIBS = $(shell pkg-config --libs opengl) $(shell pkg-config --libs glfw3) 

CPP_SRCS = $(wildcard lib/glm/detail/*.cpp) $(wildcard src/src/*.cpp) 
CPP_OBJS = $(CPP_SRCS:%.cpp=%.o_cpp)

C_SRCS = $(wildcard lib/glad/src/*.c) 
C_OBJS = $(C_SRCS:%.c=%.o_c)


main: $(CPP_OBJS) $(C_OBJS)
	g++  src/main.cpp $(INCLUDES) $(LIBS) $(C_OBJS) $(CPP_OBJS) -o build/main 
	build/main

%.o_cpp: %.cpp
	g++ $(INCLUDES) -c $< -o $@ $(LIBS)

%.o_c: %.c
	gcc $(INCLUDES) -c $< -o $@ $(LIBS)


clean:
	rm $(CPP_OBJS) $(C_OBJS) 