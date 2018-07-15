CC = cl
FLAGS = /MTd /I./include


all: $(SRC_PATH)/$(SRC)
	$(CC) $(SRC_PATH)/main.cc -o main $(FLAGS) & $(CC) $(TEST_PATH)/gtest_test.cc -o gtest $(TEST_FLAGS) & ./gtest --gtest_output=xml:gtest-report.xml

bullet: ./src/test.cc
	$(CC) ./src/test.cc $(FLAGS)

.PHONY: clean
clean:
	-rm main test