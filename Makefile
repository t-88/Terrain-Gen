INCLUDES = -Ilib/glad/include/ -Ilib/glfw-3.3.9/include/ -Ilib/
LIBS = $(shell pkg-config --libs opengl) $(shell pkg-config --libs glfw3)


CPP_SRCS = $(wildcard lib/glm/detail/*.cpp) 
CPP_OBJS = $(CPP_SRCS:%.cpp=%.o_cpp)

C_SRCS = $(wildcard lib/glad/src/*.c) 
C_OBJS = $(C_SRCS:%.c=%.o_c)


main: $(CPP_OBJS) $(C_OBJS)
	g++  $(C_OBJS) $(CPP_OBJS)  src/main.cpp -o build/main $(LIBS) $(INCLUDES)
	build/main

%.o_cpp: $(CPP_SRCS)
	g++ $(INCLUDES) $(LIBS) -c $< -o $@

%.o_c: $(C_SRCS)
	gcc $(INCLUDES) $(LIBS) -c $< -o $@
