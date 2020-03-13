OBJS = src/main.cpp src/console.cpp
CC = g++
LINKER_FLAGS = -lSDL2 -lSDL2_image -licuuc
OBJ_NAME = SDL_console

all:
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm -f $(OBJ_NAME)
