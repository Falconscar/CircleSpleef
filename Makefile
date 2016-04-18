OBJS = *.cpp

CC = g++

COMPILER_FLAGS = -w -std=c++11

LINKER_FLAGS = -L/usr/local/lib -lSDL2_image -lSDL2

HEADERS = -I/usr/local/include/SDL2 -D_THREAD_SAFE

OBJ_NAME = CircleSpleef

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(HEADERS) -o $(OBJ_NAME)
