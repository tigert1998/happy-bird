CC = g++
SRC = main.cc
SRC_PATH = src
TEST_PATH = test
FLAGS = -std=c++11
TEST_FLAGS = -lgtest -std=c++11
# FLAGS = -lGL -lGLU -lglut -lglfw -lassimp

main: $(SRC_PATH)/$(SRC)
	$(CC) $(SRC_PATH)/main.cc -o main $(FLAGS) & $(CC) $(TEST_PATH)/gtest_test.cc -o test $(TEST_FLAGS)

.PHONY: clean
clean:
	-rm main.exe test.exe