CC=gcc
RM=rm

BIN=calc_opc
SRC=calc_opc.c
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

