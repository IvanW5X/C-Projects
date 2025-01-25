CC=gcc
CFLAGS=-Wall -Wextra

TARGETS=enc_server enc_client dec_server dec_client keygen
OBJECTS=$(TARGETS:%=%.o)

all: $(TARGETS)

$(TARGETS): %: %.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGETS)
