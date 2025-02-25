CC = g++
CFLAGS = -Wall -std=c++17
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = bin/ds_lib

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

