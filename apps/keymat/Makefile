CC=gcc
RM=rm

BIN=keymat
SRC=keymat.c keymat_func.c
OBJ=$(SRC:.c=.o)
CFLAGS=
LDFLAGS=-lcrypto

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) $(BIN) $(OBJ)

