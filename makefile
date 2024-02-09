SRC_DIR=./src
BIN_DIR=./bin
CC=gcc
SRC=./src/main.c ./src/sock.c ./src/http_handle.c ./src/utils.c
HDR=constants.h http_handle.h sock.h utils.h
OBJ=./bin/main.o ./bin/sock.o ./bin/http_handle.o ./bin/utils.o
EXE=server
FLAGS=-g

$(EXE): $(OBJ)
	$(CC) $(FLAGS) -o $(EXE) $(OBJ)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXE)
