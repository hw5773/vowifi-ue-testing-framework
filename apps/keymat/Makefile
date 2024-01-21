CC=gcc
RM=rm

BIN=keymat extract
SRC=keymat.c keymat_func.c extract.c
OBJ=$(SRC:.c=.o)
CFLAGS=
LDFLAGS=-lcrypto

all: keymat extract

keymat: $(OBJ)
	$(CC) -o $@ keymat.o keymat_func.o $(LDFLAGS)

extract: $(OBJ)
	$(CC) -o $@ extract.o keymat_func.o $(LDFLAGS)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) $(BIN) $(OBJ)

