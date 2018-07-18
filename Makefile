INCLUDE_DIRS = $(shell find include -type d -depth 1)
INCLUDE_DIR_FLAG = $(addprefix -I, $(INCLUDE_DIRS))
LIB_DIR = lib
LIB_DIR_FLAG = -L$(LIB_DIR)
LIB_SRCS = $(shell find $(LIB_DIR) | grep "\.a")
LIB_FLAGS = -framework OpenGL -lglfw $(subst lib/lib, -l, $(basename $(LIB_SRCS)))
CC = clang++
STD_FLAG = -std=c++11

SRC_DIRS = $(shell find src -type d)
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c $(dir)/*.cc))

all:
	$(CC) $(LIB_DIR_FLAG) $(INCLUDE_DIR_FLAG) $(STD_FLAG) $(SRCS) -o main $(LIB_FLAGS)
