CC=gcc
RM=rm

BIN=sip_parser
SRCS=sip_parser.c sip_controller.c
OBJS=$(SRCS:.c=.o) 

all: sip_parser

sip_parser: $(OBJS)
	@echo "$(SRCS)"
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) -c $< -o $@
	@echo "CC <= $<"

clean:
	$(RM) $(OBJS) $(BIN)
