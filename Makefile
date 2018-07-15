INCLUDE_DIRS = $(shell find include -type d -depth 1)
INCLUDE_DIR_FLAG = $(addprefix -I, $(INCLUDE_DIRS))
LIB_DIR = lib
LIB_DIR_FLAG = -L$(LIB_DIR)
LIB_SRCS = $(shell find $(LIB_DIR) | grep "\.a")
LIB_FLAGS = -framework OpenGL -lglfw $(subst lib/lib, -l, $(basename $(LIB_SRCS)))
CC = clang++
STD_FLAG = -std=c++11

SRCS = $(wildcard src/*.c src/*.cpp)

all: $(SRCS)
	$(CC) $(STD_FLAG) $(INCLUDE_DIR_FLAG) $(LIB_DIR_FLAG) $(SRCS) -o main $(LIB_FLAGS) 
